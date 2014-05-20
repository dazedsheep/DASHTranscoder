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
#include "VMHDBox.h"

VMHDBox::VMHDBox(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->buffer = buffer;
    if(header != NULL) memcpy(&this->box.fullbox.header, header, sizeof(box_t));
    container = false;
    this->ctx = ctx;
}

VMHDBox::~VMHDBox()
{
}

int32_t VMHDBox::parse()
{
    box.fullbox.version = sys_rb8(buffer);
    box.fullbox.flags[0] = sys_rb8(buffer);
    box.fullbox.flags[1] = sys_rb8(buffer);
    box.fullbox.flags[2] = sys_rb8(buffer);
    
    sys_rb16(buffer);
    sys_rb16(buffer);
    sys_rb16(buffer);
    return 0;
}

bool VMHDBox::isContainer()
{
    return false;
}

uint32_t VMHDBox::getBoxType()
{
    return box.fullbox.header.type;
}

std::vector<IBox*> &VMHDBox::getIncludedBoxes()
{
    return boxes;
}

void VMHDBox::printDebugInformation(bool callIncludedBoxes)
{
    mde_log(MDE_LOG_DEBUG, "vmhd:... written\n");
}

uint32_t VMHDBox::estimateSize(dictionary *inputParams)
{
    uint32_t size;
    size = FULLBOX_HEADER_SIZE + sizeof(uint8_t) * 8;
    return size;
}

chunk *VMHDBox::dumpBox(dictionary *inputParams)
{
    uint32_t size,creationTime, modificationTime, timescale, duration;
    uint32_t reserved = 0;
    uint16_t dummy16;
    /* compute the size of the box */
    box.fullbox.header.size = (uint64_t) estimateSize(inputParams);
    
    chunk *c = new chunk((uint32_t) box.fullbox.header.size, false);
    
    size = htonl((uint32_t) box.fullbox.header.size);
    box.fullbox.header.type = htonl(VMHD_BOX);
    box.fullbox.version = htonl(0);
    box.fullbox.flags[0] = box.fullbox.flags[1] = 0;
    box.fullbox.flags[2] = 1;
    
    dummy16 = 0;
  
    c->append((uint8_t*) &size, sizeof(uint32_t));
    c->append((uint8_t*) &box.fullbox.header.type, sizeof(uint32_t));
    c->append((uint8_t*) &box.fullbox.version, sizeof(box.fullbox.version));
    c->append((uint8_t*) &box.fullbox.flags[0], sizeof(uint8_t));
    c->append((uint8_t*) &box.fullbox.flags[1], sizeof(uint8_t));
    c->append((uint8_t*) &box.fullbox.flags[2], sizeof(uint8_t));
  
    c->append((uint8_t*) &dummy16, sizeof(uint16_t));
    c->append((uint8_t*) &dummy16, sizeof(uint16_t));
    c->append((uint8_t*) &dummy16, sizeof(uint16_t));
    c->append((uint8_t*) &dummy16, sizeof(uint16_t));
    
    return c;
}