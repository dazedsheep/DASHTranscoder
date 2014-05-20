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

#include "STSDBox.h"


STSDBox::STSDBox(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->init(ctx, buffer, header);
}

void STSDBox::init(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->buffer = buffer;
    
    if(header!=NULL) memcpy(&this->box.fullbox.header, header, sizeof(box_t));
    this->ctx = ctx;
    /* container for sample entries ... */
    container = true;
}

STSDBox::~STSDBox()
{
    uint32_t i;
    
    for(i = 0; i < boxes.size(); i++)
    {
        delete boxes.at(i);
    }
}

int32_t STSDBox::parse()
{
    uint32_t i;
    boxReader reader;
    // the handler type decides what we are going to parse ...
    box.fullbox.version = sys_rb8(buffer);
    box.fullbox.flags[0] = sys_rb8(buffer);
    box.fullbox.flags[1] = sys_rb8(buffer);
    box.fullbox.flags[2] = sys_rb8(buffer);
    box.entry_count = sys_rb32(buffer);
    mde_log(MDE_LOG_DEBUG, "isom: stsd box found %d sample entries\n", box.entry_count);
    for(i = 0; i < box.entry_count; i++)
    {
         /* we won't parse the obvious parts of this sample entry, let the factory and reader do the work */
         boxes.push_back(reader.readBox(ctx, this->buffer));
    }
    mde_log(MDE_LOG_DEBUG, "isom: stsd box found %d box(es) as sample entries\n", boxes.size());
    return 0;
}

bool STSDBox::isContainer()
{
    return container;
}

uint32_t STSDBox::getBoxType()
{
    return box.fullbox.header.type;
}

std::vector<IBox*> &STSDBox::getIncludedBoxes()
{
    return boxes;
}

uint32_t STSDBox::estimateSize(dictionary *inputParams)
{
    uint32_t i, size;
    size = FULLBOX_HEADER_SIZE + sizeof(box.entry_count);
    for(i = 0; i < boxes.size(); i++) size += boxes.at(i)->estimateSize(inputParams);
    return size;
}

void STSDBox::printDebugInformation(bool callIncludedBoxes)
{
    uint32_t i;
    mde_log(MDE_LOG_DEBUG, "isom: STSD box debug info: (version: %d, sample entries: %d).\n",box.fullbox.version, box.entry_count);
    if(callIncludedBoxes)
        for(i = 0; i < boxes.size(); i++) boxes.at(i)->printDebugInformation(callIncludedBoxes);
}

chunk *STSDBox::dumpBox(dictionary *inputParams)
{
    uint32_t i, size;
    box.fullbox.header.type = htonl(STSD_BOX);
    box.fullbox.version = 0;
    box.fullbox.flags[0] = 0;
    box.fullbox.flags[1] = 0;
    box.fullbox.flags[2] = 0;
    box.entry_count = htonl(boxes.size());
    size = estimateSize(inputParams);
    chunk *c = new chunk(size, false);
    size = htonl(size);
    c->append((uint8_t *) &size, sizeof(size));
    c->append((uint8_t *) &box.fullbox.header.type, sizeof(box.fullbox.header.type));
    c->append((uint8_t *) &box.fullbox.version, sizeof(box.fullbox.version));
    c->append((uint8_t *) box.fullbox.flags, sizeofarray(box.fullbox.flags) * sizeof(uint8_t));
    c->append((uint8_t *) &box.entry_count, sizeof(box.entry_count));
    
    for(i = 0; i < boxes.size(); i++) c->append(boxes.at(i)->dumpBox(inputParams));

    return c;
}

void STSDBox::addBox(IBox *b)
{
    boxes.push_back(b);
}