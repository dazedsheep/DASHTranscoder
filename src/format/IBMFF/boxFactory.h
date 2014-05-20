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

#ifndef _BOXFACTORY_H_
#define _BOXFACTORY_H_

#include "../../buffer/chunk.h"
#include "../../buffer/IBuffer.h"
#include "../../utils/Context.h"
#include "boxes.h"

#include "FTYPBox.h"
#include "DINFBox.h"
#include "DREFBox.h"
#include "URLBox.h"
#include "MDIABox.h"
#include "MINFBox.h"
#include "MVHDBox.h"
#include "MOOFBox.h"
#include "MOOVBox.h"
#include "STBLBox.h"
#include "STCOBox.h"
#include "STSCBox.h"
#include "STSDBox.h"
#include "STSSBox.h"
#include "STSZBox.h"
#include "STTSBox.h"
#include "TRAFBox.h"
#include "TRAKBox.h"
#include "AVC1Box.h"
#include "AVCCBox.h"
#include "HDLRBox.h"
#include "TKHDBox.h"
#include "MFHDBox.h"
#include "MDHDBox.h"
#include "TFHDBox.h"
#include "TFDTBox.h"
#include "TRUNBox.h"
#include "STYPBox.h"
#include "SIDXBox.h"
#include "MDATBox.h"
#include "MVEXBox.h"
#include "TREXBox.h"
#include "ODBox.h"
#include "VMHDBox.h"

class BoxFactory {
    
public:
    static IBox *create(Context *ctx, uint32_t boxType);
    static IBox *parse(Context *ctx, uint32_t boxType, IBuffer *buffer, box_t *header);
    
};


#endif
