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

#ifndef _IBUFFERINFO_H_
#define _IBUFFERINFO_H_



#define DEFAULT_BUFFER          0x00
#define VIDEO_BUFFER            0x01
#define AUDIO_BUFFER            0x02
#define METADATA_BUFFER         0x03

class IBufferInfo{
  
    
public:
    virtual ~IBufferInfo(){};
    
    virtual uint8_t getBufferType()=0;
    virtual uint32_t getSamplingRate()=0;
        
    
};

#endif
