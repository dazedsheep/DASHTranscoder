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

#include "codecs.h"
#include "log.h"
#include "AVC/avc.h"

/* extend this to register the codecs */

CODEC_CREATE_EXTERNAL_CONSTRUCTOR(AVC);

struct codec_definition codecs[] = {
    { MKTAG('n','o','n','e'), NULL},
    { AVC_FOURCC, CODEC_REF_CONSTRUCTOR(AVC) }
};

ICodec *getCodec(Stream *stream)
{
    uint32_t i;
    for(i = 0; i < sizeofarray(codecs); i++) if(codecs[i].codecType == stream->getFourCC()) return codecs[i].ctor(stream);
    
    mde_log(MDE_LOG_ERROR, "codecs: could not find any suitable codec for type 0x%x (fourcc)\n", stream->getFourCC());
    return NULL;
}