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
 
#include "HDLRBox.h"

HDLRBox::HDLRBox(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->buffer = buffer;
    if(header != NULL) memcpy(&this->box.fullbox.header, header, sizeof(box_t));
    container = false;
    this->ctx = ctx;
}

HDLRBox::~HDLRBox()
{
    if(box.name != NULL) free(box.name);
}

int32_t HDLRBox::parse()
{
    box.fullbox.version = sys_rb8(buffer);
    box.fullbox.flags[0] = sys_rb8(buffer);
    box.fullbox.flags[1] = sys_rb8(buffer);
    box.fullbox.flags[2] = sys_rb8(buffer);
    
    box.pre_defined = sys_rb32(buffer);
    
    /* handler_type:
     * vide : for video streams
     * soun : for audio streams
     * meta : for metadata tracks
     * hint : hint tracks
     */
    
    box.handler_type = sys_rb32(buffer);
    
    sys_rb32(buffer);
    sys_rb32(buffer);
    sys_rb32(buffer);
    
    box.name = sys_rstring(buffer);
    return 0;
}

bool HDLRBox::isContainer()
{
    return false;
}

uint32_t HDLRBox::getBoxType()
{
    return box.fullbox.header.type;
}

std::vector<IBox*> &HDLRBox::getIncludedBoxes()
{
    return boxes;
}

hdlr_t *HDLRBox::getInternalBoxRepresentation()
{
    return &box;
}

void HDLRBox::printDebugInformation(bool callIncludedBoxes)
{
    mde_log(MDE_LOG_DEBUG, "isom: HDLR box debug info: (handler type: %d, handler name: %s).\n", box.handler_type, box.name);
}

uint32_t HDLRBox::estimateSize(dictionary *inputParams)
{
    uint32_t size;
    uint8_t *bname;
    inputParams->getDict(STRING("hdlr.name"), &bname);
    size = FULLBOX_HEADER_SIZE + sizeof(box.pre_defined) + sizeof(box.handler_type) + sizeof(box.reserved) * 3 + sys_strlen(bname);
    return size;
}

chunk *HDLRBox::dumpBox(dictionary *inputParams)
{
    uint32_t n, size;
    uint32_t reserved = 0;
    /* compute the size of the box */
    box.fullbox.header.size = (uint64_t)estimateSize(inputParams);
    
    chunk *c = new chunk((uint32_t) box.fullbox.header.size, false);
    
    size = htonl((uint32_t) box.fullbox.header.size);
    box.fullbox.header.type = htonl(HDLR_BOX);
    box.fullbox.version = htonl(0);
    box.fullbox.flags[0] = box.fullbox.flags[1] = box.fullbox.flags[2] = 0;
    
    inputParams->getDict(STRING("hdlr.handler_type"), &box.handler_type);
    inputParams->getDict(STRING("hdlr.name"), &box.name);
    box.handler_type = htonl(box.handler_type);
    box.pre_defined = 0;
    
    mde_log(MDE_LOG_DEBUG, "%s: hdlr string: %s.\n", __FILE__, box.name);
    
    c->append((uint8_t*) &size, sizeof(uint32_t));
    c->append((uint8_t*) &box.fullbox.header.type, sizeof(uint32_t));
    c->append((uint8_t*) &box.fullbox.version, sizeof(box.fullbox.version));
    c->append((uint8_t*) &box.fullbox.flags[0], sizeof(uint8_t));
    c->append((uint8_t*) &box.fullbox.flags[1], sizeof(uint8_t));
    c->append((uint8_t*) &box.fullbox.flags[2], sizeof(uint8_t));
    c->append((uint8_t*) &box.pre_defined, sizeof(box.pre_defined));
    c->append((uint8_t*) &box.handler_type, sizeof(box.handler_type));
    c->append((uint8_t*) &reserved, sizeof(reserved));
    c->append((uint8_t*) &reserved, sizeof(reserved));
    c->append((uint8_t*) &reserved, sizeof(reserved));
    c->append((uint8_t*) box.name, sys_strlen(box.name));
    box.name = NULL; /* the name will be freed by the dictionary */
    return c;
}
