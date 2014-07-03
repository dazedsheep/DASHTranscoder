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

#include "buffer/chunk.h"
#include "inoutformat/fdinout.h"
#include "buffer/synchronizedFileBuffer.h"
#include "format/isom.h"
#include "format/demux.h"
#include "log.h"
#include "sysreadwrite.h"
#include "transcode.h"
#include "mobileDASHEncoder.h"



mobileDashEncoder::mobileDashEncoder(std::string fileName, const char *baseName, int32_t segmentLength, int32_t fps)
{
    transcoder =(void *) new transcode(fileName, (char *)baseName, segmentLength, fps);
}

mobileDashEncoder::mobileDashEncoder(int32_t fd, const char *baseName, int32_t segmentLength, int32_t fps)
{
    transcoder =(void *) new transcode(fd, (char *)baseName, segmentLength, fps);
}

mobileDashEncoder::~mobileDashEncoder()
{
    if(transcoder != NULL) delete ((transcode*) transcoder);
  
}

void mobileDashEncoder::dumpToFile(uint8_t *fileName, int32_t fd)
{
    ((transcode *)transcoder)->dumpToFile(fileName, fd);
}

void mobileDashEncoder::start(const char *fileName)
{
    ((transcode *)transcoder)->parseSetupFile(fileName);
    ((transcode *)transcoder)->run();
}

void mobileDashEncoder::dump()
{
    ((transcode *)transcoder)->dump();
}

void mobileDashEncoder::test()
{
    ((transcode *)transcoder)->run();
}