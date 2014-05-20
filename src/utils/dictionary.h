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

#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <map>

#include "../buffer/chunk.h"
#include "../buffer/IBuffer.h"

#include "defs.h"
#include "sysreadwrite.h"
#include "log.h"

class dictionary {

private:
    std::map<uint8_t *, uint8_t *> dict;
   
public:
    dictionary();
    ~dictionary();
    /* I know it is not nice too, but way better than all the template implementations (just bloats this file ....) */
    int32_t addDictStr(uint8_t *entry_name, uint8_t* value);
    #include "dictionary.hpp"
    
    /* Debug dumb */
    void dumbDictEntries();
};
#endif