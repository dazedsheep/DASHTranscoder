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
 
#include <stdint.h>
#include "MFHDBox.h"

MFHDBox::MFHDBox(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->buffer = buffer;
    if(header != NULL) memcpy(&this->box.fullbox.header, header, sizeof(box_t));
    this->ctx = ctx;
}

MFHDBox::~MFHDBox()
{
}

int32_t MFHDBox::parse()
{
    container = false;
}

bool MFHDBox::isContainer()
{
    return false;
}

uint32_t MFHDBox::getBoxType()
{
    return box.fullbox.header.type;
}

std::vector<IBox*> &MFHDBox::getIncludedBoxes()
{
    return boxes;
}

uint32_t MFHDBox::estimateSize(dictionary *inputParams)
{
    return FULLBOX_HEADER_SIZE + sizeof(box.sequenceNumber);
}

/* dump the full box inclusive header and all included boxes */
chunk *MFHDBox::dumpBox(dictionary *inputParams)
{
    uint32_t n, size;
    
    /* compute the size of the box */
    box.fullbox.header.size = FULLBOX_HEADER_SIZE + sizeof(box.sequenceNumber);
    
    chunk *c = new chunk(box.fullbox.header.size, false);
    
    size = htonl(box.fullbox.header.size);
    box.fullbox.header.type = htonl(MFHD_BOX);
    box.fullbox.version = htonl(0);
    box.fullbox.flags[0] = box.fullbox.flags[1] = box.fullbox.flags[2] = 0;
    
    inputParams->getDict(MKSTRING(mfhd.sequenceNumber), &box.sequenceNumber);
    n = htonl((uint32_t)box.sequenceNumber);
    c->append((uint8_t*) &size, sizeof(uint32_t));
    c->append((uint8_t*) &box.fullbox.header.type, sizeof(uint32_t));
    c->append((uint8_t*) &box.fullbox.version, sizeof(box.fullbox.version));
    c->append((uint8_t*) &box.fullbox.flags[0], sizeof(uint8_t));
    c->append((uint8_t*) &box.fullbox.flags[1], sizeof(uint8_t));
    c->append((uint8_t*) &box.fullbox.flags[2], sizeof(uint8_t));
    c->append((uint8_t*) &n, sizeof(uint32_t));
    
    return c;
}