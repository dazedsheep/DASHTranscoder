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
#ifndef _FDINPUT_H_
#define _FDINPUT_H_

#include <stdint.h>
#include <string>
#include <fcntl.h>
#include <unistd.h>

#include "defs.h"


#include "IInOutFormat.h"

class fdinout : public IInOutFormat{
protected:
    int32_t fd;
    bool selfInit;
#ifdef ANDROID
    uint64_t off;
    uint64_t size;
#endif
public:
    fdinout(int32_t fd);
    fdinout(std::string file);
    fdinout(std::string file, bool create);
    ~fdinout();
    
    int32_t get(uint8_t *buf, uint32_t len);
    int32_t put(uint8_t *buf, uint32_t len);
};

#endif