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

#include "TRUNBox.h"

TRUNBox::TRUNBox(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->buffer = buffer;
    if(header != NULL) memcpy(&this->box.fullbox.header, header, sizeof(box_t));
    container = true;
    this->ctx = ctx;
}

TRUNBox::TRUNBox()
{
}

TRUNBox::~TRUNBox()
{
}

bool TRUNBox::isContainer()
{
    return false;
}

uint32_t TRUNBox::getBoxType()
{
    return box.fullbox.header.type;
}

std::vector<IBox*> &TRUNBox::getIncludedBoxes()
{
    return boxes;
}

uint32_t TRUNBox::estimateSize(dictionary *inputParams)
{
    inputParams->getDict(STRING("trun.sample_count"), &box.sample_count);
    /* the actual size depends on how many information we are putting into the track runs ... */
    return FULLBOX_HEADER_SIZE + sizeof(box.sample_count) + sizeof(box.first_sample_flags) + box.sample_count * sizeof(uint32_t) + sizeof(box.data_offset);
}

/* Before calling dumpBox of TRUN ensure that ctx->getStream(id) has the appropriate list of samples attached to it */
chunk *TRUNBox::dumpBox(dictionary *inputParams)
{
    uint8_t flag1, flag2;
    uint32_t n,trackId, offset, f, size;
    
    inputParams->getDict(STRING("trun.sample_count"), &box.sample_count);    /* depending on the coding and fps */
    inputParams->getDict(STRING("trun.trackId"), &trackId);
    inputParams->getDict(STRING("trun.offset"), &offset);
    
    flag1 = 0x02;
    flag2 = 0x05;
    box.fullbox.header.size = (uint64_t)estimateSize(inputParams);
    chunk *c = new chunk((uint32_t)box.fullbox.header.size, false);
    size = htonl((uint32_t)box.fullbox.header.size);
    box.fullbox.header.type = htonl(TRUN_BOX);
    box.fullbox.version = 0;
    box.fullbox.flags[0] = 0;
    box.fullbox.flags[1] = flag1;
    box.fullbox.flags[2] = flag2;
    n = box.sample_count;
    box.sample_count = htonl(box.sample_count);
    box.data_offset = htonl(offset);
    box.first_sample_flags = 0;
    
    c->append((uint8_t *) &size, sizeof(size));
    c->append((uint8_t *) &box.fullbox.header.type, sizeof(box.fullbox.header.type));
    c->append((uint8_t *) &box.fullbox.version, sizeof(box.fullbox.version));
    c->append((uint8_t *) box.fullbox.flags, 3);
    c->append((uint8_t *) &box.sample_count, sizeof(box.sample_count));
    c->append((uint8_t *) &box.data_offset, sizeof(box.data_offset));
    c->append((uint8_t *) &box.first_sample_flags, sizeof(box.first_sample_flags));
   
    if(ctx->getStream(trackId-1)->getSample(0) == NULL) mde_log(MDE_LOG_ERROR, "%s: no samples attached to the current stream.\n", __FILE__);

    for(uint32_t i = 0; i < n; i++)
    {
        // f = 0;
        //c->append((uint8_t *) &f, sizeof(f)); /* sample duration */
        f = htonl(ctx->getStream(trackId-1)->getSample(i)->size() + sizeof(uint32_t)); /* add the size field to each sample/access unit */
        c->append((uint8_t *) &f, sizeof(f)); /* sample size */
        //f = 0;
        //c->append((uint8_t *) &f, sizeof(f)); /* sample flags */
        //c->append((uint8_t *) &f, sizeof(f)); /* sample composition time flags */
    }
   
    return c;
}