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

#include "Stream.h"
#include "log.h"
Stream::Stream()
{
    fourcc = MKTAG('n','o','n','e');
}


Stream::~Stream()
{
}

void Stream::dumbEntries()
{
    this->dumbDictEntries();
}

uint32_t Stream::getFourCC()
{
    return fourcc;
}

void Stream::setFourCC(uint32_t tag)
{
    fourcc = tag;
}

void Stream::setSamples(std::vector<Sample*> *_s)
{
    samples = _s;
}

Sample *Stream::getSample(uint32_t sample)
{
    
    if(samples == NULL)
    {
        mde_log(MDE_LOG_DEBUG, "%s: sample container is empty.\n", __FILE__);
        return NULL;
    }
    return samples->at(sample);
}