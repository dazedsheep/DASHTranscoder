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

#include "STCOBox.h"


STCOBox::STCOBox(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->buffer = buffer;
    if(header != NULL) memcpy(&this->box.fullbox.header, header, sizeof(box_t));
    container = false;
    this->ctx = ctx;
    box.chunk_offsets = NULL;
}

STCOBox::~STCOBox()
{
    if(box.chunk_offsets != NULL) free(box.chunk_offsets);
}

bool STCOBox::isContainer()
{
    return false;
}

uint32_t STCOBox::getBoxType()
{
    return box.fullbox.header.type;
}

std::vector<IBox*> &STCOBox::getIncludedBoxes()
{
    return boxes;
}

int32_t STCOBox::parse()
{
    uint32_t i;
    box.fullbox.version = sys_rb8(buffer);    
    box.fullbox.flags[0] = sys_rb8(buffer);
    box.fullbox.flags[1] = sys_rb8(buffer);
    box.fullbox.flags[2] = sys_rb8(buffer);
    box.entry_count = sys_rb32(buffer);
    box.chunk_offsets = (uint32_t *) malloc(sizeof(uint32_t) * box.entry_count);
    for(i = 0; i < box.entry_count; i++)
    {
        box.chunk_offsets[i] = sys_rb32(buffer);
    }
    return 0;
}

void STCOBox::printDebugInformation(bool callIncludedBoxes)
{
    mde_log(MDE_LOG_DEBUG, "isom: stco box number of chunk offsets: %d.\n", box.entry_count);
    uint32_t i;
    for(i = 0; i < box.entry_count; i++)
    {
        mde_log(MDE_LOG_DEBUG, "isom: stco box, chunk offset %d: 0x%x (%d).\n", i, box.chunk_offsets[i], box.chunk_offsets[i]);
    }
}

uint32_t STCOBox::estimateSize(dictionary *inputParams)
{
    return FULLBOX_HEADER_SIZE + sizeof(box.entry_count);
}

chunk *STCOBox::dumpBox(dictionary *inputParams)
{
    uint32_t size;
    box.fullbox.header.type = htonl(STCO_BOX);
    box.fullbox.version = 0;
    box.fullbox.flags[0] = 0;
    box.fullbox.flags[1] = 0;
    box.fullbox.flags[2] = 0;
    box.entry_count = htonl(boxes.size());
    size = estimateSize(inputParams);
    chunk *c = new chunk(size, false);
    size = htonl(size);
    box.entry_count = 0;
    c->append((uint8_t *) &size, sizeof(size));
    c->append((uint8_t *) &box.fullbox.header.type, sizeof(box.fullbox.header.type));
    c->append((uint8_t *) &box.fullbox.version, sizeof(box.fullbox.version));
    c->append((uint8_t *) box.fullbox.flags, sizeofarray(box.fullbox.flags) * sizeof(uint8_t));
    c->append((uint8_t *) &box.entry_count, sizeof(box.entry_count));
    return c;
}
