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

#include "STBLBox.h"

STBLBox::STBLBox(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->buffer = buffer;
    if(header != NULL) memcpy(&this->box.header, header, sizeof(box_t));
    container = true;
    this->ctx = ctx;
}

STBLBox::~STBLBox()
{
    uint32_t i;
    
    for(i = 0; i < boxes.size(); i++)
    {
        if(boxes.at(i) != NULL) delete boxes.at(i);
    }
}

int32_t STBLBox::parse()
{
    boxReader reader;
    mde_log(MDE_LOG_DEBUG, "isom: Parsing STBL box.\n");
    /* these are mandatory boxes, and they should be there */
    uint32_t fboxes[] = { STSD_BOX, STTS_BOX, STCO_BOX, STSC_BOX, STSZ_BOX };
    boxes = reader.parseBoxes(ctx, buffer, fboxes, sizeofarray(fboxes));
    mde_log(MDE_LOG_DEBUG, "isom: Done parsing STBL box (boxes: %d).\n", boxes.size());
    return 0;
}

bool STBLBox::isContainer()
{
    return container;
}

uint32_t STBLBox::getBoxType()
{
    return box.header.type;
}

std::vector<IBox*> &STBLBox::getIncludedBoxes()
{
    return boxes;
}

uint32_t STBLBox::estimateSize(dictionary *inputParams)
{
    uint32_t size, i;
    size = BOX_HEADER_SIZE;
    for(i = 0; i < boxes.size(); i++) size += boxes.at(i)->estimateSize(inputParams);
    return size;
}

chunk *STBLBox::dumpBox(dictionary *inputParams)
{
    uint32_t size;
    
    size = estimateSize(inputParams);
    chunk *c = new chunk(size, false);
    box.header.type = htonl(STBL_BOX);
    size = htonl(size);
    c->append((uint8_t*) &size, sizeof(size));
    c->append((uint8_t*) &box.header.type, sizeof(uint32_t));
    for (uint32_t i = 0; i < boxes.size(); i++) c->append(boxes.at(i)->dumpBox(inputParams));
    return c;
}

void STBLBox::printDebugInformation(bool callIncludedBoxes)
{
    uint32_t i;
    if(callIncludedBoxes)
        for(i = 0; i < boxes.size(); i++) boxes.at(i)->printDebugInformation(callIncludedBoxes);
}
void STBLBox::addBox(IBox *b)
{
    boxes.push_back(b);
}