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
#include "MVEXBox.h"

MVEXBox::MVEXBox(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->buffer = buffer;
    if(header != NULL) memcpy(&this->box.header, header, sizeof(box_t));
    this->ctx = ctx;
}

MVEXBox::~MVEXBox()
{
    uint32_t i;
    for(i = 0; i < boxes.size(); i++)
    {
        delete boxes.at(i);
    }
}

int32_t MVEXBox::parse()
{
    container = true;
}

bool MVEXBox::isContainer()
{
    return true;
}

uint32_t MVEXBox::getBoxType()
{
    return box.header.type;
}

std::vector<IBox*> &MVEXBox::getIncludedBoxes()
{
    return boxes;
}

uint32_t MVEXBox::estimateSize(dictionary *inputParams)
{
    uint32_t i, size = BOX_HEADER_SIZE;
    for(i = 0; i < boxes.size(); i++) size += boxes.at(i)->estimateSize(inputParams);
    return size;
}

/* dump the full box inclusive header and all included boxes */
chunk *MVEXBox::dumpBox(dictionary *inputParams)
{
    uint32_t i, size;
    
    /* compute the size of the box */
    box.header.size = (uint64_t)estimateSize(inputParams);
    
    chunk *c = new chunk((uint32_t)box.header.size, false);
    size = htonl((uint32_t)box.header.size);
    box.header.type = htonl(MVEX_BOX);
    c->append((uint8_t*) &size, sizeof(uint32_t));
    c->append((uint8_t*) &box.header.type, sizeof(uint32_t));
    for(i=0;i<boxes.size();i++) c->append(boxes.at(i)->dumpBox(inputParams));
    return c;
}

void MVEXBox::addBox(IBox *b)
{
    boxes.push_back(b);
}