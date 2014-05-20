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
#include "MINFBox.h"


MINFBox::MINFBox(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->init(ctx, buffer, header);
    container = true;
}

void MINFBox::init(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->buffer = buffer;
    if(header != NULL) memcpy(&this->box.header, header, sizeof(box_t));
    container = true;
    this->ctx = ctx;
}

MINFBox::~MINFBox()
{
    uint32_t i;
    for(i = 0; i < boxes.size(); i++)
    {
        delete boxes.at(i);
    }
}

int32_t MINFBox::parse()
{
    boxReader reader;
    uint32_t fboxes[] = { DINF_BOX, STBL_BOX };
    mde_log(MDE_LOG_DEBUG, "isom: parsing MINF box (size: %d bytes).\n", box.header.size);
    boxes = reader.parseBoxes(ctx, buffer, fboxes, sizeofarray(fboxes));
    mde_log(MDE_LOG_DEBUG, "isom: MINF box, found %d inner boxes.\n", boxes.size());
    return 0;
}

bool MINFBox::isContainer()
{
    return container;
}

uint32_t MINFBox::getBoxType()
{
    return box.header.type;
}

std::vector<IBox*> &MINFBox::getIncludedBoxes()
{
    return boxes;
}

void MINFBox::addBox(IBox *b)
{
    boxes.push_back(b);
}

uint32_t MINFBox::estimateSize(dictionary *inputParams)
{
    uint32_t size = BOX_HEADER_SIZE, i;
    for(i = 0; i < boxes.size(); i++) size += boxes.at(i)->estimateSize(inputParams);
    return size;
}

chunk *MINFBox::dumpBox(dictionary *inputParams)
{
    uint32_t i, size;
    /* compute the size of the box */
    box.header.size = (uint64_t)estimateSize(inputParams);
    
    chunk *c = new chunk((uint32_t)box.header.size, false);
    size = htonl((uint64_t)box.header.size);
    box.header.type = htonl(MINF_BOX);
    c->append((uint8_t*) &size, sizeof(uint32_t));
    c->append((uint8_t*) &box.header.type, sizeof(uint32_t));
    
    for(i = 0; i < boxes.size(); i++) c->append(boxes.at(i)->dumpBox(inputParams));
    return c;
}

void MINFBox::printDebugInformation(bool callIncludedBoxes)
{
    uint32_t i;
    if(callIncludedBoxes)
        for(i = 0; i < boxes.size(); i++) boxes.at(i)->printDebugInformation(callIncludedBoxes);
}