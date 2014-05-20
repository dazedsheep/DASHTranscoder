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

#include "STSZBox.h"


STSZBox::STSZBox(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->buffer = buffer;
    if(header != NULL) memcpy(&this->box.fullbox.header, header, sizeof(box_t));
    container = false;
    this->ctx = ctx;
    box.sample_sizes = NULL;
}


STSZBox::~STSZBox()
{
    if(box.sample_sizes != NULL) free(box.sample_sizes);
}

bool STSZBox::isContainer()
{
    return false;
}

uint32_t STSZBox::getBoxType()
{
    return box.fullbox.header.type;
}

std::vector<IBox*> &STSZBox::getIncludedBoxes()
{
    return boxes;
}

int32_t STSZBox::parse()
{
    uint32_t i;
    box.fullbox.version = sys_rb8(buffer);
    box.fullbox.flags[0] = sys_rb8(buffer);
    box.fullbox.flags[1] = sys_rb8(buffer);
    box.fullbox.flags[2] = sys_rb8(buffer);
    box.sample_size = sys_rb32(buffer);
    box.sample_count = sys_rb32(buffer);
    box.sample_sizes = (uint32_t *) malloc(sizeof(uint32_t) * box.sample_count);
    for(i = 0; i < box.sample_count; i++)
    {
        box.sample_sizes[i] = sys_rb32(buffer);
    }
    
    return 0;
}

void STSZBox::printDebugInformation(bool callIncludedBoxes)
{
    mde_log(MDE_LOG_DEBUG, "isom: stsz box, number of samples: %d.\n", box.sample_count);
}

uint32_t STSZBox::estimateSize(dictionary *inputParams)
{
    return FULLBOX_HEADER_SIZE + sizeof(box.sample_count) + sizeof(box.sample_size);
}

chunk *STSZBox::dumpBox(dictionary *inputParams)
{
    uint32_t size;
    box.fullbox.header.type = htonl(STSZ_BOX);
    box.fullbox.version = 0;
    box.fullbox.flags[0] = 0;
    box.fullbox.flags[1] = 0;
    box.fullbox.flags[2] = 0;
    box.sample_count = 0;
    box.sample_size = 0;
    size = estimateSize(inputParams);
    chunk *c = new chunk(size, false);
    size = htonl(size);
    c->append((uint8_t *) &size, sizeof(size));
    c->append((uint8_t *) &box.fullbox.header.type, sizeof(box.fullbox.header.type));
    c->append((uint8_t *) &box.fullbox.version, sizeof(box.fullbox.version));
    c->append((uint8_t *) box.fullbox.flags, sizeofarray(box.fullbox.flags) * sizeof(uint8_t));
    c->append((uint8_t *) &box.sample_size, sizeof(box.sample_size));
    c->append((uint8_t *) &box.sample_count, sizeof(box.sample_count));
    return c;
}
