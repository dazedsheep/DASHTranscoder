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
 
#include "MDIABox.h"

MDIABox::MDIABox(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->buffer = buffer;
    if(header != NULL) memcpy(&this->box.header, header, sizeof(box_t));
    this->ctx = ctx;
    container = true;
}

MDIABox::~MDIABox()
{
    uint32_t i;
    for(i = 0; i < boxes.size(); i++)
    {
        delete boxes.at(i);
    }
}

int32_t MDIABox::parse()
{
    
    uint32_t fboxes[] = { MDHD_BOX, HDLR_BOX, MINF_BOX };
    boxReader reader;
    /* these are mandatory boxes, and they should be there */
    boxes = reader.parseBoxes(ctx, buffer, fboxes, sizeofarray(fboxes));
    mde_log(MDE_LOG_DEBUG, "isom: MDIA box, found %d inner boxes.\n", boxes.size());
    return 0;
}

bool MDIABox::isContainer()
{
    return container;
}

uint32_t MDIABox::getBoxType()
{
    return box.header.type;
}

std::vector<IBox*> &MDIABox::getIncludedBoxes()
{
    return boxes;
}

void MDIABox::addBox(IBox *b)
{
    boxes.push_back(b);
}

uint32_t MDIABox::estimateSize(dictionary *inputParams)
{
    uint32_t size = BOX_HEADER_SIZE, i;
    for(i = 0; i < boxes.size(); i++) size += boxes.at(i)->estimateSize(inputParams);
    return size;
}

chunk *MDIABox::dumpBox(dictionary *inputParams)
{
    uint32_t i, size;
    /* compute the size of the box */
    box.header.size = (uint64_t) estimateSize(inputParams);
    
    chunk *c = new chunk((uint32_t) box.header.size, false);
    size = htonl((uint32_t) box.header.size);
    box.header.type = htonl(MDIA_BOX);
    c->append((uint8_t*) &size, sizeof(uint32_t));
    c->append((uint8_t*) &box.header.type, sizeof(uint32_t));
    
    for(i = 0; i < boxes.size(); i++) c->append(boxes.at(i)->dumpBox(inputParams));
    return c;
}


void MDIABox::printDebugInformation(bool callIncludedBoxes)
{
    uint32_t i;
    if(callIncludedBoxes)
        for(i = 0; i < boxes.size(); i++) boxes.at(i)->printDebugInformation(callIncludedBoxes);
}