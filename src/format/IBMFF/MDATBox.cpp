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

#include "MDATBox.h"


MDATBox::MDATBox(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->buffer = buffer;
    if(header != NULL) memcpy(&this->box.header, header, sizeof(box_t));
    container = true;
    this->ctx = ctx;
}

MDATBox::~MDATBox()
{
}

bool MDATBox::isContainer()
{
    return false;
}

uint32_t MDATBox::getBoxType()
{
    return box.header.type;
}

std::vector<IBox*> &MDATBox::getIncludedBoxes()
{
    return boxes;
}

uint32_t MDATBox::estimateSize(dictionary *inputParams)
{
    return BOX_HEADER_SIZE;
}

chunk *MDATBox::dumpBox(dictionary *inputParams)
{
    uint32_t n;
    inputParams->getDict<uint32_t>(STRING("mdat.size"), &n);
    box.header.size = BOX_HEADER_SIZE + n;
    chunk *c = new chunk(estimateSize(inputParams), false);
    n = htonl(box.header.size);
    box.header.type = htonl(MDAT_BOX);
    c->append((uint8_t *) &n, sizeof(n));
    c->append((uint8_t *) &box.header.type, sizeof(box.header.type));
    return c;
}