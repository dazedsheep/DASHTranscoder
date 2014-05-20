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
 
#include <string>
#include <stdlib.h>
#include "AVCCBox.h"

AVCCBox::AVCCBox(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->init(ctx, buffer, header);
}

void AVCCBox::init(Context *ctx, IBuffer *buffer, box_t *header)
{
    this->buffer = buffer;
    if(header != NULL) memcpy(&this->box.header, header, sizeof(box_t));
    this->ctx = ctx;
    container = false;
    box.AVCDecoderConfig.pictureParameterSets = NULL;
    box.AVCDecoderConfig.sequenceParameterSets = NULL;
}

AVCCBox::~AVCCBox()
{
    uint32_t i;
    
    /* clean up sequence parameter set and picture parameter set */
    for(i = 0; i < box.AVCDecoderConfig.numOfSequenceParameterSets; i++)
        free(box.AVCDecoderConfig.sequenceParameterSets[i].parameterSet);
    if(box.AVCDecoderConfig.sequenceParameterSets != NULL) free(box.AVCDecoderConfig.sequenceParameterSets);
    for(i = 0; i < box.AVCDecoderConfig.numOfPictureParameterSets; i++)
        free(box.AVCDecoderConfig.pictureParameterSets[i].pictureSet);
    if(box.AVCDecoderConfig.pictureParameterSets != NULL) free(box.AVCDecoderConfig.pictureParameterSets);
}

int32_t AVCCBox::parse()
{
    uint32_t i, j;
    uint16_t setLength;
    uint8_t *p;
    uint8_t num[10];
    
    mde_log(MDE_LOG_DEBUG, "isom: Parsing AVC configuration box.\n");
    
    box.AVCDecoderConfig.configurationVesrion = sys_rb8(buffer);
    box.AVCDecoderConfig.AVCProfileIndication = sys_rb8(buffer);
    box.AVCDecoderConfig.profileCompatibility = sys_rb8(buffer);
    box.AVCDecoderConfig.AVCLevelIndication = sys_rb8(buffer);
    box.AVCDecoderConfig.lengthSizeMinusOne  = sys_rb8(buffer) & 0x03;
    box.AVCDecoderConfig.numOfSequenceParameterSets = sys_rb8(buffer) & 0x1F;
    box.AVCDecoderConfig.sequenceParameterSets = (sequenceParameterSetNalUnit_t *) malloc(sizeof(sequenceParameterSetNalUnit_t) * box.AVCDecoderConfig.numOfSequenceParameterSets);
    
    ctx->getStream()->addDict(MKSTRING(AVCDecoderConfig.configurationVesrion), box.AVCDecoderConfig.configurationVesrion);
    ctx->getStream()->addDict(MKSTRING(AVCDecoderConfig.AVCProfileIndication), box.AVCDecoderConfig.AVCProfileIndication);
    ctx->getStream()->addDict(MKSTRING(AVCDecoderConfig.profileCompatibility), box.AVCDecoderConfig.profileCompatibility);
    ctx->getStream()->addDict(MKSTRING(AVCDecoderConfig.AVCLevelIndication), box.AVCDecoderConfig.AVCLevelIndication);
    ctx->getStream()->addDict(MKSTRING(AVCDecoderConfig.lengthSizeMinusOne), box.AVCDecoderConfig.lengthSizeMinusOne);
    ctx->getStream()->addDict(MKSTRING(AVCDecoderConfig.numOfSequenceParameterSets), box.AVCDecoderConfig.numOfSequenceParameterSets);
    
    for(i = 0; i < box.AVCDecoderConfig.numOfSequenceParameterSets; i++)
    {
        setLength = sys_rb16(buffer);
        p = (uint8_t *)malloc(setLength);
        for(j = 0; j < setLength; j++)
            p[j] = sys_rb8(buffer);
        
        box.AVCDecoderConfig.sequenceParameterSets[i].sequenceParameterSetLength = setLength;
        box.AVCDecoderConfig.sequenceParameterSets[i].parameterSet = p;
        sprintf((char *)num, "[%d]", i);
        std::string spsName((char *)MKSTRING(AVCDecoderConfig.sequenceParameterSet));
        spsName.append((char *)num);
        ctx->getStream()->addDict((uint8_t*)spsName.c_str(), p, (uint32_t)setLength);
        spsName.append(".length");
        ctx->getStream()->addDict((uint8_t*)spsName.c_str(), setLength);
    }
    
    box.AVCDecoderConfig.numOfPictureParameterSets = sys_rb8(buffer) & 0x1F;
    box.AVCDecoderConfig.pictureParameterSets = (pictureParameterSetNalUnit_t *) malloc(sizeof(pictureParameterSetNalUnit_t) * box.AVCDecoderConfig.numOfPictureParameterSets);
    ctx->getStream()->addDict(MKSTRING(AVCDecoderConfig.numOfPictureParameterSets), box.AVCDecoderConfig.numOfPictureParameterSets);
    
    for(i = 0; i < box.AVCDecoderConfig.numOfPictureParameterSets; i++)
    {
        setLength = sys_rb16(buffer);
        p = (uint8_t *) malloc(setLength);
        
        for(j = 0; j < setLength; j++)
            p[j] = sys_rb8(buffer);
        
        box.AVCDecoderConfig.pictureParameterSets[i].pictureParameterSetLength = setLength;
        box.AVCDecoderConfig.pictureParameterSets[i].pictureSet = p;
        sprintf((char *)num, "[%d]", i);
        std::string ppsName((char *)MKSTRING(AVCDecoderConfig.pictureParameterSet));
        ppsName.append((char *)num);
        ctx->getStream()->addDict((uint8_t *)ppsName.c_str(), p, (uint32_t)setLength);
        ppsName.append(".length");
        ctx->getStream()->addDict((uint8_t*)ppsName.c_str(), setLength);
    }
    mde_log(MDE_LOG_DEBUG, "isom: Done parsing AVC configuration box.\n");
    return 0;
}

