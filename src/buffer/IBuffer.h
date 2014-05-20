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

#ifndef _IBUFFER_H_
#define _IBUFFER_H_

#include "IBufferInfo.h"
#include "IBufferObserver.h"


/*
 * abstract class for buffers
 *
 * pushing and reading is based on chunks which are provided though chunk.cpp resp. chunk.h
 * chunks will provide you a pointer to the raw data
 */

class IBuffer{
    
public:
    virtual ~IBuffer(){};
    
    virtual uint32_t write(chunk *c, uint32_t len) = 0;
    
    // should maintain an own offset
    virtual chunk *read(uint32_t len) = 0;
    
    virtual uint32_t digest(uint32_t len) = 0;
    
    // used for seeking and reading
    virtual chunk *read(uint32_t off, uint32_t len) = 0;
    
    /* used for peeking (won't consume any byte or increase the real offset nor create data chunks */
    virtual uint32_t peek(uint8_t *pdata, uint32_t len) { return 0; };
    
    /* what sort of data is stored in this buffer, we will need this to distinguish 
       video and audio buffers, and maybe for storing sampling rates and so on */
    virtual IBufferInfo *getBufferInfo(){ return NULL; };
  
    virtual void setEOF() = 0;
    
    virtual bool isEOF() = 0;
    
    virtual uint32_t bufferedBytes() = 0;
    
    /* should allow for a continous offset */
    virtual uint64_t getOffset()=0;
    
    virtual uint32_t getSize() {  return 0; }
    
    virtual void attach(IBufferObserver *observer) = 0;
};

#endif