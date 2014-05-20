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

#if (defined WIN32 || WIN64)
#include <Windows.h>
#else
#include <pthread.h>
#endif

#include "log.h"

#include "isom.h"


isom::isom(Context *ctx)
{
    this->ctx = ctx;
}

isom::~isom()
{
    uint32_t i;
    if(boxreader != NULL) delete boxreader;
    for (i = 0; i < boxes.size(); i++)
    {
        if(boxes.at(i) != NULL) delete boxes.at(i);
        
    }
}

int32_t isom::open(IBuffer *isoFile)
{
    
    this->isoFile = isoFile;
    this->boxreader = new boxReader();
    return 0;
}


int32_t isom::parse(bool untilEOF)
{
    uint32_t i;
    /* try to find the following boxes: ftyp, moov, mdat */
    /* when we see the mdat box we will stop parsing */
    
    boxes = boxreader->parseBoxes(ctx, isoFile, true, untilEOF);
    
    if(!boxes.size())
    {
        mde_log(MDE_LOG_ERROR, "isom: no boxes found, invalid isom?\n");
        return -1;  /* need to define these errors ... */
    }
    
    mde_log(MDE_LOG_DEBUG, "isom: found %d root boxes.\n", boxes.size());
    
    for(i = 0; i < boxes.size(); i++) boxes.at(i)->printDebugInformation(true);
}

int32_t isom::skipToMDAT()
{
    return boxreader->skipUntilMDAT(isoFile);
}

IBuffer *isom::getRawStream()
{
    return this->isoFile;
}

Sample *isom::readSample()
{
    /* TODO: implement generic read, for now we just read ... anything :D */
    
    /* for now we just read the mdat box straight forward and say it is AVC ... */
    
    /* the first four bytes is the length of the following chunk in the mdat box (somehow strange, when we got isoms that are actually not streamed live */
    
    uint32_t chunkSize = sys_rb32(isoFile);
    return new Sample(isoFile->read(chunkSize),  MKTAG('a','v','c','1'));
}

int32_t isom::writeSample(Sample *sample)
{
    return _ERR;
}

/*IStreamInformation *isom::getStreamInformations()
{
    return NULL;
}

IStream *isom::getStream(uint32_t iHandle)
{
    return NULL;
}*/