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

#ifndef _ICODER_H_
#define _ICODER_H_

class ICoder
{
    
public:
    virtual ~ICoder() {};

    virtual const uint8_t *getCodecName()=0;
    virtual int32_t testForRawBitStream(IBuffer *buffer)=0;
    virtual int32_t writeRawBitStream(const char *fileName)=0;
    virtual int32_t parseRawBitStream(IBuffer *buffer)=0;
    virtual int32_t parseFromContainer(chunk *data)=0;
    virtual uint32_t getCodecFourCC()=0;
};

#endif
