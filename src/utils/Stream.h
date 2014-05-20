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

#ifndef _STREAM_H
#define _STREAM_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <vector>

#include "../buffer/chunk.h"
#include "../buffer/IBuffer.h"

#include "defs.h"
#include "sysreadwrite.h"
#include "../utils/dictionary.h"
#include "IStream.h"
#include "../format/Sample.h"

#define     STREAM_TYPE_VIDEO       0x010101
#define     STREAM_TYPE_AUDIO       0x101010
#define     STREAM_TYPE_METADATA    0x000000

class Stream : public IStream, public dictionary {

private:
    uint32_t fourcc;    /* used for distinguishing codecs */
    std::vector<Sample *> *samples;
public:
    Stream();
    ~Stream();
    
    uint32_t getFourCC();
    void setFourCC(uint32_t tag);
    
    void setSamples(std::vector<Sample*> *_s);
    Sample *getSample(uint32_t sample);
    /* Debug dumb */
    void dumbEntries();
};

#endif