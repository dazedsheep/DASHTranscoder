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
#include <pthread.h>
#include <string>
#include <vector>

#include "transcode.h"



transcode::transcode(std::string fileName, char *baseName, int32_t segmentSizeInSeconds, int32_t fps) : runable(1)
{
    buffer = (void *) new synchronizedFileBuffer(fileName);
    ctx = (void *) new Context();
    container = (void *) new isom((Context *)ctx);
    mde_log(MDE_LOG_DEBUG, "baseName in constructor: %s .\n", baseName);
    this->baseName = new std::string(baseName);
    this->segmentSizeInSeconds = segmentSizeInSeconds;
    this->fps = fps;
}

transcode::transcode(int32_t fd, char *baseName, int32_t segmentSizeInSeconds, int32_t fps) : runable(1)
{
    buffer = (void *) new synchronizedFileBuffer(fd);
    ctx = (void *) new Context();
    container = (void *) new isom((Context *) ctx);
    this->baseName = new std::string(baseName);
    this->segmentSizeInSeconds = segmentSizeInSeconds;
    this->fps = fps;
}

transcode::~transcode()
{
    if(baseName != NULL) delete (std::string *)baseName;
    if(buffer != NULL) delete ((synchronizedFileBuffer*)buffer);
    if(container != NULL) delete ((isom *) container);
    if(demuxer!= NULL) delete ((demux *) demuxer);
    if(muxer != NULL) delete muxer;
    if(obuffer != NULL) delete obuffer;
    if(ctx != NULL) delete((Context *) ctx);
    
}

void transcode::dumpToFile(uint8_t *fileName, int32_t fd)
{
    uint8_t data;
    fdinout fn(std::string((char*)fileName),true);
    mde_log(MDE_LOG_DEBUG, "Dumping stream to file %s.\n", fileName);
    do{
        data = sys_rb8((IBuffer*)buffer);
        fn.put(&data, 1);
    }while(1);
}

void transcode::parseSetupFile(const char *fileName)
{
    mde_log(MDE_LOG_DEBUG, "MDE: parsing setup file...\n");
    synchronizedFileBuffer *buffy = new synchronizedFileBuffer(std::string(fileName));
    isom iso((Context *)ctx);
    iso.open(buffy);
    iso.parse(true);
    mde_log(MDE_LOG_DEBUG, "MDE: done parsing setup file...\n");
    delete buffy;
}

void transcode::startwithSetupFile()
{
    uint8_t *test;
    ((Context*)ctx)->addDictStr(MKSTRING(global.settings.filename), (uint8_t*)((std::string *)baseName)->c_str());
    ((Context*)ctx)->addDict<uint32_t>(MKSTRING(global.settings.segmentSizeInSeconds), segmentSizeInSeconds);
    ((Context*)ctx)->addDict<uint32_t>(MKSTRING(global.settings.fps), fps);
    ((isom*)container)->open((IBuffer*)buffer);
    ((Context *) ctx)->getDict(MKSTRING(global.settings.filename), &test);
    ((isom*)container)->skipToMDAT();
    // try to read the bitstream
      obuffer = new synchronizedObjectBuffer<Sample>();
    ((Context*)ctx)->setSampleBuffer(obuffer);
    demuxer = new demux((Context *)ctx, (IFormat*)container);
    
    mde_log(MDE_LOG_DEBUG,"MDE: running demultiplexer... \n");
    
    ((demux *)demuxer)->run();
    
    mde_log(MDE_LOG_DEBUG, "MDE: init muxer...");
    multiplexer *muxer = new multiplexer((Context*) ctx, ISOM_CONTAINER, FRAGMENTED_ISOM);
    
    mde_log(MDE_LOG_DEBUG, "MDE: running muxer...");
    muxer->run();
    
}

void transcode::dump()
{

}


void transcode::test()
{

}

void transcode::run()
{
    this->rwP->attach((void *) &this->transcodeThread, (void *)this);
}
void transcode::run_blocked()
{
    this->rwP->attachAndWait((void *) &this->transcodeThread, (void *)this);
}

void *transcode::transcodeThread(void *params)
{
    transcode *_this = (transcode *) params;
    _this->startwithSetupFile();
}