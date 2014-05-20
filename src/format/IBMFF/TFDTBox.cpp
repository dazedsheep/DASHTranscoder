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

#include "TFDTBox.h"


TFDTBox::TFDTBox(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->buffer = buffer;
    if(header != NULL) memcpy(&this->box.fullbox.header, header, sizeof(box_t));
    container = true;
    this->ctx = ctx;
}

TFDTBox::~TFDTBox()
{
}

bool TFDTBox::isContainer()
{
    return false;
}

uint32_t TFDTBox::getBoxType()
{
    return box.fullbox.header.type;
}

std::vector<IBox*> &TFDTBox::getIncludedBoxes()
{
    return boxes;
}

uint32_t TFDTBox::estimateSize(dictionary *inputParams)
{
    return FULLBOX_HEADER_SIZE + sizeof(uint32_t);
}

chunk *TFDTBox::dumpBox(dictionary *inputParams)
{
    
    uint8_t flag1, flag2;
    uint32_t decodeTime, n;
    
    inputParams->getDict<uint64_t>(MKSTRING(tfdt.decodeTime), &box.decodeTime);
    
    box.fullbox.header.size = (uint64_t) estimateSize(inputParams);
    chunk *c = new chunk(box.fullbox.header.size, false);
    
    n = htonl((uint32_t)box.fullbox.header.size);
    box.fullbox.header.type = htonl(TFDT_BOX);
    box.fullbox.version = (uint32_t)0;
    
    
    box.fullbox.flags[0] = box.fullbox.flags[1] = box.fullbox.flags[2] = 0;
    decodeTime = htonl((uint32_t)box.decodeTime);
    c->append((uint8_t *) &n, sizeof(n));
    c->append((uint8_t *) &box.fullbox.header.type, sizeof(box.fullbox.header.type));
    c->append((uint8_t *) &box.fullbox.version, sizeof(box.fullbox.version));
    c->append((uint8_t *) box.fullbox.flags, 3);
    c->append((uint8_t *) &decodeTime, sizeof(decodeTime));
    
    return c;
}