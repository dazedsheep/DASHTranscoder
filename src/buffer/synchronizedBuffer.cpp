/*
 *****************************************************************************
 * Copyright (C) 2012 - 2014 Institute of Information Technology,
 * Alpen-Adria-Universität Klagenfurt
 *
 * Created on: May 20, 2014
 * Authors: Benjamin Rainer <benjamin.rainer@itec.aau.at>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#if (defined WIN32 || WIN64)
#include <Windows.h>
#else
#include <pthread.h>
#endif

#include "synchronizedBuffer.h"

synchronizedBuffer::synchronizedBuffer()
{
    this->init();
}

synchronizedBuffer::synchronizedBuffer(uint32_t type)
{
    this->type = type;
    this->init();
}

synchronizedBuffer::~synchronizedBuffer()
{
    mutex_destroy(bufferMutex);
    mutex_destroy(observerMutex);
    event_destroy(bufferFull);
    event_destroy(bufferEmpty);
    if(mem != NULL) free(mem);
}


void synchronizedBuffer::init()
{
    
    bufferMutex = mutex_create(false);
    observerMutex = mutex_create(false);     // will prevent adding observer during notifying them, and to prevent problems when adding observer, the thread-safe version of std::queue will hold its own mutex but ...
    bufferFull = event_create();
    bufferEmpty = event_create();
    
    event_init(bufferFull);
    event_init(bufferEmpty);
    
    size = BUFFER_DEFAULT_SIZE;
    mem = (uint8_t *) calloc(sizeof(uint8_t), size);     // will init 32k
    offset = 0;
    realOffset = 0;
    bytesLeft = 0;
    eof = false;
    
    uid = *(uint32_t *)(this);
}

uint32_t synchronizedBuffer::write(chunk *c, uint32_t len)
{
    uint32_t ret;
    mutex_lock(bufferMutex);
    
    if(bytesLeft >= size || (((offset % size) + len) > size && bytesLeft > 0))
    {
        event_wait_without_unlock(bufferEmpty, bufferMutex);
        event_reset(bufferEmpty);
        offset = 0;
    }
    
    ret = (((offset % size) + len) > size ? (size - (offset % size)) : len);
    
    memcpy(mem + (offset % size), c->getPointer(), ret);
    
    bytesLeft += ret;
    
    this->notify();
    
    event_signal(bufferFull);           // signal that we have data ...
    mutex_unlock(bufferMutex);
    
    return len;
}

uint32_t synchronizedBuffer::read(chunk *c, uint32_t len)
{
    uint32_t ret;
    mutex_lock(bufferMutex);
    
    if(bytesLeft == 0 && !eof)
    {
        event_signal(bufferEmpty);
        event_wait_without_unlock(bufferFull, bufferMutex);     // will lock our mutex, when the event raised
        event_reset(bufferFull);
    }
    
    if(bytesLeft <= 0 && eof)
    {
        mutex_unlock(bufferMutex);
        return 0;
    }
    
    ret = (len > bytesLeft ? bytesLeft : len);
    
    c->append((mem + (offset % size)), ret);
  
    offset += ret;
    bytesLeft -= ret;
    realOffset += ret;
    
    mutex_unlock(bufferMutex);
    return ret;
}

uint32_t synchronizedBuffer::digest(uint32_t len)
{
    uint32_t readBytes = 0, ret=0;
    
    while(readBytes < len){
        chunk *c = new chunk(((len - readBytes) > size ? size : (len - readBytes)), false);
        ret = read(c, c->getSize());
        if(ret == 0 && eof) return readBytes;
        readBytes += ret;
        delete c;
    };
}


chunk *synchronizedBuffer::read(uint32_t len)
{
    uint32_t readBytes = 0, ret=0;
    chunk *c = new chunk(len, false);
    
    while(readBytes < len){
        
        ret = read(c, len - readBytes);
       
        if (eof && ret==0)
        {
            return c;
        }
        readBytes += ret;
    };
    
    return c;
}

uint32_t synchronizedBuffer::peek(chunk *pdata, uint32_t peekOffset, uint32_t len)
{
    uint32_t ret;
    mutex_lock(bufferMutex);
    
    if( (bytesLeft-peekOffset) == 0 && !eof)
    {
        event_signal(bufferEmpty);
        event_wait_without_unlock(bufferFull, bufferMutex);     // will lock our mutex, when the event raised
        event_reset(bufferFull);
    }
    
    if(bytesLeft <= 0 && eof)
    {
        mutex_unlock(bufferMutex);
        return 0;
    }

    ret = (len > bytesLeft ? bytesLeft : len);
    
    memcpy(pdata, mem + ((offset + peekOffset) % size), len);
    
    mutex_unlock(bufferMutex);
    return ret;
}

uint32_t synchronizedBuffer::peek(uint8_t *data, uint32_t len)
{
    uint32_t readBytes = 0, ret=0;
    chunk *c = new chunk(len, false);
    
    while(readBytes < len){
        
        ret = peek(c, readBytes, len - readBytes);
        readBytes += ret;
        if (eof && ret==0)
        {
            
            break;
        }
    };
    
    memcpy(data, c->getPointer(), ret);
    delete c;
    return ret;
}


chunk *synchronizedBuffer::read(uint32_t offset, uint32_t len)
{
    return NULL;            // not supported
}

void synchronizedBuffer::setEOF()
{
    mutex_lock(bufferMutex);
    
    eof = true;
    
    mutex_unlock(bufferMutex);
}

bool synchronizedBuffer::isEOF()
{
    return (bytesLeft == 0 && eof ? true : false);
}

uint64_t synchronizedBuffer::getOffset()
{
    return realOffset;
}

uint32_t synchronizedBuffer::bufferedBytes()
{
    return bytesLeft;
}

void synchronizedBuffer::notify()
{
    mutex_lock(observerMutex);

    for(uint32_t i = 0; i < observers.size(); ++i ) observers.at(i)->bufferLevelChanged(uid,(uint32_t)(bytesLeft/size));      // value between 0 and 100
    
    mutex_unlock(observerMutex);
}

void synchronizedBuffer::attach(IBufferObserver* observer)
{
    observers.push_back(observer);
}