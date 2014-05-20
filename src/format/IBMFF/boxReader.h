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

#ifndef _BOXREADER_H_
#define _BOXREADER_H_

#include <stdint.h>
#include <string>

#include "../../buffer/chunk.h"
#include "../../buffer/IBuffer.h"
#include "../../buffer/IBufferObserver.h"
#include "../../utils/Context.h"
#include "IBox.h"
#include "boxFactory.h"

#include "../IMultiplexer.h"

class boxReader{
    
private:
    
    std::vector<IBox *> parsedBoxes;
    
public:
    boxReader();
    ~boxReader();
    
    IBox *parseBox(Context *ctx, uint32_t boxType, IBuffer *isoFile, box_t *boxHeader);
    std::vector<IBox *> &parseBoxes(Context *ctx, IBuffer *isoFile, bool hierarchical, bool untilEOF);
    std::vector<IBox *> &parseBoxes(Context *ctx, IBuffer *isoFile, uint32_t *boxList, uint32_t listLength);
    IBox *readBox(Context *ctx, IBuffer *isoFile, uint32_t type);
    IBox *readBox(Context *ctx, IBuffer *isoFile);
    int32_t skipUntilMDAT(IBuffer *isoFile);
    void readBoxHeader(IBuffer *isoFile, box_t *header);
};


#endif