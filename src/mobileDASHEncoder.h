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

#ifndef _MOBILEDASHENCODER_H_
#define _MOBILEDASHENCODER_H_

#include <stdint.h>

class mobileDashEncoder {
private:
    void *transcoder;
    
public:
    mobileDashEncoder(std::string fileName, const char *baseName, int32_t segmentLength, int32_t fps);
    mobileDashEncoder(int32_t fd, const char *baseName, int32_t segmentLength, int32_t fps);
    ~mobileDashEncoder();
    void dumbToFile(uint8_t *fileName, int32_t fd);
    void start(const char *fileName);
    void test();
    void dumb();
};

#endif