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

#include "chunk.h"
#include "log.h"
// a chunk will hold the data, it will not free anything that is appended to it

chunk::chunk()
{
    this->size = DEFAULT_CHUNK_SIZE;
    mem = (uint8_t *)calloc(size, sizeof(uint8_t));
    memset(mem, 0x66, this->size * sizeof(uint8_t));
    offset = 0;
    allowGrowth = false;
}

chunk::chunk(uint32_t size, bool allowGrowth)
{
    this->size = (size > 0 ? size : DEFAULT_CHUNK_SIZE);
    mem = (uint8_t *)calloc(this->size, sizeof(uint8_t));
    memset(mem, 0x66, this->size * sizeof(uint8_t));
    offset = 0;
    this->allowGrowth = allowGrowth;
}

chunk::~chunk()
{
    if(mem != NULL) free(mem);
}

uint32_t chunk::getSize()
{
    
    return this->size;
    
}

uint32_t chunk::getOffset()
{
    
    return this->offset;
    
}

void chunk::grow(uint32_t len)
{
    uint32_t oldsize = this->size;
    this->size += len;
    uint8_t *newMemLocation = (uint8_t *) malloc(this->size * sizeof(uint8_t));
    memcpy(newMemLocation, mem, sizeof(uint8_t) * oldsize);
    free(mem);
    mem = newMemLocation;
    // the offset can be let untouched because we just extended our space. ..    
}

int32_t chunk::append(chunk *ch)
{
    int32_t ret;
    
    if((this->offset + ch->getSize()) > size)
    {
        if(!this->allowGrowth)
        {
            mde_log(MDE_LOG_ERROR, "%s: error at appending data to current chunk (0x%x), chunk is smaller than the current data to append plus the precious size.\n", __FILE__, this);
            return _ERR_OUT_OF_BOUNDS;
        }else
            this->grow( (ch->getSize() > DEFAULT_CHUNK_SIZE ? ch->getSize() : DEFAULT_CHUNK_SIZE) ); // do not only grow by the wanted amount if len << DEFAULT_CHUNK_SIZE
    }
    
    
    ret = this->append(ch->getPointer(), ch->getSize());
    
    
    delete ch;  // get rid of the merged chunk
    
    return ret;
}

int32_t chunk::append(uint8_t *rdata, uint32_t len)
{
    if((this->offset + len) > size)
    {
        if(!this->allowGrowth) return _ERR_OUT_OF_BOUNDS;
            else
                this->grow( (len > DEFAULT_CHUNK_SIZE ? len : DEFAULT_CHUNK_SIZE) ); // do not only grow by the wanted amount if len << DEFAULT_CHUNK_SIZE
    }
    
    memcpy(this->mem + this->offset, rdata, len);
    this->offset += len;
    return len;
}

uint8_t *chunk::getPointer()
{
    return this->mem;
}

chunk *chunk::dup()
{
    chunk *c = new chunk(size, allowGrowth);
    c->append(this->getPointer(), this->getOffset());
    return c;
}