bool AVCCBox::isContainer()
{
    return container;
}

uint32_t AVCCBox::getBoxType()
{
    return box.header.type;
}

std::vector<IBox*> &AVCCBox::getIncludedBoxes()
{
    return boxes;
}

void AVCCBox::printDebugInformation(bool callIncludedBoxes)
{
    mde_log(MDE_LOG_DEBUG, "isom: AVC config box debug info: (configVersion: %d, profile: %d, compatibility: %d, level: %d, lengthSize-1: %d, number of sequence parameter sets: %d, number of picture parameter sets: %d).\n", box.AVCDecoderConfig.configurationVesrion, box.AVCDecoderConfig.AVCProfileIndication, box.AVCDecoderConfig.profileCompatibility, box.AVCDecoderConfig.AVCLevelIndication,box.AVCDecoderConfig.lengthSizeMinusOne, box.AVCDecoderConfig.numOfSequenceParameterSets, box.AVCDecoderConfig.numOfPictureParameterSets);
}

uint32_t AVCCBox::estimateSize(dictionary *inputParams)
{
    uint32_t size = BOX_HEADER_SIZE + sizeof(box.AVCDecoderConfig.configurationVesrion) + sizeof(box.AVCDecoderConfig.AVCProfileIndication) + sizeof(box.AVCDecoderConfig.profileCompatibility) + sizeof(box.AVCDecoderConfig.AVCLevelIndication) + sizeof(box.AVCDecoderConfig.lengthSizeMinusOne) + sizeof(box.AVCDecoderConfig.numOfSequenceParameterSets) + sizeof(box.AVCDecoderConfig.numOfPictureParameterSets);
    uint32_t i;
    uint16_t pLength;
    uint8_t num[10], spsCount, ppsCount;
    
    ctx->getStream()->getDict(MKSTRING(AVCDecoderConfig.numOfSequenceParameterSets), &spsCount);
    ctx->getStream()->getDict(MKSTRING(AVCDecoderConfig.numOfPictureParameterSets), &ppsCount);
    
    for(i = 0; i < spsCount; i++)
    {
        sprintf((char *)num, "[%d]", i);
        std::string spsName((char *)MKSTRING(AVCDecoderConfig.sequenceParameterSet));
        spsName.append((char *)num);
        spsName.append(".length");
        ctx->getStream()->getDict((uint8_t*)spsName.c_str(), &pLength);
        size += pLength;
        size += sizeof(uint16_t);
    }
    
    for(i = 0; i < ppsCount; i++)
    {
        sprintf((char *)num, "[%d]", i);
        std::string ppsName((char *)MKSTRING(AVCDecoderConfig.pictureParameterSet));
        ppsName.append((char *)num);
        ppsName.append(".length");
        ctx->getStream()->getDict((uint8_t*)ppsName.c_str(), &pLength);
        size += pLength;
        size += sizeof(uint16_t);
    }

    return size;    
}

