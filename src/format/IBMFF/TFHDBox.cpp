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

#include "TFHDBox.h"


TFHDBox::TFHDBox(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->buffer = buffer;
    if(header != NULL) memcpy(&this->box.fullbox.header, header, sizeof(box_t));
    container = true;
    this->ctx = ctx;
}

TFHDBox::~TFHDBox()
{
}

bool TFHDBox::isContainer()
{
    return false;
}

uint32_t TFHDBox::getBoxType()
{
    return box.fullbox.header.type;
}

std::vector<IBox*> &TFHDBox::getIncludedBoxes()
{
    return boxes;
}

uint32_t TFHDBox::estimateSize(dictionary *inputParams)
{
    return FULLBOX_HEADER_SIZE + sizeof(box.trackId);
}

chunk *TFHDBox::dumpBox(dictionary *inputParams)
{
    
    uint8_t flag1, flag2;
    uint32_t n, trackId, offset, f, size, sdi, res;

    inputParams->getDict(MKSTRING(tfhd.trackId), &box.trackId);
 
    box.fullbox.header.size = (uint64_t)estimateSize(inputParams);
    chunk *c = new chunk((uint32_t)box.fullbox.header.size, false);
    
    size = htonl((uint32_t)box.fullbox.header.size);
    box.fullbox.header.type = htonl(TFHD_BOX);
    box.fullbox.version = 0;
    box.fullbox.flags[0] = box.fullbox.flags[1] = box.fullbox.flags[2] = 0;
    trackId = htonl(box.trackId);
    c->append((uint8_t *) &size, sizeof(size));
    c->append((uint8_t *) &box.fullbox.header.type, sizeof(box.fullbox.header.type));
    c->append((uint8_t *) &box.fullbox.version, sizeof(box.fullbox.version));
    c->append((uint8_t *) box.fullbox.flags, 3);
    
    // for now we only need the trackId
    c->append((uint8_t*) &trackId, sizeof(trackId));    
    return c;
}