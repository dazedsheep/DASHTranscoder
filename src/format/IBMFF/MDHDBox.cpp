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
 
#include "MDHDBox.h"

MDHDBox::MDHDBox(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->buffer = buffer;
    if(header != NULL) memcpy(&this->box.fullbox.header, header, sizeof(box_t));
    container = false;
    this->ctx = ctx;
}

MDHDBox::~MDHDBox()
{
}

int32_t MDHDBox::parse()
{
    box.fullbox.version = sys_rb8(buffer);
    box.fullbox.flags[0] = sys_rb8(buffer);
    box.fullbox.flags[1] = sys_rb8(buffer);
    box.fullbox.flags[2] = sys_rb8(buffer);
    
    box.creationTime = (uint64_t) sys_rb32(buffer);
    box.modificationTime = (uint64_t) sys_rb32(buffer);
    box.timescale = sys_rb32(buffer);
    box.duration = (uint64_t) sys_rb32(buffer);
    box.lang = sys_rb16(buffer);
    box.pre_defined = sys_rb8(buffer);
    
    ctx->getStream()->addDict(MKSTRING(mdhd.creationTime), box.creationTime);
    ctx->getStream()->addDict(MKSTRING(mdhd.modificationTime), box.modificationTime);
    ctx->getStream()->addDict(MKSTRING(mdhd.timescale), box.timescale);
    ctx->getStream()->addDict(MKSTRING(mdhd.duration), box.duration);
    ctx->getStream()->addDict(MKSTRING(mdhd.lang), box.lang);
    /* set the global timescale of this track */
    ctx->getStream()->addDict<uint32_t>(MKSTRING(track.timescale), (uint32_t) box.timescale);
    
    return 0;
}

bool MDHDBox::isContainer()
{
    return false;
}

uint32_t MDHDBox::getBoxType()
{
    return box.fullbox.header.type;
}

std::vector<IBox*> &MDHDBox::getIncludedBoxes()
{
    return boxes;
}

void MDHDBox::printDebugInformation(bool callIncludedBoxes)
{
    mde_log(MDE_LOG_DEBUG, "mdhd: timescale %d\n", (uint32_t)box.timescale);
}

uint32_t MDHDBox::estimateSize(dictionary *inputParams)
{
    uint32_t size;
    size = FULLBOX_HEADER_SIZE + sizeof(uint32_t) * 4 + sizeof(box.lang) + sizeof(box.pre_defined);
    return size;
}

chunk *MDHDBox::dumpBox(dictionary *inputParams)
{
    uint32_t size,creationTime, modificationTime, timescale, duration;
    uint32_t reserved = 0;
    /* compute the size of the box */
    box.fullbox.header.size = (uint64_t) estimateSize(inputParams);
    
    chunk *c = new chunk((uint32_t) box.fullbox.header.size, false);
    
    size = htonl((uint32_t) box.fullbox.header.size);
    box.fullbox.header.type = htonl(MDHD_BOX);
    box.fullbox.version = htonl(0);
    box.fullbox.flags[0] = box.fullbox.flags[1] = box.fullbox.flags[2] = 0;
    
    inputParams->getDict(STRING("mdhd.creationTime"), &box.creationTime);
    inputParams->getDict(STRING("mdhd.modificationTime"), &box.modificationTime);
    inputParams->getDict(STRING("mdhd.timescale"), &box.timescale);
    inputParams->getDict<uint64_t>(STRING("mdhd.duration"), &box.duration);
    inputParams->getDict(STRING("mdhd.langCode"), &box.lang);
    box.lang = htons(box.lang);
    creationTime = htonl((uint32_t) box.creationTime);
    modificationTime = htonl((uint32_t) box.modificationTime);
    timescale = htonl(box.timescale);
    duration = htonl((uint32_t) box.duration);
    box.pre_defined = 0;
    c->append((uint8_t*) &size, sizeof(uint32_t));
    c->append((uint8_t*) &box.fullbox.header.type, sizeof(uint32_t));
    c->append((uint8_t*) &box.fullbox.version, sizeof(box.fullbox.version));
    c->append((uint8_t*) &box.fullbox.flags[0], sizeof(uint8_t));
    c->append((uint8_t*) &box.fullbox.flags[1], sizeof(uint8_t));
    c->append((uint8_t*) &box.fullbox.flags[2], sizeof(uint8_t));
    c->append((uint8_t*) &creationTime, sizeof(creationTime));
    c->append((uint8_t*) &modificationTime, sizeof(modificationTime));
    c->append((uint8_t*) &timescale, sizeof(timescale));
    c->append((uint8_t*) &duration, sizeof(duration));
    c->append((uint8_t*) &box.lang, sizeof(box.lang));
    c->append((uint8_t*) &box.pre_defined, sizeof(box.pre_defined));
    
    return c;
}