chunk *AVCCBox::dumpBox(dictionary *inputParams)
{
    uint32_t n, i;
    uint16_t pLength, nt;
    uint8_t num[10];
    uint8_t *p, ppsCount, spsCount;
    // calculate the size of this box
    ctx->getStream()->getDict(MKSTRING(AVCDecoderConfig.configurationVesrion), &box.AVCDecoderConfig.configurationVesrion);
    ctx->getStream()->getDict(MKSTRING(AVCDecoderConfig.AVCProfileIndication), &box.AVCDecoderConfig.AVCProfileIndication);
    ctx->getStream()->getDict(MKSTRING(AVCDecoderConfig.profileCompatibility), &box.AVCDecoderConfig.profileCompatibility);
    ctx->getStream()->getDict(MKSTRING(AVCDecoderConfig.AVCLevelIndication), &box.AVCDecoderConfig.AVCLevelIndication);
    ctx->getStream()->getDict(MKSTRING(AVCDecoderConfig.lengthSizeMinusOne), &box.AVCDecoderConfig.lengthSizeMinusOne);
    ctx->getStream()->getDict(MKSTRING(AVCDecoderConfig.numOfSequenceParameterSets), &box.AVCDecoderConfig.numOfSequenceParameterSets);
    ctx->getStream()->getDict(MKSTRING(AVCDecoderConfig.numOfPictureParameterSets), &box.AVCDecoderConfig.numOfPictureParameterSets);
    box.header.size = (uint64_t) estimateSize(inputParams);
    box.header.type = htonl(AVCC_BOX);
    
    chunk *c = new chunk((uint32_t) box.header.size, false);
    n = htonl((uint32_t) box.header.size);
    spsCount = box.AVCDecoderConfig.numOfSequenceParameterSets;
    ppsCount = box.AVCDecoderConfig.numOfPictureParameterSets;
    box.AVCDecoderConfig.lengthSizeMinusOne |= 0xFC;
    box.AVCDecoderConfig.numOfSequenceParameterSets |= (0xFF - 0x1F);
   
    
    c->append((uint8_t *) &n, sizeof(uint32_t));
    c->append((uint8_t *) &box.header.type, sizeof(box.header.type));
    c->append((uint8_t *) &box.AVCDecoderConfig.configurationVesrion, sizeof(box.AVCDecoderConfig.configurationVesrion));
    c->append((uint8_t *) &box.AVCDecoderConfig.AVCProfileIndication, sizeof(box.AVCDecoderConfig.AVCProfileIndication));
    c->append((uint8_t *) &box.AVCDecoderConfig.profileCompatibility, sizeof(box.AVCDecoderConfig.profileCompatibility));
    c->append((uint8_t *) &box.AVCDecoderConfig.AVCLevelIndication, sizeof(box.AVCDecoderConfig.AVCLevelIndication));
    c->append((uint8_t *) &box.AVCDecoderConfig.lengthSizeMinusOne, sizeof(box.AVCDecoderConfig.lengthSizeMinusOne));
    c->append((uint8_t *) &box.AVCDecoderConfig.numOfSequenceParameterSets, sizeof(box.AVCDecoderConfig.numOfSequenceParameterSets));

    /* now the sequence parameter sets */
    for(i = 0; i < spsCount; i++)
    {
        sprintf((char *)num, "[%d]", i);
        std::string spsName((char *)MKSTRING(AVCDecoderConfig.sequenceParameterSet));
        spsName.append((char *)num);
        ctx->getStream()->getDict((uint8_t *)spsName.c_str(), &p);
        spsName.append(".length");
        ctx->getStream()->getDict((uint8_t*)spsName.c_str(), &pLength);
        nt = htons(pLength);
        c->append((uint8_t *) &nt, sizeof(nt));
        c->append(p, pLength);
    }
    
    c->append((uint8_t *) &box.AVCDecoderConfig.numOfPictureParameterSets, sizeof(box.AVCDecoderConfig.numOfPictureParameterSets));

    for(i = 0; i < ppsCount; i++)
    {
        sprintf((char *)num, "[%d]", i);
        std::string ppsName((char *)MKSTRING(AVCDecoderConfig.pictureParameterSet));
        ppsName.append((char *)num);
        ctx->getStream()->getDict((uint8_t *)ppsName.c_str(), &p);
        ppsName.append(".length");
        ctx->getStream()->getDict((uint8_t *)ppsName.c_str(), &pLength);
        nt = htons(pLength);
        c->append((uint8_t *) &nt, sizeof(nt));
        c->append(p, pLength);
    }
  
    /* set the number of sets back to zero, because this could cause problems in the destructor when we free sps and pps */
    box.AVCDecoderConfig.numOfSequenceParameterSets = 0;
    box.AVCDecoderConfig.numOfPictureParameterSets = 0;
    return c;
}
