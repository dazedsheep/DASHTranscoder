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

#ifndef _TRANSCODE_H_
#define _TRANSCODE_H_

#include <stdint.h>

#include "threadsys/mutex.h"
#include "threadsys/event.h"
#include "threadsys/thread.h"
#include "threadsys/workerpool.h"
#include "threadsys/runable.h"

#include "buffer/chunk.h"
#include "inoutformat/fdinout.h"
#include "buffer/synchronizedFileBuffer.h"
#include "format/isom.h"
#include "format/fragmentedIsom.h"
#include "format/demux.h"
#include "format/mux.h"
#include "log.h"
#include "sysreadwrite.h"
#include "buffer/synchronizedObjectBuffer.h"


class transcode : public runable {
private:
    void *demuxer;
    void *container;
    void *buffer;
    void *ctx;
    synchronizedObjectBuffer<Sample> *obuffer;
    multiplexer *muxer;
    int32_t segmentSizeInSeconds;
    int32_t fps;
    void *baseName;
public:
    transcode(std::string fileName, char *baseName, int32_t segmentSizeInSeconds, int32_t fps);
    transcode(int32_t fd, char *baseName, int32_t segmentSizeInSeconds, int32_t fps);
    ~transcode();
    void dumpToFile(uint8_t *fileName, int32_t fd);
    void parseSetupFile(const char *fileName);
    void startwithSetupFile();
    void test();
    void dump();

    /* thread things */
    void run();
    void run_blocked();
    static void *transcodeThread(void *params);
};

#endif