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

#ifndef _CHUNKBUFFER_H_
#define _CHUNKBUFFER_H_

#include <stdint.h>
#include <string.h>
#include <vector>

#include "chunk.h"
#include "IBuffer.h"


#define BUFFER_DEFAULT_SIZE         32768

class chunkBuffer : public IBuffer
{
private:
    uint8_t *mem;
    
    uint32_t type;
    uint64_t offset;
    uint32_t bytesLeft;
    uint32_t size;
    uint32_t uid;
    bool eof;
    std::vector<IBufferObserver*> observers;
    
    uint32_t peek(chunk *c,uint32_t peekOffset, uint32_t len);
    uint32_t read(chunk *c, uint32_t len);
    
    void notify();
    
public:
    chunkBuffer(chunk *c);
    chunkBuffer(chunk *c, uint32_t off);
    virtual ~chunkBuffer();
    
       
    uint32_t write(chunk *c, uint32_t len);         // not supported
    
    chunk *read(uint32_t len);
    uint32_t digest(uint32_t len);
    chunk *read(uint32_t off, uint32_t len);

    uint32_t peek(uint8_t *pdata, uint32_t len);
    
    void setEOF();
    bool isEOF();
    
    uint64_t getOffset();
    uint32_t bufferedBytes();
    
    uint32_t getSize();
    
    void attach(IBufferObserver *observer);
    
};



#endif
