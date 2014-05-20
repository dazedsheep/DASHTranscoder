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
#include "STYPBox.h"

STYPBox::STYPBox(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->buffer = buffer;
    if(header != NULL) memcpy(&this->box.header, header, sizeof(box_t));
    this->ctx = ctx;
}

STYPBox::~STYPBox()
{
    if(box.compatible_brands != NULL)
        free(box.compatible_brands);
}

int32_t STYPBox::parse()
{
    mde_log(MDE_LOG_DEBUG, "isom: parsing STYP box\n");
    brandSize = (uint32_t)(box.header.size - box.header.realSize);
    
    if(!brandSize)
        mde_log(MDE_LOG_WARNING, "isom: no compatible brands found in FTYP box.");
    
    uint8_t *brands = (uint8_t *) calloc(sizeof(uint8_t), brandSize);
    
    for(uint32_t i = 0; i < brandSize; i++)
        brands[i] = sys_rb8(buffer);
    
    box.compatible_brands = (uint32_t*) brands;
    
    // that's all we dont have to do anything else because this box does not contain any other boxes
    mde_log(MDE_LOG_DEBUG, "isom: done parsing STYP box, found brands: %s\n",box.compatible_brands);
    
    container = false;
}

bool STYPBox::isContainer()
{
    return false;
}

uint32_t STYPBox::getBoxType()
{
    return box.header.type;
}

std::vector<IBox*> &STYPBox::getIncludedBoxes()
{
    return boxes;
}

uint32_t STYPBox::estimateSize(dictionary *inputParams)
{
    return BOX_HEADER_SIZE + sizeof(box.major_brand) + sizeof(box.minor_version) + 2*sizeof(uint32_t);
}

chunk *STYPBox::dumpBox(dictionary *inputParams)
{
    uint32_t n;
    // calculate the size of this box
    box.header.size = (uint64_t)estimateSize(inputParams);
    box.header.type = htonl(STYP_BOX);
    
    chunk *c = new chunk((uint32_t)box.header.size, false);
    
    inputParams->getDict(STRING("styp.major_brand"), &box.major_brand);
    inputParams->getDict(STRING("styp.minor_version"), &box.minor_version);
    box.minor_version = htonl((uint32_t)box.minor_version);
    /* the major brand should do it for now ... */
    box.compatible_brands = NULL;
    
    n = htonl(box.header.size);
    box.major_brand = htonl(box.major_brand);
    c->append((uint8_t *) &n, sizeof(uint32_t));
    
    c->append((uint8_t*) &box.header.type, sizeof(uint32_t));
    
    c->append((uint8_t*) &box.major_brand, sizeof(uint32_t));
    
    c->append((uint8_t*) &box.minor_version, sizeof(uint32_t));
    
    uint32_t brand1 = htonl(ISOM_MKTAG('m','s','d','h'));
    uint32_t brand2 = htonl(ISOM_MKTAG('m','s','i','x'));
    
    c->append((uint8_t*) &brand1, sizeof(uint32_t));
    c->append((uint8_t*) &brand2, sizeof(uint32_t));
    //if(box.compatible_brands != NULL) c->append((uint8_t*) box.compatible_brands, brandSize);
    
    return c;
}