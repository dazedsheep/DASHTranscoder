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

#include "demux.h"
#include "Sample.h"


// this demuxer may be used for only a single raw bitstream too, then it is nohting else than a wrapper for the codec and will handle the parsing

demux::demux(Context *ctx, IFormat *container) : runable(1)
{
    this->parsedContainer = container;
    this->rawStream = parsedContainer->getRawStream();
    this->ctx = ctx;
    this->objectBuffer = ctx->getSampleBuffer();
    initCodecs();
}

/*
 * used for demuxing raw mdat streams (esp. for mobile devices)
 */
demux::demux(Context *ctx, IBuffer *rawStream) : runable(1)
{
    this->parsedContainer = NULL;
    this->rawStream = rawStream;
    /*
     * when demuxing raw media streams, we have to know the codecs in advance, so the codecs have to be set in advance
     */
    this->ctx = ctx;
    initCodecs();
}

demux::~demux()
{
    uint32_t i;
    for(i=0; i < coders.size(); i++) delete coders.at(0);
}

void demux::initCodecs()
{
    uint32_t i;
    for(i=0; i < ctx->size(); i++) coders.push_back(getCodec(ctx->getStream(i)));
}

void demux::demultiplex()
{
    uint32_t i;
    int32_t auOffset, sampleType;
    Sample *sample;
    chunk *c;
    mde_log(MDE_LOG_DEBUG, "demux: input buffer offset: %llu\n",rawStream->getOffset());
    
    if(objectBuffer == NULL)
    {
        mde_log(MDE_LOG_DEBUG, "demux: error, object buffer is not initialized.\n");
        return;
    }
    
    while(!this->rawStream->isEOF())
    {
        /* test all decoders if they can identify their access unit */
            /* if there is a container format given, we will use it to get our video/audio/meta data */
            if(!parsedContainer)
            {
                /* hmmm, this should not happen, because we cannot demux from a raw bitstream ..., nevertheless try it ... (FOR NOW) */
                for(i = 0; i < coders.size(); i++)
                {
                    if( (auOffset = coders.at(i)->testForRawBitStream(rawStream)) >= 0)
                    {
                        mde_log(MDE_LOG_DEBUG, "codec-%s: found access unit at offset: %llu\n", coders.at(i)->getCodecName(), rawStream->getOffset() + auOffset);
                        /* consume those odd bytes in between us and the AU */
                        delete rawStream->read(auOffset);
                        c = coders.at(i)->parseRawBitStream(rawStream);
                        sample = new Sample(c, coders.at(i)->getCodecFourCC());
                        objectBuffer->write(sample);
                        break;
                    }else
                    {
                        if(this->rawStream->isEOF()) return;
                    }
                }
            }else{
                
                sample = parsedContainer->readSample();
                
                if(sample != NULL)
                {
                    if(sample->data() == NULL) mde_log(MDE_LOG_ERROR, "demux: empty sample found.\n");
                    for(i = 0; i < coders.size(); i++)
                    {
                        if(sample->getFourCC() == coders.at(i)->getCodecFourCC())
                        {
                            sampleType = coders.at(i)->parseFromContainer(sample->data());
                            if(sampleType == _ERR)
                            {
                                /* TODO: error handling */
                                mde_log(MDE_LOG_ERROR, "%s: invalid sample type.\n", __FILE__);
                                delete sample;
                                break;
                            }
                            sample->setSampleType(sampleType);
                            objectBuffer->write(sample);
                            break;
                        }
                    }
                   
                }else{
                    if(this->rawStream->isEOF()) break;
                    mde_log(MDE_LOG_ERROR, "demux: error getting sample from container format.\n");
                }
                
            }
    };
    this->objectBuffer->setEOF();
}

void demux::run()
{
    this->rwP->attach((void*) &this->demultiplexingThread, (void*) this);
}

void demux::run_blocked()
{
   this->rwP->attachAndWait((void*) &this->demultiplexingThread, (void*) this);
}

void *demux::demultiplexingThread(void *params)
{
    demux *_this = (demux *)params;
    _this->demultiplex();
}
