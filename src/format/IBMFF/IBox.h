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

#ifndef _IBOX_H_
#define _IBOX_H_

#include <stdint.h>
#include <defs.h>
#include <arpa/inet.h>
#include <queue>
#include <vector>

#include "../../buffer/IBuffer.h"
#include "../../buffer/chunk.h"
#include "../../utils/Context.h"
#include "boxes.h"
#include "../../utils/dictionary.h"

class IBox;
                       
#define BOX_CREATE_EXTERNAL_CONSTRUCTOR(boxName)        IBox *ctor_##boxName(Context *ctx, IBuffer *buffer, box_t *header) { return new boxName(ctx, buffer, header); }
#define BOX_REF_CONSTRUCTOR(boxName)                    &(ctor_##boxName)

struct de_box_definition {
    uint32_t boxType;
    IBox* (*ctor)(Context *ctx, IBuffer *, box_t *);
};

class IBox{
    
public:
    
    virtual ~IBox(){};
    
    virtual bool isContainer()=0;                                       // tells us if this box contains other boxes
    
    virtual uint32_t getBoxType()=0;                                    // ask the box for its type
    
    virtual std::vector<IBox*> &getIncludedBoxes()=0;                   // get the included boxes
    
    virtual int32_t parse() { return _ERR_NYI; };                       // parse the given box
    
    virtual chunk *dumpBox(){ return NULL; };                           // this should return a chunk which includes a byte stream in the proper format and with all boxes included by the current box
    
    virtual chunk *dumpBox(dictionary *inputParams){ return NULL; };
    
    virtual uint32_t estimateSize(dictionary *inputParams) {return 0;};
    
    virtual uint32_t getSize(){ return 0; };                            // this allows temp boxes
    
    virtual void addBox(IBox *) { };
    
    virtual void printDebugInformation(bool callIncludedBoxes){};
};



#endif
