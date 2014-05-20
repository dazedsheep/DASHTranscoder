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

#include <arpa/inet.h>

#include "boxReader.h"
#include "log.h"

// do not search boxes out of order ... this may result into data loss because some buffer implementations will get rid of already read bytes, so be careful how you use this

boxReader::boxReader()
{
    
}

boxReader::~boxReader()
{

}

IBox *boxReader::parseBox(Context *ctx, uint32_t boxType, IBuffer *isoFile, box_t *boxHeader)
{
    IBox *realBox;
    
    realBox = BoxFactory::parse(ctx, boxType, isoFile, boxHeader); // this may create a new boxReader to read other boxes included in this box
    
    return realBox;
}

void boxReader::readBoxHeader(IBuffer *isoFile, box_t *header)
{
    header->size = sys_rb32(isoFile);
    if(header->size == 0)
    {
        mde_log(MDE_LOG_ERROR, "isom: fatal error, box size is ZERO.\n");
        return;
    }
    header->realSize = (header->size == 1 ? sizeof(uint64_t) : sizeof(uint32_t)) + sizeof(uint32_t);
    header->type = sys_rb32(isoFile);
    if(header->size == 1)
        header->size = sys_rb64(isoFile);
    mde_log(MDE_LOG_DEBUG, "isom: read box header with type: %c%c%c%c and size %d.\n", ((uint8_t*)(&header->type))[3],((uint8_t*)(&header->type))[2], ((uint8_t*)(&header->type))[1], ((uint8_t*)(&header->type))[0], header->size);

}

/* this will try to parse a set of given boxes */
std::vector<IBox *> &boxReader::parseBoxes(Context *ctx, IBuffer *isoFile, uint32_t *boxList, uint32_t listLength)
{
    box_t header;
    uint32_t i;
    uint64_t n=0;
    IBox *box = NULL;
    bool found = false;
    
    while(n<listLength && !isoFile->isEOF())
    {
        readBoxHeader(isoFile, &header);
        
        if(header.size == 0) return parsedBoxes;
        
        if(header.type == MDAT_BOX)
            break;
        
        for(i = 0; i < listLength; i++)
        {
            if(header.type == boxList[i])
            {
                mde_log(MDE_LOG_DEBUG, "isom: found box %c%c%c%c matched (0x%x,0x%x).\n", ((uint8_t*)(&header.type))[3],((uint8_t*)(&header.type))[2], ((uint8_t*)(&header.type))[1], ((uint8_t*)(&header.type))[0], header.type, boxList[i]);
                box = parseBox(ctx, header.type, isoFile, &header);
                n++;
            }
        }
        
        if (!box)
        {
            mde_log(MDE_LOG_DEBUG, "isom: could not match any box of the given ones to %c%c%c%c (unknown box) (file: %s, line: %d, function: %s).\n", ((uint8_t*)(&header.type))[3],((uint8_t*)(&header.type))[2], ((uint8_t*)(&header.type))[1], ((uint8_t*)(&header.type))[0], __FILE__, __LINE__, __FUNCTION__);
            mde_log(MDE_LOG_DEBUG, "isom: digesting %d bytes to skip unknown box.\n", header.size - header.realSize);
            /* type did not match any of the searched boxes (or the box is not yet implemented but we already searched for it)? if so, skip this box */
            delete isoFile->read(header.size - header.realSize);
        }else{
            parsedBoxes.push_back(box);
            box = NULL;
        }
        
    };
    return parsedBoxes;
}

std::vector<IBox *> &boxReader::parseBoxes(Context *ctx, IBuffer *isoFile, bool hierarchical, bool untilEOF)
{
    box_t header;
    IBox *foundBox;
    
    if(hierarchical) mde_log(MDE_LOG_DEBUG, "isom: parsing iso file in a hierarchical manner.\n");
        else
            mde_log(MDE_LOG_DEBUG, "isom: parsing iso file in a linear manner.\n");
    
    while(!isoFile->isEOF())
    {
        readBoxHeader(isoFile, &header);
        
        /* crash if there is no box size */
        if(header.size == 0) return parsedBoxes;
        
        /* stop if we see the mdat box */
        if(header.type == MDAT_BOX)
        {
            mde_log(MDE_LOG_DEBUG, "isom: found MDAT box, heading forward to demuxer ...\n");
            if(!untilEOF)break;
        }
           
        if(hierarchical)
        {
            switch (header.type)
            {
                case FTYP_BOX:
                case MOOV_BOX:
                case MOOF_BOX:
                    //case MFRA_BOX:
                    parsedBoxes.push_back(parseBox(ctx, header.type, isoFile, &header));
                    break;
                default:
                    /* a box we dont know, skip it for now */
                    mde_log(MDE_LOG_DEBUG, "isom: could not find a mandatory root box, skipping the found one. (file: %s, line: %d, function: %s).\n", __FILE__, __LINE__, __FUNCTION__);
                    mde_log(MDE_LOG_DEBUG, "isom: digesting %d bytes to skip unknown box.\n", header.size - header.realSize);
                    
                    isoFile->digest(header.size - header.realSize);
                    break;
            }
        }else{
            foundBox = parseBox(ctx, header.type, isoFile, &header);
            if(foundBox)
            {
                parsedBoxes.push_back(foundBox);
            }else
            {
                mde_log(MDE_LOG_DEBUG, "isom: digesting %d bytes to skip unknown box. (file: %s, line: %d, function: %s).\n", header.size - header.realSize, __FILE__, __LINE__, __FUNCTION__);
                isoFile->digest(header.size - header.realSize);
            }
        }
    }
    return parsedBoxes;
}


int32_t boxReader::skipUntilMDAT(IBuffer *isoFile)
{
    uint8_t box[4];
    while(!isoFile->isEOF())
    {
        isoFile->peek(box,sizeofarray(box));
        if(box[0] == 'm' && box[1] == 'd' && box[2] == 'a' && box[3] == 't')
        {
            sys_rb32(isoFile);
            mde_log(MDE_LOG_DEBUG, "boxReader: found MDAT box.\n");
            return 0;
        }else{
            sys_rb8(isoFile);
        }
    }
    return -1;
}

IBox *boxReader::readBox(Context *ctx, IBuffer *isoFile)
{
    box_t header;
    
    if(!isoFile->isEOF())
    {
        readBoxHeader(isoFile, &header);
        return parseBox(ctx, header.type, isoFile, &header);
    }
    return NULL;
}

IBox *boxReader::readBox(Context *ctx, IBuffer *isoFile, uint32_t boxType)
{
    box_t header;
    
    while(!isoFile->isEOF())
    {
        readBoxHeader(isoFile, &header);
      
        if(header.type == boxType)
            return parseBox(ctx, header.type, isoFile, &header);
        
        if(header.type == MDAT_BOX)
            break;
        
        /* other box type => skip this box */
        mde_log(MDE_LOG_DEBUG, "isom: digesting %d bytes to skip unwanted/unknown box.\n", header.size - header.realSize);
        isoFile->digest(header.size - header.realSize);
    };    
    return NULL;
}