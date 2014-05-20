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
#include "TKHDBox.h"

TKHDBox::TKHDBox(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->buffer = buffer;
    if(header != NULL) memcpy(&this->box.fullbox.header, header, sizeof(box_t));
    container = false;
    this->ctx = ctx;
}

TKHDBox::~TKHDBox()
{
}

int32_t TKHDBox::parse()
{
    uint32_t i;
    
    box.fullbox.version = sys_rb8(buffer);
    box.fullbox.flags[0] = sys_rb8(buffer);
    box.fullbox.flags[1] = sys_rb8(buffer);
    box.fullbox.flags[2] = sys_rb8(buffer);
    
    if(box.fullbox.version == 1)
    {
        box.creationTime = sys_rb64(buffer);
        box.modificationTime = sys_rb64(buffer);
        box.trackId = sys_rb32(buffer);
        box.reserved = sys_rb32(buffer);
        box.duration = sys_rb64(buffer);
    }else{
        box.creationTime = (uint64_t) sys_rb32(buffer);
        box.modificationTime = (uint64_t) sys_rb32(buffer);
        box.trackId = (uint64_t) sys_rb32(buffer);
        box.reserved = (uint64_t) sys_rb32(buffer);
        box.duration = (uint64_t) sys_rb32(buffer);
    }
    
    box.res[0] = sys_rb32(buffer);
    box.res[1] = sys_rb32(buffer);
    box.layer = (int16_t) sys_rb16(buffer);
    box.alternate_group = (int16_t) sys_rb16(buffer);
    box.volume = (int16_t) sys_rb16(buffer);
    box._reserved = (uint16_t) sys_rb16(buffer);
    
    for(i = 0; i < sizeofarray(box.matrix); i++)
    {
        box.matrix[i] = (int32_t) sys_rb32(buffer);
    }
    
    box.width = (sys_rb16(buffer) << 16) | sys_rb16(buffer);
    box.height = (sys_rb16(buffer) << 16)| sys_rb16(buffer);
    
    /* we already created the stream if we see the mvhd */
    ctx->getStream()->addDict(MKSTRING(tkhd.trackId), box.trackId);
    ctx->getStream()->addDict(MKSTRING(tkhd.creationTime), box.creationTime);
    ctx->getStream()->addDict(MKSTRING(tkhd.modificationTime), box.modificationTime);
    ctx->getStream()->addDict(MKSTRING(tkhd.duration), box.duration);
    ctx->getStream()->addDict(MKSTRING(tkhd.layer), box.layer);
    ctx->getStream()->addDict(MKSTRING(tkhd.alternate_group), box.alternate_group);
    ctx->getStream()->addDict(MKSTRING(tkhd.volume), box.volume);
    ctx->getStream()->addDict(MKSTRING(tkhd.width), box.width);
    ctx->getStream()->addDict(MKSTRING(tkhd.height), box.height);
    
    return 0;
}

bool TKHDBox::isContainer()
{
    return container;
}

uint32_t TKHDBox::getBoxType()
{
    return box.fullbox.header.type;
}

std::vector<IBox*> &TKHDBox::getIncludedBoxes()
{
    return boxes;
}

uint32_t TKHDBox::estimateSize(dictionary *inputParams)
{
    return FULLBOX_HEADER_SIZE + sizeof(uint32_t) * 5 + sizeof(uint32_t) * 2 + sizeof(box.layer) + sizeof(box.alternate_group) + sizeof(box.volume) + sizeof(box._reserved) + sizeofarray(unityMatrix) * sizeof(uint32_t) + sizeof(box.width) + sizeof(box.height);
}

