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

#include "synchronizedFileBuffer.h"

synchronizedFileBuffer::synchronizedFileBuffer(int32_t fd)
{
    this->fd = new fdinout(fd);
    this->init();
}

synchronizedFileBuffer::synchronizedFileBuffer(std::string fileName)
{
    this->type = type;
    this->fd = new fdinout(fileName);
    this->init();
}

synchronizedFileBuffer::~synchronizedFileBuffer()
{
    mutex_destroy(bufferMutex);
    if(fd != NULL) delete fd;
    if(mem != NULL) free(mem);
}


void synchronizedFileBuffer::init()
{
    bufferMutex = mutex_create(false);
    size = BUFFER_DEFAULT_SIZE;
    mem = (uint8_t *) calloc(sizeof(uint8_t), size);     // will init 32k
    offset = 0;
    realOffset = 0;
    bytesLeft = 0;
    eof = false;
    peekCausedRewind = false;
    uid = *(uint32_t *)(this);
}

uint32_t synchronizedFileBuffer::write(chunk *c, uint32_t additionalOffset, uint32_t len)
{
    int32_t ret;
    
    ret = fd->get(mem + ((offset + additionalOffset) %  size), len);
    
    if(ret <= 0)
    {
        if(ret == _ERR_READING_FROM_FD) mde_log(MDE_LOG_ERROR, "synchronizedFileBuffer: Error reading from FD.\n");
        if(ret == _ERR_INVALID_FD) mde_log(MDE_LOG_ERROR, "synchronizedFileBuffer: Error invalid FD.\n");
        if(ret == _ERR_EOF) mde_log(MDE_LOG_DEBUG, "synchronizedFileBuffer: reached end of file.\n");
        this->setEOF();
        return 0;
    }
    
    bytesLeft += len;
    return (uint32_t)ret;
}

uint32_t synchronizedFileBuffer::write(chunk *c, uint32_t len)
{
    int32_t ret = 0;
    ret = fd->get(mem + (offset % size), len);
    if(ret <= 0)
    {
        if(ret == _ERR_READING_FROM_FD) mde_log(MDE_LOG_ERROR, "synchronizedFileBuffer: Error reading from FD.\n");
        if(ret == _ERR_INVALID_FD) mde_log(MDE_LOG_ERROR, "synchronizedFileBuffer: Error invalid FD.\n");
        if(ret == _ERR_EOF) mde_log(MDE_LOG_DEBUG, "synchronizedFileBuffer: reached end of file.\n");
        this->setEOF();
        return 0;
    }
    bytesLeft += len;
    return (uint32_t)ret;
}

uint32_t synchronizedFileBuffer::read(chunk *c, uint32_t len)
{
    uint32_t ret;
    mutex_lock(bufferMutex);

    /* be aware of the max read */
    if(((offset % size) + len > size)) len = (size - (offset % size));

    if(bytesLeft <= 0 && !eof)
    {
        if( this->write(NULL, len) == 0);
        {
            mutex_unlock(bufferMutex);
            return 0;
        }
    }
    
    if(bytesLeft <= 0 && eof)
    {
        mutex_unlock(bufferMutex);
        return 0;
    }
        
    ret = (len > bytesLeft ? bytesLeft : len);
    
    c->append(mem + (offset % size), ret);
    
    offset += ret;
    bytesLeft -= ret;
    realOffset += ret;
    
    mutex_unlock(bufferMutex);
    return ret;
}

uint32_t synchronizedFileBuffer::digest(uint32_t len)
{
    uint32_t readBytes = 0, ret=0;
    
    while(readBytes < len){
        chunk *c = new chunk(((len - readBytes) > size ? size : (len - readBytes)), false);
        ret = read(c, c->getSize());
        if(ret == 0 && eof) return readBytes;
        readBytes += ret;
        delete c;
    };
    return readBytes;
}

chunk *synchronizedFileBuffer::read(uint32_t len)
{
    uint32_t readBytes = 0, ret=0;
    chunk *c = new chunk(len, false);
    
    while(readBytes < len){
        
        ret = read(c, len - readBytes);
        readBytes += ret;
        if (eof && ret==0)
        {
            break;
        }
    };
    
    return c;
}

uint32_t synchronizedFileBuffer::peek(chunk *c, uint32_t peekOffset, uint32_t len)
{
    uint32_t ret;
    mutex_lock(bufferMutex);
    
    if((((offset + peekOffset) % size) + len > size)) len = (size - ((offset + peekOffset) % size));

    if((bytesLeft - peekOffset) <= 0 && !eof)
    {
        if(write(NULL, peekOffset, len) == 0)
        {
            mutex_unlock(bufferMutex);
            return 0;
        }
    }
    
    if((bytesLeft - peekOffset) <= 0 && eof)
    {
        mutex_unlock(bufferMutex);
        return 0;
    }
    
    if( peekCausedRewind ) peekOffset = 0;

    ret = (len > (bytesLeft - peekOffset) ? (bytesLeft - peekOffset) : len);
    c->append(mem + ((offset + peekOffset) % size), ret);
    
    mutex_unlock(bufferMutex);
    
    return ret;
}

uint32_t synchronizedFileBuffer::peek(uint8_t *data, uint32_t len)
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
    
    memcpy(data, c->getPointer(), readBytes);
    delete c;
    return readBytes;
}

chunk *synchronizedFileBuffer::read(uint32_t offset, uint32_t len)
{
    return NULL;            // not supported
}

void synchronizedFileBuffer::setEOF()
{
    eof = true;
}

bool synchronizedFileBuffer::isEOF()
{
    return eof;
}

uint64_t synchronizedFileBuffer::getOffset()
{
    return realOffset;
}

uint32_t synchronizedFileBuffer::bufferedBytes()
{
    return bytesLeft;
}

void synchronizedFileBuffer::notify()
{
}

void synchronizedFileBuffer::attach(IBufferObserver* observer)
{
}
