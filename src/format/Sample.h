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

#ifndef _SAMPLE_H_
#define _SAMPLE_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "../buffer/chunk.h"

#define SAMPLE_IS_FRAME 0xC0

class Sample{

private:
    uint32_t fourcc;
    chunk *sampleData;
    int32_t type;
public:
    Sample(chunk *c, uint32_t fourCC);
    Sample(Sample *sample); /* not a real copy constructor, but needed for the object buffer */
    ~Sample();

    uint32_t getFourCC();
    chunk *data();
    uint32_t size();
    void setSampleType(int32_t type);
    int32_t getSampleType();
};

#endif