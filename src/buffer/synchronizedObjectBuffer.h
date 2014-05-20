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
 
#ifndef _SYNCHRONIZEDOBJECTBUFFER_H_
#define _SYNCHRONIZEDOBJECTBUFFER_H_

#include <stdint.h>
#include <string.h>
#include <vector>
#include <queue>

#include "../threadsys/mutex.h"
#include "../threadsys/event.h"
#include "../threadsys/thread.h"
#include "chunk.h"
#include "IObjectBuffer.h"
#include "log.h"

template<class T>
class synchronizedObjectBuffer : public IObjectBuffer<T>{
    
private:
    uint32_t type;
    uint32_t size;
    uint32_t maxSize;
    uint32_t uid;
    std::vector<IBufferObserver*> observers;
    std::queue<T *> container; /* could be easily exchanged */
    bool eof;
    
    mutex_t bufferMutex;
    threadEvent *bufferFullEvent;
    threadEvent *bufferEmptyEvent;
    
public:

    #include "synchronizedObjectBuffer.hpp"
    
};



#endif
