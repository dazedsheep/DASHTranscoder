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
#include "FTYPBox.h"

FTYPBox::FTYPBox(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->buffer = buffer;
    if(header != NULL) memcpy(&this->box.header, header, sizeof(box_t));
    this->ctx = ctx;
}

FTYPBox::FTYPBox()
{
    box.header.type = FTYP_BOX;
}

FTYPBox::~FTYPBox()
{
    if(box.compatible_brands != NULL)
        free(box.compatible_brands);
}

int32_t FTYPBox::parse()
{
    mde_log(MDE_LOG_DEBUG, "isom: parsing FTYP box\n");
    brandSize = (uint32_t)(box.header.size - box.header.realSize);
    
    if(!brandSize)
        mde_log(MDE_LOG_WARNING, "isom: no compatible brands found in FTYP box.");
    
    uint8_t *brands = (uint8_t *) calloc(sizeof(uint8_t), brandSize);
    
    for(uint32_t i = 0; i < brandSize; i++)
        brands[i] = sys_rb8(buffer);
    
    box.compatible_brands = (uint32_t*) brands;
    
    // that's all we dont have to do anything else because this box does not contain any other boxes
    mde_log(MDE_LOG_DEBUG, "isom: done parsing FTYP box, found brands: %s\n",box.compatible_brands);
    container = false;
}

bool FTYPBox::isContainer()
{
    return false;
}

uint32_t FTYPBox::getBoxType()
{
    return box.header.type;
}

std::vector<IBox*> &FTYPBox::getIncludedBoxes()
{
    return boxes;
}

chunk *FTYPBox::dumpBox(dictionary *inputParams)
{
    uint32_t n;
    // calculate the size of this box
    box.header.size = sizeof(box.minor_version) + sizeof(box.major_brand) + BOX_HEADER_SIZE;
    box.header.type = htonl(FTYP_BOX);
    
    chunk *c = new chunk(box.header.size, false);
    
    inputParams->getDict(STRING("ftyp.major_brand"), &box.major_brand);
    inputParams->getDict(STRING("ftyp.minor_version"), &box.minor_version);
    box.minor_version = htonl(box.minor_version);
    /* the major brand should do it for now ... */
    box.compatible_brands = NULL;
    
    n = htonl(box.header.size);
    box.major_brand = htonl(box.major_brand);
    
    c->append((uint8_t *) &n, sizeof(uint32_t));
    
    c->append((uint8_t*) &box.header.type, sizeof(uint32_t));
    
    c->append((uint8_t*) &box.major_brand, sizeof(uint32_t));
    
    c->append((uint8_t*) &box.minor_version, sizeof(uint32_t));
    
    //if(box.compatible_brands != NULL) c->append((uint8_t*) box.compatible_brands, brandSize);
    
    return c;
}