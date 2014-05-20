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

#include "Sample.h"
#include "log.h"

Sample::Sample(chunk *c, uint32_t fourCC)
{
    this->sampleData = c;
    this->fourcc = fourCC;
    this->type = 0;
}

Sample::Sample(Sample *sample)
{
    this->sampleData = sample->data()->dup();
    this->fourcc = sample->getFourCC();
    this->type = sample->type;
}

Sample::~Sample()
{
    if(sampleData!=NULL) delete sampleData;
    
    //mde_log(MDE_LOG_DEBUG, "%s: destructor of sample got called.\n", __FILE__);
    
}

chunk *Sample::data()
{
    return sampleData;
}

uint32_t Sample::getFourCC()
{
    return fourcc;
}

uint32_t Sample::size()
{
    return sampleData->getSize();
}

void Sample::setSampleType(int32_t type)
{
    this->type = type;
}

int32_t Sample::getSampleType()
{
    return type;
}