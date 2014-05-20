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

#ifndef _TREXBOX_H_
#define _TREXBOX_H_

#include "boxes.h"
#include "log.h"
#include "../../buffer/chunk.h"
#include "../../buffer/IBuffer.h"
#include "IBox.h"
#include "defs.h"
#include "sysreadwrite.h"
#include "../../utils/Context.h"

class TREXBox : public IBox
{
private:
    trex_t box;
    IBuffer *buffer;
    bool container;
    std::vector<IBox*> boxes;
    Context *ctx;
public:
    TREXBox(Context *ctx, IBuffer *buffer, box_t *header);     // will try to parse the box directly from the given chunk
    virtual ~TREXBox();
    bool isContainer();
    uint32_t getBoxType();
    std::vector<IBox*> &getIncludedBoxes();
    chunk *dumpBox(dictionary *inputParams);
    int32_t parse();
    uint32_t estimateSize(dictionary *inputParams);
    void addBox(IBox *b);
};


#endif 