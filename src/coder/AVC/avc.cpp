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
 
 
#include "../../utils/Stream.h"
#include "avc.h"
#include "nalunit.h"
#include "../../inoutformat/fdinout.h"
#include "../../buffer/chunkBuffer.h"
#include "../../format/Sample.h"

AVC::AVC(Stream *stream)
{
    codecName = (uint8_t *)"AVC/H264";
    fourcc = AVC_FOURCC;
    IDRSlice = BSlice = PSlice = 0;
    this->stream = stream;
    if(stream) this->getParameterSets();
    IDRFrameNum = 99;
    sameFrameNum = false;
}

AVC::~AVC()
{
    uint32_t i;
    for(i = 0; i < NALUnits.size(); i++)
    {
        delete NALUnits.at(i);
    }
}

void AVC::getParameterSets()
{
    /* get the SPS and PPS from the parsed container (if possible)*/
    uint8_t *sps,*pps;
    uint16_t length;
    
    /* TODO: check the number of SPS and PPS and get all the sets */
    
    if(stream->getDict(MKSTRING(AVCDecoderConfig.sequenceParameterSet[0]), &sps) == 0)
    {
        stream->getDict(MKSTRING(AVCDecoderConfig.sequenceParameterSet[0].length), &length);
        chunk *sps_c = new chunk(length, true);
        sps_c->append(sps, length);
        NALUnits.push_back(sps_c);
        mde_log(MDE_LOG_DEBUG, "%s: got SPS from container with length %d.\n", codecName,length);
    }else{
        mde_log(MDE_LOG_ERROR, "%s: got no SPS from container (will try to find SPS in byte stream).\n", codecName);
    }
    
    if(stream->getDict(MKSTRING(AVCDecoderConfig.pictureParameterSet[0]), &pps) == 0)
    {
        stream->getDict(MKSTRING(AVCDecoderConfig.pictureParameterSet[0].length), &length);
        chunk *pps_c = new chunk(length, true);
        pps_c->append(pps, length);
        NALUnits.push_back(pps_c);
        mde_log(MDE_LOG_DEBUG, "%s: got PPS from container with length %d.\n", codecName,length);
    }else{
        mde_log(MDE_LOG_ERROR, "%s: got no PPS from container (will try to find SPS in byte stream).\n", codecName);
    }
}

chunk *AVC::findNextNALUnit(IBuffer *buffer)
{
    uint32_t ret = 0,i,j, offset = 0;
    uint8_t data[4];
    
    chunk *c = new chunk(1, true);
    do
    {
        if(buffer->peek(data, 3) == 0)
            break;
        if(data[0] == 0x00 && data[1] == 0x00 && data[2] == 0x01) return (c);
        if(buffer->peek(data, 4) == 0)
            break;
        if(data[0] == 0x00 && data[1] == 0x00 && data[2] == 0x00 && data[3] == 0x01) return (c);
        c->append(buffer->read(1));
    }while(!buffer->isEOF());
    
    if(buffer->isEOF())
    {
        /* eat up remaining bytes, peek may have arrived at EOF */
        c->append(buffer->read(buffer->bufferedBytes()));
        mde_log(MDE_LOG_DEBUG, "%s: tried to scan for NAL unit but ended up at EOF since I've seen the last NAL unit.\n", codecName);
    }
    return c;
}

int32_t AVC::testForRawBitStream(IBuffer *buffer)
{
    int32_t ret = 0,i,j, offset=0;
    uint8_t data[4];
    
    ret = buffer->peek(data, 3);
    if(data[0] == 0x00 && data[1] == 0x00 && data[2] == 0x01) return (ret);
    ret = buffer->peek(data, 4);
    if(data[0] == 0x00 && data[1] == 0x00 && data[2] == 0x00 && data[3] == 0x01) return (ret);
    return _ERR;
}

chunk *AVC::getRBSPFromNALUnit(chunk *c, void *ptrHeader)
{
    uint32_t i;
    uint8_t bytes[3];
    
    NALHeader_t *header = (NALHeader_t *) ptrHeader;
    chunkBuffer *buffer = new chunkBuffer(c);
    chunk *rbsp = new chunk(sizeof(uint8_t), true);
    for (i = 0; i < buffer->getSize() - header->nalHeaderSize; i++)
    {
        if( (i + 2) < (buffer->getSize() - header->nalHeaderSize))
        {
            if( buffer->peek(bytes, 3) == 0 ) return NULL;
            
            if(bytes[2] == 0x03)
            {
                rbsp->append(buffer->read(2));
                
                i += 2;
                sys_rb8(buffer);    /* eat the emulation prevention byte (0x03) */
                continue;
            }
        }
        rbsp->append(buffer->read(1));
    }
    delete buffer;
    return rbsp;
}

uint32_t get_bit(uint8_t *base, uint32_t offset)
{
    return ((*(base + (offset >> 0x3))) >> (0x7 - (offset & 0x7))) & 0x1;
}

uint32_t decodeExpGolomb(uint8_t *base, uint32_t *offset)
{
    uint32_t zeros = 0;

    // count zero bits. Will be optimized.
    while (0 == get_bit(base, (*offset)++)) zeros++;

    // insert first 1 bit
    uint32_t info = 1 << zeros;
    (*offset)++;

    for (int32_t i = zeros - 1; i >= 0; i--)
    {
        info |= get_bit(base, (*offset)++) << i;
    }
    return (info - 1);
}

