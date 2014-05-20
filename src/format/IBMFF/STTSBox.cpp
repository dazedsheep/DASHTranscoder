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

#include "STTSBox.h"

STTSBox::STTSBox(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->buffer = buffer;
    if(header != NULL) memcpy(&this->box.fullbox.header, header, sizeof(box_t));
    this->ctx = ctx;
}

STTSBox::~STTSBox()
{
}

bool STTSBox::isContainer()
{
    return false;
}

uint32_t STTSBox::getBoxType()
{
    return 0;
}

std::vector<IBox*> &STTSBox::getIncludedBoxes()
{
    return boxes;
}

uint32_t STTSBox::estimateSize(dictionary *inputParams)
{
    return FULLBOX_HEADER_SIZE + sizeof(box.entry_count);
}

chunk *STTSBox::dumpBox(dictionary *inputParams)
{
    uint32_t size;
    box.fullbox.header.type = htonl(STTS_BOX);
    box.fullbox.version = 0;
    box.fullbox.flags[0] = 0;
    box.fullbox.flags[1] = 0;
    box.fullbox.flags[2] = 0;
    box.entry_count = htonl(boxes.size());
    size = estimateSize(inputParams);
    chunk *c = new chunk(size, false);
    size = htonl(size);
    c->append((uint8_t *) &size, sizeof(size));
    c->append((uint8_t *) &box.fullbox.header.type, sizeof(box.fullbox.header.type));
    c->append((uint8_t *) &box.fullbox.version, sizeof(box.fullbox.version));
    c->append((uint8_t *) box.fullbox.flags, sizeofarray(box.fullbox.flags) * sizeof(uint8_t));
    c->append((uint8_t *) &box.entry_count, sizeof(box.entry_count));
    return c;
}