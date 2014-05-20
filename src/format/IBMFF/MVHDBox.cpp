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
#include "MVHDBox.h"

MVHDBox::MVHDBox(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->buffer = buffer;
    if(header != NULL) memcpy(&this->box.fullbox.header, header, sizeof(box_t));
    container = false;
    this->ctx = ctx;
}

MVHDBox::~MVHDBox()
{
}

int32_t MVHDBox::parse()
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
        box.timescale = sys_rb32(buffer);
        box.duration = sys_rb64(buffer);
    }else{
        box.creationTime = (uint64_t) sys_rb32(buffer);
        box.modificationTime = (uint64_t) sys_rb32(buffer);
        box.timescale = sys_rb32(buffer);
        box.duration = (uint64_t) sys_rb32(buffer);
    }
    
    box.rate = (int32_t) sys_rb32(buffer);
    box.volume = (int16_t) sys_rb16(buffer);
    
    for(i = 0; i < 10; i++) sys_rb8(buffer);
    
    for(i = 0; i < sizeofarray(box.matrix); i++)
    {
        box.matrix[i] = (int32_t) sys_rb32(buffer);
    }
    
    for(i = 0; i < 6; i++) sys_rb32(buffer);
    
    box.nextTrackId = sys_rb32(buffer);

    /* create a new stream (track) and add some info for the demuxer ... */
    ctx->newStream();
    ctx->getStream()->addDict(MKSTRING(mvhd.creationTime), box.creationTime);
    ctx->getStream()->addDict(MKSTRING(mvhd.modificationTime), box.modificationTime);
    ctx->getStream()->addDict(MKSTRING(mvhd.duration), box.duration);
    ctx->getStream()->addDict(MKSTRING(mvhd.timescale), box.timescale);
    ctx->getStream()->addDict(MKSTRING(mvhd.rate), box.rate);
    ctx->getStream()->addDict(MKSTRING(mvhd.volume), box.volume);
    ctx->getStream()->addDict(MKSTRING(mvhd.nextTrackId), box.nextTrackId);
    
    return 0;
}

bool MVHDBox::isContainer()
{
    return false;
}

uint32_t MVHDBox::getBoxType()
{
    return box.fullbox.header.type;
}

std::vector<IBox*> &MVHDBox::getIncludedBoxes()
{
    return boxes;
}

void MVHDBox::printDebugInformation(bool callIncludedBoxes)
{

}

uint32_t MVHDBox::estimateSize(dictionary *inputParams)
{
    uint32_t size;
    size = FULLBOX_HEADER_SIZE + sizeof(uint32_t) * 4 + sizeof(box.rate) + sizeof(box.volume) + sizeof(box.nextTrackId) + sizeof(uint32_t) * 6 + sizeof(uint8_t) * 10 + sizeof(int32_t) * sizeofarray(unityMatrix);
    return size;
}

chunk *MVHDBox::dumpBox(dictionary *inputParams)
{
    uint32_t size,creationTime, modificationTime, timescale, duration, nextTrackId, i;
    uint32_t reserved = 0;
    uint16_t res16 = 0;
    uint8_t res8 = 0;
    /* compute the size of the box */
    box.fullbox.header.size = (uint64_t)estimateSize(inputParams);
    
    chunk *c = new chunk((uint32_t)box.fullbox.header.size, false);
    
    size = htonl((uint32_t)box.fullbox.header.size);
    box.fullbox.header.type = htonl(MVHD_BOX);
    box.fullbox.version = htonl(0);
    box.fullbox.flags[0] = box.fullbox.flags[1] = box.fullbox.flags[2] = 0;
    
    inputParams->getDict(MKSTRING(mvhd.creationTime), &box.creationTime);
    inputParams->getDict(MKSTRING(mvhd.modificationTime), &box.modificationTime);
    inputParams->getDict(MKSTRING(mvhd.duration), &box.duration);
    inputParams->getDict(MKSTRING(mvhd.timescale), &box.timescale);
    inputParams->getDict(MKSTRING(mvhd.rate), &box.rate);
    inputParams->getDict(MKSTRING(mvhd.volume), &box.volume);
    inputParams->getDict(MKSTRING(mvhd.nextTrackId), &box.nextTrackId);
    
    creationTime = htonl((uint32_t) box.creationTime);
    modificationTime = htonl((uint32_t) box.modificationTime);
    duration = htonl((uint32_t) box.duration);
    timescale = htonl(box.timescale);
    box.rate = htonl(box.rate);
    box.volume = htons(box.volume);
    box.nextTrackId = htonl(box.nextTrackId);
    
    c->append((uint8_t*) &size, sizeof(uint32_t));
    c->append((uint8_t*) &box.fullbox.header.type, sizeof(uint32_t));
    c->append((uint8_t *) &box.fullbox.version, sizeof(box.fullbox.version));
    c->append((uint8_t*) &box.fullbox.flags[0], sizeof(uint8_t));
    c->append((uint8_t*) &box.fullbox.flags[1], sizeof(uint8_t));
    c->append((uint8_t*) &box.fullbox.flags[2], sizeof(uint8_t));
    c->append((uint8_t*) &creationTime, sizeof(creationTime));
    c->append((uint8_t*) &modificationTime, sizeof(modificationTime));
    c->append((uint8_t*) &timescale, sizeof(timescale));
    c->append((uint8_t*) &duration, sizeof(duration));
    c->append((uint8_t*) &box.rate, sizeof(box.rate));
    c->append((uint8_t*) &box.volume, sizeof(box.volume));

    for(i = 0; i < 10; i++) c->append((uint8_t*) &res8, sizeof(res8));
    
    for(i = 0; i < sizeofarray(unityMatrix); i++)
    {
        unityMatrix[i] = htonl(unityMatrix[i]);
        c->append((uint8_t *) &unityMatrix[i], sizeof(int32_t));
    }
    
    for(i = 0; i < 6; i++) c->append((uint8_t*) &reserved, sizeof(reserved));
   
    c->append((uint8_t*) &box.nextTrackId, sizeof(box.nextTrackId));
    return c;

}