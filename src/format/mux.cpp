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

#include "mux.h"
#include "containers.h"

// a single thread for demuxing or muxing should be enough

multiplexer::multiplexer(Context *ctx, uint32_t containerType, uint32_t containerFlags) : runable(1)
{
    this->ctx = ctx;
    this->objectBuffer = ctx->getSampleBuffer();
    this->container = getContainer(ctx, containerType, containerFlags);
}

multiplexer::~multiplexer()
{
    if(this->container != NULL) delete this->container;
}

void multiplexer::multiplex()
{
    while(!objectBuffer->isEOF())
    {
        container->writeSample(objectBuffer->read());
    };
}

void multiplexer::run()
{
    this->rwP->attach((void *) multiplexingThread, (void *) this);
}

void multiplexer::run_blocked()
{
    this->rwP->attachAndWait((void *) multiplexingThread, (void *) this);
}

void *multiplexer::multiplexingThread(void *params)
{
    multiplexer *_this = (multiplexer*)params;
    _this->multiplex();
}