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

#ifndef _MDATBOX_H_
#define _MDATBOX_H_

#include <stdint.h>

#include "boxes.h"
#include "log.h"
#include "../../buffer/chunk.h"
#include "../../buffer/IBuffer.h"
#include "IBox.h"
#include "defs.h"
#include "sysreadwrite.h"
#include "boxReader.h"
#include "../../utils/Context.h"

class MDATBox : public IBox
{
private:
    mdat_t box;
    bool container;
    IBuffer *buffer;
    std::vector<IBox*> boxes;
    Context *ctx;
public:
    MDATBox(Context *ctx, IBuffer *buffer, box_t *header);     // will try to parse the box directly from the given chunk
    virtual ~MDATBox();
    bool isContainer();
    uint32_t getBoxType();
    std::vector<IBox*> &getIncludedBoxes();
    chunk *dumpBox(dictionary *inputParams);
    uint32_t estimateSize(dictionary *inpuParams);
};


#endif
