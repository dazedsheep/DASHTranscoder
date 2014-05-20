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

#include "chunkBuffer.h"

chunkBuffer::chunkBuffer(chunk *c)
{
    size = c->getSize();
    mem = (uint8_t *) calloc(sizeof(uint8_t), size);
    memcpy(mem, c->getPointer(), size);
    offset = 0;
    bytesLeft = size;
    eof = false;
    uid = *(uint32_t *)(this);
}

chunkBuffer::chunkBuffer(chunk *c, uint32_t off)
{
    if(off > c->getSize()) return;

    size = c->getSize() - offset;
    mem = (uint8_t *) calloc(sizeof(uint8_t), size);
    memcpy(mem, c->getPointer() + off, size);
    offset = 0;
    bytesLeft = size;
    eof = false;
    uid = *(uint32_t *)(this);
}

chunkBuffer::~chunkBuffer()
{
    if(mem != NULL) free(mem);    
}

uint32_t chunkBuffer::write(chunk *c, uint32_t len)
{
    delete c;
    return 0;
}

uint32_t chunkBuffer::read(chunk *c, uint32_t len)
{
    uint32_t ret;
    
    if(bytesLeft == 0) return 0;
    
    ret = (len > bytesLeft ? bytesLeft : len);
    
    c->append(mem + offset, ret);
    
    offset += ret;
    bytesLeft -= ret;
    
    return ret;
}

uint32_t chunkBuffer::digest(uint32_t len)
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

chunk *chunkBuffer::read(uint32_t len)
{
    uint32_t readBytes = 0, ret=0;
    chunk *c = new chunk(len, false);
    
    while(readBytes < len){
        
        ret = read(c, len - readBytes);
        readBytes += ret;
        
        if (ret==0)
        {
            return c;
        }
        
    };
    return c;
}

chunk *chunkBuffer::read(uint32_t offset, uint32_t len)
{
    return NULL;            // not supported
}

uint32_t chunkBuffer::peek(chunk *c,uint32_t peekOffset, uint32_t len)
{
    uint32_t ret;
  
    if((bytesLeft - peekOffset) <= 0) return 0;
    
    ret = (len > (bytesLeft - peekOffset) ? (bytesLeft - peekOffset) : len);
    c->append(mem + offset + peekOffset, ret);
    
    return ret;
}

uint32_t chunkBuffer::peek(uint8_t *data, uint32_t len)
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

void chunkBuffer::setEOF()
{
    eof = true;
}

bool chunkBuffer::isEOF()
{
    return eof;
}

uint64_t chunkBuffer::getOffset()
{
    return offset;
}

uint32_t chunkBuffer::bufferedBytes()
{
    return bytesLeft;
}

uint32_t chunkBuffer::getSize()
{
    return size;
}

void chunkBuffer::notify()
{
    //NYI
}

void chunkBuffer::attach(IBufferObserver* observer)
{
    //NYI
}