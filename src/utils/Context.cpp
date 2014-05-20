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

#include "Context.h"
#include "log.h"

Context::Context()
{
    currentStream = -1;
    sampleBuffer = NULL;
}

Context::~Context()
{
    std::vector<Stream *>::iterator it;
    for(it = streams.begin(); it != streams.end(); ++it)
    {
        delete (*it);
    }
}

int32_t Context::setSampleBuffer(IObjectBuffer<Sample> *buffer)
{
    if(sampleBuffer != NULL) return _ERR;
    sampleBuffer = buffer;
    return _NOERR;
}

IObjectBuffer<Sample> *Context::getSampleBuffer()
{
    return sampleBuffer;
}

void Context::detachSampleBuffer()
{
    delete sampleBuffer;
    sampleBuffer = NULL;
}

int32_t Context::newStream()
{
    mde_log(MDE_LOG_DEBUG, "context: creating new stream.\n");
    streams.push_back(new Stream());
    currentStream++;
    return currentStream;
}

Stream *Context::getStream()
{
    return streams.at(currentStream);
}

Stream *Context::getStream(uint32_t id)
{
    return streams.at(id);
}

int32_t Context::size()
{
    return (currentStream + 1);
}

void Context::dumbEntriesOfStreams()
{
    std::vector<Stream *>::iterator it;
    for(it = streams.begin(); it != streams.end(); ++it)
    {
        (*it)->dumbEntries();
    }
}