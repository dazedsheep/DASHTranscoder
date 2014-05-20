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
#ifndef _IBMFF_H_
#define _IBMFF_H_

#include <stdint.h>
#include <vector>

#include "../../threadsys/mutex.h"
#include "../../threadsys/event.h"
#include "../../threadsys/thread.h"
#include "../../threadsys/workerpool.h"
#include "../../threadsys/runable.h"

#include "../../buffer/chunk.h"

#include "../IMultiplexer.h"
#include "../../buffer/IBuffer.h"
#include "../../buffer/IBufferObserver.h"
#include "../../buffer/synchronizedBuffer.h"

#include "../../coder/IDecoderInformation.h"

#include "boxReader.h"


class IBMFF : public IMultiplexer, public runable {
    
private:
    
    IBuffer *isoFile;
    IDecoderInformation *decoderInfo;
   
    boxReader *boxreader;
    std::vector<IBuffer *> bufferList;
    
public:
    IBMFF();
    ~IBMFF();
    
    int32_t open(IBuffer* isoFile);
    
    int32_t multiplexSegmented(IDecoderInformation &decoderInfo, std::vector<IBuffer*> &streams);  // the sample and frame rates can be stored within the buffer info and will be identified via the type of the buffer
    
    // demuxing will return buffers including different streams (if there are more elementary streams muxed within the iso file
    IDecoderInformation *demultiplex(std::vector<IBuffer *> &out_BufferList);
   
    IDecoderInformation *getDecoderInformation();
    
    void run();
    void run_blocked();
    
    static void *demultiplexingThread(void *params);
    static void *multiplexingThread(void *params);
    
    
    
    
};

#endif
