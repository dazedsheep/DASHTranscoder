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

#ifndef _AVC_H_
#define _AVC_H_

#include <stdint.h>
#include <vector>

#include "defs.h"
#include "../../buffer/chunk.h"
#include "../../buffer/IBuffer.h"
#include "../ICodec.h"
#include "../../format/Sample.h"

#include "log.h"
#include "sysreadwrite.h"

#define AVC_FOURCC                          MKTAG('a','v','c','1')
#define AVC_PREFIX_CODE_SIZE                0x03
#define AVC_TRIES                           8        /* 15 * 3 byte, for testing if it is a NAL unit */

typedef struct{
uint32_t value;
uint32_t *offset;
}value_offset_pair_t;


class AVC : public ICodec
{
private:
    const uint8_t *codecName;
    /* the stream with all the information of the container format (for this track/...), in our case this includes the PPS and SPS */
    Stream *stream;
    uint32_t fourcc;
    uint32_t IDRFrameNum;
    bool sameFrameNum;
    std::vector<chunk *> NALUnits;
    uint32_t IDRSlice, BSlice , PSlice;
    
    chunk *findNextNALUnit(IBuffer *buffer);
    chunk *getRBSPFromNALUnit(chunk *c, void *header);
   
    int32_t parseNALUnit(chunk *nalUnit);
    
    void parseSlice(chunk *rbsp);
    void parseSPS(chunk *rbsp);
    void parsePPS(chunk *rbsp);
    
    void checkSliceHeader(chunk *rbsp);
    
    void getParameterSets();
    
public:
    AVC(Stream *stream);
    ~AVC();
    
    /* several NAL units form an access unit, but for the sake of simplicity we first search for NAL units and then glue them together to access units ... */
    int32_t testForRawBitStream(IBuffer *buffer);
    int32_t writeRawBitStream(const char *fileName);
    chunk *parseRawBitStream(IBuffer *buffer);
    int32_t parseFromContainer(chunk *data);
    const uint8_t *getCodecName();
    uint32_t getCodecFourCC();
};
#endif