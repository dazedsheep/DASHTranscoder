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
#include "ODBox.h"

ODBox::ODBox(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->buffer = buffer;
    if(header != NULL) memcpy(&this->box.fullbox.header, header, sizeof(box_t));
    this->ctx = ctx;
}

ODBox::~ODBox()
{
    uint32_t i;
    for(i = 0; i < boxes.size(); i++)
    {
        delete boxes.at(i);
    }
}

int32_t ODBox::parse()
{
    container = false;
}

bool ODBox::isContainer()
{
    return false;
}

uint32_t ODBox::getBoxType()
{
    return box.fullbox.header.type;
}

std::vector<IBox*> &ODBox::getIncludedBoxes()
{
    return boxes;
}

uint32_t ODBox::estimateSize(dictionary *inputParams)
{
    uint32_t i, size =FULLBOX_HEADER_SIZE + 9;
    return size;
}

/* dump the full box inclusive header and all included boxes */
chunk *ODBox::dumpBox(dictionary *inputParams)
{
    uint32_t i, n, size;
    uint32_t dummy32;
    uint8_t dummy8;
    /* compute the size of the box */
    box.fullbox.header.size = (uint64_t)estimateSize(inputParams);
    
    chunk *c = new chunk((uint32_t)box.fullbox.header.size, false);
    
    size = htonl((uint32_t)box.fullbox.header.size);
    box.fullbox.header.type = htonl(OD_BOX);
    box.fullbox.version = htonl(0);
    box.fullbox.flags[0] = box.fullbox.flags[1] = box.fullbox.flags[2] = 0;
  
    c->append((uint8_t*) &size, sizeof(uint32_t));
    c->append((uint8_t*) &box.fullbox.header.type, sizeof(uint32_t));
    c->append((uint8_t*) &box.fullbox.version, sizeof(box.fullbox.version));
    c->append((uint8_t*) &box.fullbox.flags[0], sizeof(uint8_t));
    c->append((uint8_t*) &box.fullbox.flags[1], sizeof(uint8_t));
    c->append((uint8_t*) &box.fullbox.flags[2], sizeof(uint8_t));

    dummy32 = htonl(0x1007004F);
    c->append((uint8_t*) &dummy32, sizeof(dummy32));
    dummy32 = htonl(0xFFFFFF15);
    c->append((uint8_t*) &dummy32, sizeof(dummy32));
    dummy8 = 0xFF;
    c->append((uint8_t*) &dummy8, sizeof(dummy8));
   /* dummy8 = 0x00;
    c->append((uint8_t*) &dummy8, sizeof(dummy8)); */
    for(i=0;i<boxes.size();i++) c->append(boxes.at(i)->dumpBox(inputParams));
    return c;
}

void ODBox::addBox(IBox *b)
{
    boxes.push_back(b);
}