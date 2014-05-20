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

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>

#include "defs.h"
#include "../buffer/chunk.h"
#include "../buffer/IBuffer.h"



uint64_t _htonll(uint64_t value)
{
    int32_t check = 42;
    
    if(*reinterpret_cast<char *>(&check) == check)
    {
        // little endian
        const uint32_t high = htonl( static_cast<uint32_t>(value >> 32) );
        const uint32_t low = htonl( static_cast<uint32_t>(value & 0xFFFFFFLL));
        return ((static_cast<uint64_t>(low) << 32) | high);
    }else{
        // big endian
        return value;
    }
}

// not very efficient
uint8_t sys_rb8(IBuffer *buffer)
{
    chunk *c = buffer->read(sizeof(uint8_t));
    uint8_t ret = (uint8_t) *(c->getPointer());
    delete c;
    return ret;
}

uint8_t *sys_rstring(IBuffer *buffer)
{
    /* let the chunk grow */
    chunk *_str = new chunk(sizeof(uint8_t), true);
    uint8_t *mem,c;
    
    while((c = sys_rb8(buffer)) != NULL) _str->append(&c, sizeof(uint8_t));
    
    _str->append(&c, sizeof(uint8_t)); /* create a null terminated string */
    
    mem = (uint8_t *) malloc(_str->getOffset());
    memcpy(mem, _str->getPointer(), _str->getOffset());
    
    delete _str;
    return mem;
}

uint32_t sys_rtag(IBuffer *buffer)
{
    // tags are usually 4 bytes big
    char tag[TAG_LENGTH];
    int i;
    
    for(i=0;i<TAG_LENGTH;i++) tag[i] = sys_rb8(buffer);
    
    return MKTAG(tag[0], tag[1], tag[2], tag[3]);
}

uint16_t sys_rb16(IBuffer *buffer)
{
    chunk *c = buffer->read(sizeof(uint16_t));
    uint16_t ret = ntohs(*((uint16_t*)c->getPointer()));
    delete c;
    return ret;
}

uint32_t sys_rb32(IBuffer *buffer)
{
    chunk *c = buffer->read(sizeof(uint32_t));
    uint32_t ret = ntohl(*((uint32_t*)c->getPointer()));
    delete c;
    return ret;
}

uint64_t sys_rb64(IBuffer *buffer)
{
    chunk *c = buffer->read(sizeof(uint64_t));
    uint64_t ret = _htonll(*((uint64_t*)c->getPointer()));
    delete c;
    return ret;
}

void sys_wb8(uint8_t *mem, uint8_t b)
{
    *mem = b;
    mem++;
}

void sys_wb16(uint8_t *mem, uint16_t d)
{
    uint16_t *_mem = (uint16_t *)mem;
    *_mem = htons(d);
    mem += sizeof(uint16_t);
}

void sys_wb32(uint8_t *mem, uint32_t d)
{
    uint32_t *_mem = (uint32_t *)mem;
    *_mem = htonl(d);
    mem += sizeof(uint32_t);
}

void sys_wb64(uint8_t *mem, uint64_t d)
{
    uint64_t *_mem = (uint64_t *)mem;
    *_mem = _htonll(d);
    mem += sizeof(uint64_t);
}

uint8_t *sys_dup(uint8_t *data, uint32_t size)
{
    uint8_t *_mem = (uint8_t *) malloc(size);
    memcpy(_mem, data, size);
    return _mem;
}

uint32_t sys_strlen(uint8_t *str)
{
    uint8_t *p = str;
    while(*p != '\0') p++;
    return ((p - str)+1);
}

uint8_t *sys_strdup(uint8_t *str)
{
    return sys_dup(str, sys_strlen(str));
}

int32_t sys_strcompare(uint8_t *a, uint8_t *b)
{
    return strcmp((char *)a, (char *)b);
}