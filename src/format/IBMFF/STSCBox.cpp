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

#include "STSCBox.h"


STSCBox::STSCBox(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->ctx = ctx;
    if(header != NULL) memcpy(&box.fullbox.header, header, sizeof(box.fullbox.header));
    this->buffer = buffer;
    this->box.entries = NULL;
}

STSCBox::~STSCBox()
{
    if(box.entries != NULL) free(box.entries);
}

bool STSCBox::isContainer()
{
    return false;
}

int32_t STSCBox::parse()
{
    uint32_t i;
    
    box.fullbox.version = sys_rb8(buffer);
    box.fullbox.flags[0] = sys_rb8(buffer);
    box.fullbox.flags[1] = sys_rb8(buffer);
    box.fullbox.flags[2] = sys_rb8(buffer);
    
    box.entry_count = sys_rb32(buffer);
    
    box.entries = (stsc_entry_t *) malloc( sizeof(stsc_entry_t) * box.entry_count);
    
    for(i = 0; i < box.entry_count; i++)
    {
        box.entries[i].first_chunk = sys_rb32(buffer);
        box.entries[i].samples_per_chunk = sys_rb32(buffer);
        box.entries[i].sample_description_index = sys_rb32(buffer);
    }
    
    
    
    return 0;
}

uint32_t STSCBox::getBoxType()
{
    return box.fullbox.header.type;
}

std::vector<IBox*> &STSCBox::getIncludedBoxes()
{
    return boxes;
}

void STSCBox::printDebugInformation(bool callIncludedBoxes)
{
//    uint32_t i;
//    mde_log(MDE_LOG_DEBUG, "isom: stsc box entries %d.\n", box.entry_count);
//    for(i = 0; i < box.entry_count; i++) mde_log(MDE_LOG_DEBUG, "isom: stsc box entry 0x%d: first chunk %x, samples per chunk %d, sample description index %d.\n", box.entries[i].first_chunk, box.entries[i].samples_per_chunk, box.entries[i].sample_description_index);
}

uint32_t STSCBox::estimateSize(dictionary *inputParams)
{
    return FULLBOX_HEADER_SIZE + sizeof(box.entry_count);
}

chunk *STSCBox::dumpBox(dictionary *inputParams)
{
    uint32_t size;
    box.fullbox.header.type = htonl(STSC_BOX);
    box.fullbox.version = 0;
    box.fullbox.flags[0] = 0;
    box.fullbox.flags[1] = 0;
    box.fullbox.flags[2] = 0;
    size = estimateSize(inputParams);
    
    chunk *c = new chunk(size, false);
    size = htonl(size);
    box.entry_count = 0;
    
    c->append((uint8_t*) &size, sizeof(size));
    c->append((uint8_t*) &box.fullbox.header.type, sizeof(box.fullbox.header.type));
    c->append((uint8_t*) &box.fullbox.version, sizeof(box.fullbox.version));
    c->append((uint8_t*) &box.fullbox.flags[0], sizeof(uint8_t));
    c->append((uint8_t*) &box.fullbox.flags[1], sizeof(uint8_t));
    c->append((uint8_t*) &box.fullbox.flags[2], sizeof(uint8_t));
    c->append((uint8_t*) &box.entry_count, sizeof(box.entry_count));
   
    
    return c;
}
