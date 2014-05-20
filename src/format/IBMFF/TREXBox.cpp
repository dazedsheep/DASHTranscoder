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
#include "TREXBox.h"

TREXBox::TREXBox(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->buffer = buffer;
    if(header != NULL) memcpy(&this->box.fullbox.header, header, sizeof(box_t));
    this->ctx = ctx;
}

TREXBox::~TREXBox()
{
    uint32_t i;
    for(i = 0; i < boxes.size(); i++)
    {
        delete boxes.at(i);
    }
}

int32_t TREXBox::parse()
{
    container = false;
}

bool TREXBox::isContainer()
{
    return false;
}

uint32_t TREXBox::getBoxType()
{
    return box.fullbox.header.type;
}

std::vector<IBox*> &TREXBox::getIncludedBoxes()
{
    return boxes;
}

uint32_t TREXBox::estimateSize(dictionary *inputParams)
{
    uint32_t size = FULLBOX_HEADER_SIZE + sizeof(box.trackId) + sizeof(box.default_sample_description_index) + sizeof(box.default_sample_duration) + sizeof(box.default_sample_size) + sizeof(box.default_sample_flags);
    return size;
}

/* dump the full box inclusive header and all included boxes */
chunk *TREXBox::dumpBox(dictionary *inputParams)
{
    uint32_t i, n, size;
    
    /* compute the size of the box */
    box.fullbox.header.size = (uint64_t)estimateSize(inputParams);

    chunk *c = new chunk((uint32_t)box.fullbox.header.size, false);

    inputParams->getDict(MKSTRING(trex.trackId), &box.trackId);
    inputParams->getDict(MKSTRING(trex.default_sample_description_index), &box.default_sample_description_index);
    inputParams->getDict(MKSTRING(trex.default_sample_duration), &box.default_sample_duration);
    inputParams->getDict(MKSTRING(trex.default_sample_size), &box.default_sample_size);
    inputParams->getDict(MKSTRING(trex.default_sample_flags), &box.default_sample_flags);
    
    size = htonl((uint32_t)box.fullbox.header.size);
    box.fullbox.header.type = htonl(TREX_BOX);
    box.fullbox.version = htonl(0);
    box.fullbox.flags[0] = box.fullbox.flags[1] = box.fullbox.flags[2] = 0;

    box.trackId = htonl(box.trackId);
    box.default_sample_flags = htonl(box.default_sample_flags);
    box.default_sample_description_index = htonl(box.default_sample_description_index);
    box.default_sample_size = htonl(box.default_sample_size);
    box.default_sample_duration = htonl(box.default_sample_duration);
    
    c->append((uint8_t*) &size, sizeof(uint32_t));
    c->append((uint8_t*) &box.fullbox.header.type, sizeof(uint32_t));
    c->append((uint8_t*) &box.fullbox.version, sizeof(box.fullbox.version));
    c->append((uint8_t*) &box.fullbox.flags[0], sizeof(uint8_t));
    c->append((uint8_t*) &box.fullbox.flags[1], sizeof(uint8_t));
    c->append((uint8_t*) &box.fullbox.flags[2], sizeof(uint8_t));
    c->append((uint8_t*) &box.trackId, sizeof(box.trackId));
    c->append((uint8_t*) &box.default_sample_description_index, sizeof(box.default_sample_description_index));
    c->append((uint8_t*) &box.default_sample_duration, sizeof(box.default_sample_duration));
    c->append((uint8_t*) &box.default_sample_size, sizeof(box.default_sample_size));
    c->append((uint8_t*) &box.default_sample_flags, sizeof(box.default_sample_flags));
      
    return c;
}

void TREXBox::addBox(IBox *b)
{
    boxes.push_back(b);
}