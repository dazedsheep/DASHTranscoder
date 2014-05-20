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

#ifndef _CODECS_H_
#define _CODECS_H_

#include <stdint.h>
#include <defs.h>
#include <arpa/inet.h>
#include <queue>
#include <vector>
#include "../buffer/chunk.h"
#include "../buffer/IBuffer.h"
#include "../utils/Context.h"
#include "ICodec.h"

ICodec *getCodec(Stream *stream);

#define CODEC_CREATE_EXTERNAL_CONSTRUCTOR(codecName)        ICodec *ctor_##codecName(Stream *stream) { return new codecName(stream); }
#define CODEC_REF_CONSTRUCTOR(codecName)                    &(ctor_##codecName)

struct codec_definition {
    uint32_t codecType;
    ICodec* (*ctor)(Stream *stream);
};

#endif
