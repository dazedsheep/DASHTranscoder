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

#ifndef _FRAGMENTED_ISOM_
#define _FRAGMENTED_ISOM_

#include <stdint.h>
#include <vector>



#include "../buffer/chunk.h"
#include "../buffer/IBuffer.h"
#include "../buffer/IBufferObserver.h"
#include "../buffer/synchronizedBuffer.h"
#include "../utils/Context.h"
#include "../inoutformat/fdinout.h"
#include "IFormat.h"

#include "IBMFF/boxFactory.h"
#include "IBMFF/boxReader.h"
#include "Sample.h"

#define FRAGMENTED_ISOM         MKTAG('s','e','g','m')
#define MVHD_DEFAULT_TIMESCALE  600

class fragmentedIsom : public IFormat{
private:
    char currentTime[200];
    fdinout *file;
    /* list of boxes in the correct order */
    std::vector<IBox *> boxes;
    std::vector<Sample *> *samples;
    Context *ctx;
    uint32_t segmentSize;   /* in number of samples */
    uint32_t sequence;
    uint32_t timescale;
    uint8_t *baseName;
    uint32_t segmentSizeInSeconds;
    uint32_t fps;
    uint32_t do_once;
    uint32_t frame_number;
    uint32_t parts;
public:
    fragmentedIsom(Context *ctx);
    ~fragmentedIsom();
    int32_t createInitSegment();
    int32_t writeSample(Sample *sample);
    int32_t end(uint32_t trackId);
    Sample *readSample();
};

#endif
