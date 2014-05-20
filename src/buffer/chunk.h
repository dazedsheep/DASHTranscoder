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

#ifndef _CHUNK_H_
#define _CHUNK_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"

// the default initial chunk size, if not spezified
#define DEFAULT_CHUNK_SIZE      1024

class chunk{
    
protected:
    uint8_t *mem;
    uint32_t offset;
    uint32_t size;
    bool allowGrowth;
    
    void grow(uint32_t len);
public:
    
    chunk();
    chunk(uint32_t size, bool allowGrowth);
    ~chunk();
    
    int32_t append(uint8_t  *rdata, uint32_t len);
    int32_t append(chunk *ch);
    uint8_t *getPointer();
    uint32_t getSize();
    uint32_t getOffset();
    
    chunk* dup();

};

#endif