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
#include "TRAKBox.h"

TRAKBox::TRAKBox(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->buffer = buffer;
    if(header != NULL) memcpy(&this->box.header, header, sizeof(box_t));
    container = true;
    this->ctx = ctx;
}

TRAKBox::~TRAKBox()
{
    uint32_t i;
    for(i = 0; i < boxes.size(); i++)
    {
        delete boxes.at(i);
    }
}

int32_t TRAKBox::parse()
{
    uint32_t i;
    boxReader reader;
    uint32_t fboxes[] = {TKHD_BOX, MDIA_BOX};
    mde_log(MDE_LOG_DEBUG, "isom: going to parse inner boxes of TRAK box.\n");
    boxes = reader.parseBoxes(ctx, buffer, (uint32_t *)fboxes, sizeofarray(fboxes));
    mde_log(MDE_LOG_DEBUG, "isom: found %d inner box(es) of TRAK box.\n", boxes.size());
    return 0;
}

bool TRAKBox::isContainer()
{
    return container;
}

uint32_t TRAKBox::getBoxType()
{
    return box.header.type;
}

std::vector<IBox*> &TRAKBox::getIncludedBoxes()
{
    return boxes;
}

uint32_t TRAKBox::estimateSize(dictionary *inputParams)
{
    uint32_t size, i;
    size = BOX_HEADER_SIZE;
    for(i = 0; i < boxes.size(); i++) size += boxes.at(i)->estimateSize(inputParams);
    return size;
}

void TRAKBox::addBox(IBox *b)
{
    boxes.push_back(b);
}

chunk *TRAKBox::dumpBox(dictionary *inputParams)
{
    uint32_t i, size;
    uint32_t written=0;
    /* compute the size of the box */
    box.header.size = (uint64_t)estimateSize(inputParams);
    mde_log(MDE_LOG_DEBUG, "%s: allocated %d bytes.\n", __FILE__, (uint32_t)box.header.size);
    chunk *c = new chunk((uint32_t)box.header.size, false);
    
    size = htonl((uint32_t)box.header.size);
    box.header.type = htonl(TRAK_BOX);
 
    written += c->append((uint8_t*) &size, sizeof(uint32_t));
    written +=c->append((uint8_t*) &box.header.type, sizeof(uint32_t));
    
    for( i = 0; i < boxes.size(); i++) written += c->append(boxes.at(i)->dumpBox(inputParams));
    
    mde_log(MDE_LOG_DEBUG, "%s: wrote %d bytes.\n", __FILE__, written);
    return c;
}

void TRAKBox::printDebugInformation(bool callIncludedBoxes)
{
    uint32_t i;
    if(callIncludedBoxes)
        for(i = 0; i < boxes.size(); i++) boxes.at(i)->printDebugInformation(callIncludedBoxes);
}