chunk *TKHDBox::dumpBox(dictionary *inputParams)
{
    uint32_t size,creationTime, modificationTime, duration, trackId, i;
    uint32_t reserved = 0, written=0;
    uint16_t res16 = 0;
    /* compute the size of the box */
    box.fullbox.header.size = (uint64_t)estimateSize(inputParams);
    chunk *c = new chunk((uint32_t)box.fullbox.header.size, false);
    
    size = htonl((uint32_t)box.fullbox.header.size);
    box.fullbox.header.type = htonl(TKHD_BOX);
    box.fullbox.version = htonl(0);
    box.fullbox.flags[0] = box.fullbox.flags[1] = 0;
    
    inputParams->getDict(MKSTRING(tkhd.trackId), &box.trackId);
    inputParams->getDict(MKSTRING(tkhd.creationTime), &box.creationTime);
    inputParams->getDict(MKSTRING(tkhd.modificationTime), &box.modificationTime);
    inputParams->getDict(MKSTRING(tkhd.duration), &box.duration);
    inputParams->getDict(MKSTRING(tkhd.alternate_group), &box.alternate_group);
    inputParams->getDict(MKSTRING(tkhd.volume), &box.volume);
    inputParams->getDict(MKSTRING(tkhd.width), &box.width);
    inputParams->getDict(MKSTRING(tkhd.height), &box.height);
    inputParams->getDict(MKSTRING(tkhd.hintFlag), &box.fullbox.flags[2]);
    creationTime = htonl((uint32_t) box.creationTime);
    modificationTime = htonl((uint32_t) box.modificationTime);
    duration = htonl((uint32_t) box.duration);
    trackId = htonl((uint32_t) box.trackId);
    box.alternate_group = htons(box.alternate_group);
    box.volume = htons(box.volume);
    box.layer = htons(box.layer);
    box.width = htons(box.width >> 16) << 16  | htons((uint16_t)box.width);
    box.height = htons(box.height >> 16) << 16 | htons((uint16_t)box.height);
    
    written +=c->append((uint8_t*) &size, sizeof(uint32_t));
    written +=c->append((uint8_t*) &box.fullbox.header.type, sizeof(uint32_t));
    written +=c->append((uint8_t*) &box.fullbox.version, sizeof(box.fullbox.version));
    written +=c->append((uint8_t*) &box.fullbox.flags[0], sizeof(uint8_t));
    written +=c->append((uint8_t*) &box.fullbox.flags[1], sizeof(uint8_t));
    written +=c->append((uint8_t*) &box.fullbox.flags[2], sizeof(uint8_t));
    written +=c->append((uint8_t*) &creationTime, sizeof(creationTime));
    written +=c->append((uint8_t*) &modificationTime, sizeof(modificationTime));
    written +=c->append((uint8_t*) &trackId, sizeof(trackId));
    written +=c->append((uint8_t*) &reserved, sizeof(reserved));
    written +=c->append((uint8_t*) &duration, sizeof(duration));
    written +=c->append((uint8_t*) &reserved, sizeof(reserved));
    written +=c->append((uint8_t*) &reserved, sizeof(reserved));
    written +=c->append((uint8_t*) &box.layer, sizeof(box.layer));
    written +=c->append((uint8_t*) &box.alternate_group, sizeof(box.alternate_group));
    written +=c->append((uint8_t*) &box.volume, sizeof(box.volume));
    written +=c->append((uint8_t*) &res16, sizeof(res16));
    for(i = 0; i < sizeofarray(unityMatrix); i++)
    {
        unityMatrix[i] = htonl(unityMatrix[i]);
        written +=c->append((uint8_t *) &unityMatrix[i], sizeof(int32_t));
    }
    c->append((uint8_t*) &box.width, sizeof(box.width));
    c->append((uint8_t*) &box.height, sizeof(box.height));
    return c;
}

void TKHDBox::printDebugInformation(bool callIncludedBoxes)
{
    uint32_t i;
    mde_log(MDE_LOG_DEBUG, "isom: TKHD debug info: found track with track id: %d, width: %d.%d, height: %d.%d, creation time: %llu, modification time: %llu, duration: %llu.\n", box.trackId, box.width >> 16, box.width & 0xFFFF, box.height >> 16, box.height & 0xFFFF, box.creationTime, box.modificationTime, box.duration);

    if(callIncludedBoxes)
        for(i = 0; i < boxes.size(); i++) boxes.at(i)->printDebugInformation(callIncludedBoxes);
}