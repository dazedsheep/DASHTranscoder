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

#include "AVC1Box.h"


AVC1Box::AVC1Box(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->init(ctx, buffer, header);
}

void AVC1Box::init(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->buffer = buffer;
    if(header != NULL) memcpy(&this->avcSampleEntry.entry.sampleEntry.header, header, sizeof(box_t));
    this->ctx= ctx;
    /* container for sample entries ... */
    container = true;
}

AVC1Box::~AVC1Box()
{
    uint32_t i;
    for(i = 0; i < boxes.size(); i++)
    {
        delete boxes.at(i);
    }
}

int32_t AVC1Box::parse()
{
    uint32_t i, _width, _height;
    boxReader reader;
    uint64_t off = buffer->getOffset();
    mde_log(MDE_LOG_DEBUG, "isom: Parsing AVC sample entry box.\n");
    
    for(i = 0; i < sizeofarray(avcSampleEntry.entry.sampleEntry.reserved); i++) avcSampleEntry.entry.sampleEntry.reserved[i] = sys_rb8(buffer);
    
    avcSampleEntry.entry.sampleEntry.dataRefIdx = sys_rb16(buffer);
    /* now the visual sample entry */
    avcSampleEntry.entry.pre_defined = sys_rb16(buffer);
    avcSampleEntry.entry.reserved = sys_rb16(buffer);
  
    for(i = 0; i < sizeofarray(avcSampleEntry.entry.pre_defined_); i++) avcSampleEntry.entry.pre_defined_[i] = sys_rb32(buffer);

    avcSampleEntry.entry.width = sys_rb16(buffer);
    avcSampleEntry.entry.height = sys_rb16(buffer);
    avcSampleEntry.entry.horizResolution = sys_rb32(buffer);
    avcSampleEntry.entry.vertResolution = sys_rb32(buffer);
    avcSampleEntry.entry._reserved = sys_rb32(buffer);
    avcSampleEntry.entry.frameCount = sys_rb16(buffer);
    
    for(i = 0; i < sizeofarray(avcSampleEntry.entry.compressorname); i++) avcSampleEntry.entry.compressorname[i] = sys_rb8(buffer);

    avcSampleEntry.entry.depth = sys_rb16(buffer);
    avcSampleEntry.entry.predef = (int16_t)sys_rb16(buffer);
    
    ctx->getStream()->addDict(MKSTRING(avcSampleEntry.entry.sampleEntry.dataRefIdx), avcSampleEntry.entry.sampleEntry.dataRefIdx);
    ctx->getStream()->addDict(MKSTRING(avcSampleEntry.entry.width), avcSampleEntry.entry.width);
    ctx->getStream()->addDict(MKSTRING(avcSampleEntry.entry.height), avcSampleEntry.entry.height);
    ctx->getStream()->addDict(MKSTRING(avcSampleEntry.entry.horizResolution), avcSampleEntry.entry.horizResolution);
    ctx->getStream()->addDict(MKSTRING(avcSampleEntry.entry.vertResolution), avcSampleEntry.entry.vertResolution);
    ctx->getStream()->addDict(MKSTRING(avcSampleEntry.entry.frameCount), avcSampleEntry.entry.frameCount);
    ctx->getStream()->addDict(MKSTRING(avcSampleEntry.entry.depth), avcSampleEntry.entry.depth);
    
    _width = (uint32_t) avcSampleEntry.entry.width;
    _height = (uint32_t) avcSampleEntry.entry.height;
    ctx->getStream()->addDict(MKSTRING(video.width), _width);
    ctx->getStream()->addDict(MKSTRING(video.height), _height);
    
    /* maybe we got some additional boxes here, but we will skip them if they are there (at least for now) */
    mde_log(MDE_LOG_DEBUG, "isom: Done parsing AVC sample entry box.\n");
    boxes.push_back(reader.readBox(ctx, buffer, AVCC_BOX));
    
    /* there may be some additional boxes here too, but for now we won't bother reading and parsing them */
    ctx->getStream()->setFourCC(avcSampleEntry.entry.sampleEntry.header.type);
    return 0;
}

bool AVC1Box::isContainer()
{
    return container;
}

uint32_t AVC1Box::getBoxType()
{
    return avcSampleEntry.entry.sampleEntry.header.type;
}

std::vector<IBox*> &AVC1Box::getIncludedBoxes()
{
    return boxes;
}

void AVC1Box::printDebugInformation(bool callIncludedBoxes)
{
    uint32_t i;
    mde_log(MDE_LOG_DEBUG, "isom: AVC sample entry box debug info: (drefIdx: %d, witdh: %d, height: %d, frameCount: %d, depth: %d).\n",avcSampleEntry.entry.sampleEntry.dataRefIdx, avcSampleEntry.entry.width, avcSampleEntry.entry.height, avcSampleEntry.entry.frameCount, avcSampleEntry.entry.depth);
    if(callIncludedBoxes)
        for(i = 0; i < boxes.size(); i++) boxes.at(i)->printDebugInformation(callIncludedBoxes);
}

uint32_t AVC1Box::estimateSize(dictionary *inputParams)
{
    uint32_t i,size;
    size = BOX_HEADER_SIZE + sizeofarray(avcSampleEntry.entry.sampleEntry.reserved) * sizeof(uint8_t) + sizeof(avcSampleEntry.entry.sampleEntry.dataRefIdx) + sizeof(avcSampleEntry.entry.pre_defined) + sizeof(avcSampleEntry.entry.reserved) + sizeofarray(avcSampleEntry.entry.pre_defined_) * sizeof(uint32_t) + sizeof(avcSampleEntry.entry.width) + sizeof(avcSampleEntry.entry.height) + sizeof(avcSampleEntry.entry.horizResolution) + sizeof(avcSampleEntry.entry.vertResolution) + sizeof(avcSampleEntry.entry._reserved) + sizeof(avcSampleEntry.entry.frameCount) + sizeofarray(avcSampleEntry.entry.compressorname)*sizeof(uint8_t) + sizeof(avcSampleEntry.entry.depth) + sizeof(avcSampleEntry.entry.predef);
     for(i = 0; i < boxes.size(); i++) size += boxes.at(i)->estimateSize(inputParams);
    
    return size;
}