void AVC::parseSlice(chunk *rbsp)
{
    /* TODO implement exp golomb decoding */
    uint32_t off = 0;
    uint32_t fm = decodeExpGolomb(rbsp->getPointer(), &off);
    uint32_t slice_type = decodeExpGolomb(rbsp->getPointer(), &off);
    uint32_t pic_param = decodeExpGolomb(rbsp->getPointer(), &off);
    uint32_t frame_num = *(rbsp->getPointer() + off);
   // get_bit(rbsp->getPointer(), off++);
   // get_bit(rbsp->getPointer(), off++);
    if(frame_num == IDRFrameNum) sameFrameNum = true;
    IDRFrameNum = frame_num;
    //mde_log(MDE_LOG_DEBUG, "%d, %d, %d, Frame num in slice header: %d", fm, slice_type, pic_param, frame_num);
    delete rbsp;
}

void AVC::parseSPS(chunk *rbsp)
{
    delete rbsp;
}

void AVC::parsePPS(chunk *rbsp)
{
    delete rbsp;
}

void AVC::checkSliceHeader(chunk *rbsp)
{
    delete rbsp;    
}

/* todo: check whether this is a valid NAL unit or not */
int32_t AVC::parseNALUnit(chunk *c)
{
    uint32_t i;
    int32_t ret = -1;
    uint8_t firstNALByte;
    chunkBuffer *buffer = new chunkBuffer(c);
    
    NALHeader_t header;
    firstNALByte = sys_rb8(buffer);
    header.nal_type = firstNALByte & 0x1F;
    header.ref_idc = (firstNALByte >> 5) & 0x03;
    header.zero_bit = (firstNALByte >> 7) & 0x01;
    header.nalHeaderSize=1;
    
    for(i = 0; i < sizeofarray(nalUnitTypes); i++)
    {
        
        if(nalUnitTypes[i].type == header.nal_type)
        {
            //mde_log(MDE_LOG_DEBUG, "%s: found NAL unit with type (%d,%d) and info: %s.\n", codecName, nalUnitTypes[i].type, header.nal_type, nalUnitTypes[i].info);
            ret = _NOERR;
            break;
        }
    }
    
    if(header.nal_type == AVC_NAL_SVC_EXTENSION_FLAG || header.nal_type == AVC_NAL_MVC_EXTENSION_FLAG)
    {
        /* skip the SVC or MVC extensions... */
        header.nalHeaderSize += 3;
    }
    
    // check the type and then decide what to do
    
    switch (header.nal_type)
    {
        case AVC_NAL_SLICE_OF_IDR_PIC:
            IDRSlice++;
            mde_log(MDE_LOG_DEBUG, "%d perdictive slices after last IDR", PSlice);
            PSlice = 0;
            //parseSlice(getRBSPFromNALUnit(c, &header));
            ret = SAMPLE_IS_FRAME;
            sameFrameNum = false;
            break;
        case AVC_NAL_SLICE_OF_NON_IDR_PIC:
            PSlice++;
                       //parseSlice(getRBSPFromNALUnit(c, &header));
            ret = SAMPLE_IS_FRAME;
            sameFrameNum = false;
            break;
        case AVC_NAL_SLICE_DATA_PART_A:
            mde_log(MDE_LOG_DEBUG, "AVC: found slice DATA PART A");
            break;
        case AVC_NAL_SLICE_DATA_PART_B:
            mde_log(MDE_LOG_DEBUG, "AVC: found slice DATA PART B");
            break;
        case AVC_NAL_SLICE_DATA_PART_C:
            mde_log(MDE_LOG_DEBUG, "AVC: found slice DATA PART C");
            break;
        case AVC_NAL_SEQUENCE_PARAMETER_SET:
            /* only if we are reading from a compliant raw file/stream */
            parseSPS(getRBSPFromNALUnit(c, &header));
            ret = PARAMETER_SET;
            break;
        case AVC_NAL_PICTURE_PARAMETER_SET:
            /* only if we are reading from a compliant raw file/stream */
            parsePPS(getRBSPFromNALUnit(c, &header));
            ret = PARAMETER_SET;
            break;
    }
    
    delete buffer;
    return ret;
}

const uint8_t *AVC::getCodecName()
{
    return codecName;
}

int32_t AVC::writeRawBitStream(const char *fileName)
{
    uint32_t i;
    fdinout fd(fileName, true);
    uint8_t data = 0x00;
    
    /* TODO: generate PPS and SPS if they are not available */
    
    fd.put(&data, sizeof(uint8_t));
    
    for(i = 0; i < NALUnits.size(); i++)
    {
        data = 0x00;
        fd.put(&data, sizeof(uint8_t));
        fd.put(&data, sizeof(uint8_t));
        data = 0x01;
        fd.put(&data, sizeof(uint8_t));
        fd.put(NALUnits.at(i)->getPointer(), NALUnits.at(i)->getOffset());
        if( i < NALUnits.size()-1)
        {
            data = 0x00;
            fd.put(&data, sizeof(uint8_t));
        }
    }
    
    mde_log(MDE_LOG_DEBUG, "%s: %d NAL units, slices:  %d I, %d B, %d P\n", codecName, NALUnits.size(), IDRSlice, BSlice , PSlice);
}

chunk *AVC::parseRawBitStream(IBuffer *buffer)
{
    int32_t ret;
    /* should parse the NALUnit ...*/
    chunk *c = findNextNALUnit(buffer);
    ret = parseNALUnit(c);
    /* TODO: error checking */
    return c;
}

int32_t AVC::parseFromContainer(chunk *data)
{
    /* use the container to get the NAL units, the container format will handle alle the necessary things */
    int32_t ret;
    if(!data) return _ERR;
    //    c = data->dup();
    ret = parseNALUnit(data);
    //NALUnits.push_back(c);
    return  ret;
}

uint32_t AVC::getCodecFourCC()
{
    return fourcc;
}
