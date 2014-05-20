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
#include "SIDXBox.h"

SIDXBox::SIDXBox(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->buffer = buffer;
    if(header != NULL) memcpy(&this->box.fullbox.header, header, sizeof(box_t));
    this->ctx = ctx;
    box.rc = NULL;
}

SIDXBox::~SIDXBox()
{
    if(box.rc != NULL) free(box.rc);
}

int32_t SIDXBox::parse()
{
        container = false;
}

bool SIDXBox::isContainer()
{
    return false;
}

uint32_t SIDXBox::getBoxType()
{
    return box.fullbox.header.type;
}

std::vector<IBox*> &SIDXBox::getIncludedBoxes()
{
    return boxes;
}

uint32_t SIDXBox::estimateSize(dictionary *inputParams)
{
    uint16_t rcs;
    inputParams->getDict(STRING("sidx.reference_count"), &rcs);
    return FULLBOX_HEADER_SIZE + sizeof(box.refId) + sizeof(box.reference_count) + sizeof(box.timescale) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(box.reserved) + rcs * (sizeof(refcount_t) - 12);
}

/* dump the full box inclusive header and all included boxes */
chunk *SIDXBox::dumpBox(dictionary *inputParams)
{
    uint32_t size, i, earliestPresentationTime, firstOffset;
    uint16_t rcs;
    refcount_t refcount;
    inputParams->getDict(STRING("sidx.refId"), &box.refId);
    inputParams->getDict(STRING("sidx.timescale"), &box.timescale);
    inputParams->getDict(STRING("sidx.earliestPresentationTime"), &box.earliestPresentationTime);
    inputParams->getDict(STRING("sidx.firstOffset"), &box.firstOffset);
    inputParams->getDict(STRING("sidx.reference_count"), &box.reference_count);
    if(box.reference_count > 0)
    {
        /* normally we should read all of the reference counts, but for now we stick to only a single reference */
        inputParams->getDict(MKSTRING(sidx.reference_counts[0].referenceSize), &refcount.referenceSize);
        inputParams->getDict(MKSTRING(sidx.reference_counts[0].referenceType), &refcount.referenceType);
        inputParams->getDict(MKSTRING(sidx.reference_counts[0].subsegmentDuration), &refcount.subsegmentDuration);
        inputParams->getDict(MKSTRING(sidx.reference_counts[0].startsWithSAP), &refcount.startsWithSAP);
        inputParams->getDict(MKSTRING(sidx.reference_counts[0].SAPType), &refcount.SAPType);
        inputParams->getDict(MKSTRING(sidx.reference_counts[0].SAPDeltaTime), &refcount.SAPDeltaTime);
    }
        
    box.fullbox.header.size = (uint64_t)estimateSize(inputParams);
    chunk *c = new chunk((uint32_t)box.fullbox.header.size, false);
    
    size = htonl((uint32_t)box.fullbox.header.size);
    box.fullbox.header.type = htonl(SIDX_BOX);
    box.fullbox.version = 0;
    box.fullbox.flags[0] = box.fullbox.flags[1] = box.fullbox.flags[2] = 0;
    box.refId = htonl(box.refId);
    box.timescale = htonl(box.timescale);
    earliestPresentationTime = htonl((uint32_t)box.earliestPresentationTime);
    firstOffset = htonl((uint32_t)box.firstOffset);
    rcs = box.reference_count;
    box.reference_count = htons(box.reference_count);
    box.reserved = 0;
    
    c->append((uint8_t *) &size, sizeof(size));
    c->append((uint8_t *) &box.fullbox.header.type, sizeof(box.fullbox.header.type));
    c->append((uint8_t *) &box.fullbox.version, sizeof(box.fullbox.version));
    c->append((uint8_t *) box.fullbox.flags, 3);
    c->append((uint8_t *) &box.refId, sizeof(box.refId));
    c->append((uint8_t *) &box.timescale, sizeof(box.timescale));
    c->append((uint8_t *) &earliestPresentationTime, sizeof(earliestPresentationTime));
    c->append((uint8_t *) &firstOffset, sizeof(firstOffset));
    c->append((uint8_t *) &box.reserved, sizeof(box.reserved));
    c->append((uint8_t *) &box.reference_count, sizeof(box.reference_count));
    // for(uint32_t i = 0; i < rcs; i++)
    //{
        refcount.referenceSize = htonl(refcount.referenceSize);
        refcount.subsegmentDuration = htonl(refcount.subsegmentDuration);
        refcount.SAPType = htonl((refcount.SAPType ) | refcount.startsWithSAP << 31);
        refcount.SAPDeltaTime = htonl(refcount.SAPDeltaTime);
        c->append((uint8_t *) &refcount.referenceSize, sizeof(refcount.referenceSize));
        c->append((uint8_t *) &refcount.subsegmentDuration, sizeof(refcount.subsegmentDuration));
        c->append((uint8_t *) &refcount.SAPType, sizeof(refcount.SAPType));
    // c->append((uint8_t *) &refcount.SAPDeltaTime, sizeof(refcount.SAPDeltaTime));
    //}
    
    return c;
}