chunk *AVC1Box::dumpBox(dictionary *inputParams)
{
    uint32_t i,n;
    
    avcSampleEntry.entry.sampleEntry.header.size = (uint64_t)estimateSize(inputParams);
    avcSampleEntry.entry.sampleEntry.header.type = htonl(AVC1_BOX);
    chunk *c = new chunk((uint32_t) avcSampleEntry.entry.sampleEntry.header.size, false);
    
    /* read the sample entry from our dict */
    ctx->getStream()->getDict(MKSTRING(avcSampleEntry.entry.sampleEntry.dataRefIdx), &avcSampleEntry.entry.sampleEntry.dataRefIdx);
    ctx->getStream()->getDict(MKSTRING(avcSampleEntry.entry.width), &avcSampleEntry.entry.width);
    ctx->getStream()->getDict(MKSTRING(avcSampleEntry.entry.height), &avcSampleEntry.entry.height);
    ctx->getStream()->getDict(MKSTRING(avcSampleEntry.entry.horizResolution), &avcSampleEntry.entry.horizResolution);
    ctx->getStream()->getDict(MKSTRING(avcSampleEntry.entry.vertResolution), &avcSampleEntry.entry.vertResolution);
    ctx->getStream()->getDict(MKSTRING(avcSampleEntry.entry.frameCount), &avcSampleEntry.entry.frameCount);
    ctx->getStream()->getDict(MKSTRING(avcSampleEntry.entry.depth), &avcSampleEntry.entry.depth);
    avcSampleEntry.entry.sampleEntry.dataRefIdx = htons(avcSampleEntry.entry.sampleEntry.dataRefIdx);
    avcSampleEntry.entry.pre_defined = 0;
    avcSampleEntry.entry.reserved = 0;
    avcSampleEntry.entry.width = htons(avcSampleEntry.entry.width);
    avcSampleEntry.entry.height = htons(avcSampleEntry.entry.height);
    avcSampleEntry.entry.horizResolution = htonl(avcSampleEntry.entry.horizResolution);
    avcSampleEntry.entry.vertResolution = htonl(avcSampleEntry.entry.vertResolution);
    avcSampleEntry.entry._reserved = 0;
    avcSampleEntry.entry.frameCount = htons(1);
    avcSampleEntry.entry.depth = htons(avcSampleEntry.entry.depth);
    avcSampleEntry.entry.predef = htons(-1);
    
    for(i = 0; i < sizeofarray(avcSampleEntry.entry.pre_defined_); i++) avcSampleEntry.entry.pre_defined_[i] = 0;
    for(i = 0; i < sizeofarray(avcSampleEntry.entry.sampleEntry.reserved); i++) avcSampleEntry.entry.sampleEntry.reserved[i] = 0;
    
    n = htonl((uint32_t)avcSampleEntry.entry.sampleEntry.header.size);
    
    c->append((uint8_t *) &n, sizeof(uint32_t));
    c->append((uint8_t *) &avcSampleEntry.entry.sampleEntry.header.type, sizeof(uint32_t));
    c->append((uint8_t *) &avcSampleEntry.entry.sampleEntry.reserved, sizeof(uint8_t) * sizeofarray(avcSampleEntry.entry.sampleEntry.reserved));
    c->append((uint8_t *) &avcSampleEntry.entry.sampleEntry.dataRefIdx, sizeof(avcSampleEntry.entry.sampleEntry.dataRefIdx));
    c->append((uint8_t *) &avcSampleEntry.entry.pre_defined, sizeof(avcSampleEntry.entry.pre_defined));
    c->append((uint8_t *) &avcSampleEntry.entry.reserved, sizeof(avcSampleEntry.entry.reserved));
    c->append((uint8_t *) &avcSampleEntry.entry.pre_defined_, sizeof(uint32_t) * sizeofarray(avcSampleEntry.entry.pre_defined_));
    c->append((uint8_t *) &avcSampleEntry.entry.width, sizeof(avcSampleEntry.entry.width));
    c->append((uint8_t *) &avcSampleEntry.entry.height, sizeof(avcSampleEntry.entry.height));
    c->append((uint8_t *) &avcSampleEntry.entry.horizResolution, sizeof(avcSampleEntry.entry.horizResolution));
    c->append((uint8_t *) &avcSampleEntry.entry.vertResolution, sizeof(avcSampleEntry.entry.vertResolution));
    c->append((uint8_t *) &avcSampleEntry.entry._reserved, sizeof(avcSampleEntry.entry._reserved));
    c->append((uint8_t *) &avcSampleEntry.entry.frameCount, sizeof(avcSampleEntry.entry.frameCount));
    c->append((uint8_t *) &avcSampleEntry.entry.compressorname, sizeof(uint8_t) * sizeofarray(avcSampleEntry.entry.compressorname));
    c->append((uint8_t *) &avcSampleEntry.entry.depth, sizeof(avcSampleEntry.entry.depth));
    c->append((uint8_t *) &avcSampleEntry.entry.predef, sizeof(avcSampleEntry.entry.predef));
    
    for(i = 0; i < boxes.size(); i++) c->append(boxes.at(i)->dumpBox(inputParams));
    return c;
}

void AVC1Box::addBox(IBox *b)
{
    boxes.push_back(b);
}
