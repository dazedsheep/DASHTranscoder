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

#ifndef _DEMUX_H_
#define _DEMUX_H_

#include <stdint.h>
#include <vector>

#include "../threadsys/mutex.h"
#include "../threadsys/event.h"
#include "../threadsys/thread.h"
#include "../threadsys/workerPool.h"
#include "../threadsys/runable.h"
#include "../buffer/chunk.h"
#include "IMultiplexer.h"
#include "../buffer/IBuffer.h"
#include "../buffer/IObjectBuffer.h"
#include "../buffer/IBufferObserver.h"
#include "../buffer/synchronizedBuffer.h"
#include "../coder/ICodec.h"
#include "../utils/Context.h"
#include "../coder/AVC/avc.h"
#include "../coder/codecs.h"
#include "IFormat.h"

#include "log.h"

class demux : public IMultiplexer, public runable {
    
private:
    IFormat *parsedContainer;
    IBuffer *rawStream;
    std::vector<ICodec *> coders;
    Context *ctx;
    IObjectBuffer<Sample> *objectBuffer;
    void initCodecs();
    void demultiplex();
    
public:
    demux(Context *ctx, IFormat *parsedContainer);
    demux(Context *ctx, IBuffer *rawStream);
    ~demux();
    
    void run();
    void run_blocked();
    
    static void *demultiplexingThread(void *params);
};

#endif
