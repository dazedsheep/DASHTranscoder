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

#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include <stdint.h>
#include <vector>
#include "Stream.h"
#include "../format/Sample.h"
#include "../buffer/IObjectBuffer.h"

class Context : public dictionary {
    
private:
    std::vector<Stream *> streams;
    int32_t currentStream;
    IObjectBuffer<Sample> *sampleBuffer;
public:
    Context();
    ~Context();
    
    int32_t setSampleBuffer(IObjectBuffer<Sample> *buffer);
    IObjectBuffer<Sample> *getSampleBuffer();
    void detachSampleBuffer();
    
    int32_t newStream();
    Stream *getStream();
    Stream *getStream(uint32_t id);
    int32_t size();
    
    /* Debug dump*/
    void dumpEntriesOfStreams();
};
#endif