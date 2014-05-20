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
 
 
#ifndef _ISOM_H_
#define _ISOM_H_

#include <stdint.h>
#include <vector>

#include "../buffer/chunk.h"
#include "../buffer/IBuffer.h"
#include "../buffer/IBufferObserver.h"
#include "../buffer/synchronizedBuffer.h"
#include "../utils/Context.h"

#include "IFormat.h"

#include "IBMFF/boxReader.h"
#include "Sample.h"

#define ISOM_CONTAINER          MKTAG('i','s','o','m')
#define ISOM_CONTAINER_FLAGS    0                           /* the standard IBMFF contianer (no segmentation or other things) */

class isom : public IFormat {
private:
    Context *ctx;
    IBuffer *isoFile;
    boxReader *boxreader;
    std::vector<IBox *> boxes;
public:
    isom(Context *ctx);
    virtual ~isom();
    
    int32_t open(IBuffer* isoFile);
    int32_t parse(bool untilEOF);
    int32_t skipToMDAT();
    IBuffer *getRawStream();
    Sample *readSample();
    int32_t writeSample(Sample *sample);
};

#endif
