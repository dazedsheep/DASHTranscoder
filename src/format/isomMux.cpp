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

#if (defined WIN32 || WIN64)
#include <Windows.h>
#else
#include <pthread.h>
#endif

#include "ibmff.h"


// a single thread for demuxing or muxing should be enough

IBMFF::IBMFF() : runable(1)
{
    
    
}

IBMFF::~IBMFF()
{
    if(boxreader != NULL) delete boxreader;
}

int32_t IBMFF::open(IBuffer *isoFile)
{
    
    this->isoFile = isoFile;
    this->boxreader = new boxReader();
    
    return 0;
}


IDecoderInformation *IBMFF::getDecoderInformation()
{
    
    // read the decoder infromation from the stsd box, therefore we use the AVCSampleEntry ...
    
    return NULL;
}


int32_t IBMFF::multiplexSegmented(IDecoderInformation &decoderInfo, std::vector<IBuffer*> &streams)
{
    
    // for now only AVC elementary streams are supported and only segmented IBMFF will be created for now
    return 0;
    
}

IDecoderInformation *IBMFF::demultiplex(std::vector<IBuffer *> &out_BufferList)
{
    // create the buffers
    synchronizedBuffer *videoES = new synchronizedBuffer(VIDEO_BUFFER);
   
    this->bufferList.push_back(videoES);
    
    out_BufferList.push_back(videoES);
    
    // JUST FOR TESTING NOW!!!!! - TODO: introduce full demuxing when all necessary boxes are introduced ...
    
    
    boxReader *reader = new boxReader();
    
    IBox *f = reader->readBox(this->isoFile, "ftyp");
    delete reader;
    
    if(f != NULL) delete f;
    
    /// TESTING .... END
    
    
    // TODO read boxes
    // before we start to demux all the things we will try to find the decoder information which should be located in the 'stsd' box
    
    decoderInfo = getDecoderInformation();
    
   // this->rwP->attach((void *)&this->demultiplexingThread, (void *)this);
    
    return decoderInfo;
}

void IBMFF::run()
{
    
    
}

void IBMFF::run_blocked()
{
    
    
}

void *IBMFF::demultiplexingThread(void *params)
{
    IBMFF *_this = (IBMFF *)params;
    
    while(!_this->isoFile->isEOF())
    {
        // read elementary stream(s)
        
        
    };
    
}

void *IBMFF::multiplexingThread(void *params)
{
    
    
    
}