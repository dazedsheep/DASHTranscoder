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

#ifndef _SYNCHRONIZEDFILEBUFFER_H_
#define _SYNCHRONIZEDFILEBUFFER_H_

#include <stdint.h>
#include <string.h>
#include <vector>
#include <string>
#include <fcntl.h>
#include <unistd.h>

#include "../threadsys/mutex.h"
#include "../threadsys/event.h"
#include "../threadsys/thread.h"

#include "../inoutformat/fdinout.h"

#include "log.h"

#include "chunk.h"
#include "IBuffer.h"


#define BUFFER_DEFAULT_SIZE         32768

class synchronizedFileBuffer : public IBuffer
{
private:

    uint8_t *mem;
   
    uint32_t type;
    uint32_t peekBytes;
    uint64_t offset;
    uint64_t realOffset;
    uint32_t bytesLeft;
    uint32_t size;
    uint32_t uid;
    
    fdinout *fd;
    bool peekCausedRewind;
    bool eof;
    
    mutex_t bufferMutex;
    
    
    uint32_t read(chunk *c, uint32_t len);
    
    void notify();
    
    uint32_t write(chunk *c, uint32_t additionalOffset, uint32_t len);
    uint32_t peek(chunk *c,uint32_t peekOffset, uint32_t len);

    
public:
    synchronizedFileBuffer(int32_t fd);
    synchronizedFileBuffer(std::string fileName);
    
    virtual ~synchronizedFileBuffer();
    
    void init();
    
    uint32_t write(chunk *c, uint32_t len);
    
    chunk *read(uint32_t len);
    uint32_t digest(uint32_t len);
    chunk *read(uint32_t off, uint32_t len);
    
    uint32_t peek(uint8_t *pdata, uint32_t len);

    void setEOF();
    bool isEOF();
    
    uint64_t getOffset();
    
    uint32_t bufferedBytes();
    
    void attach(IBufferObserver *observer);
    
};



#endif
