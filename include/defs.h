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

#ifndef _DEFS_H_
#define _DEFS_H_

/* */

#define     KEY_FRAME               ((int32_t) 0x0000EFEE)
#define     PERDICTIVE_FRAME        ((int32_t) 0x0000DDDD)
#define     PARAMETER_SET           ((int32_t) 0x0000AAAA)

// error codes, extend as you wish


#define     _NOERR                  ((int32_t)0)
#define     _ERR                    ((int32_t)-1)
#define     _NATIVE_FD_ERROR        ((int32_t)-2)
#define     _ERR_OUT_OF_BOUNDS      ((int32_t)-3)
#define     _ERR_READING_FROM_FD    ((int32_t)-4)
#define     _ERR_INVALID_FD         ((int32_t)-5)
#define     _ERR_WRITING_TO_FD      ((int32_t)-6)

#define     _ERR_NYI                ((int32_t)-255)

#define     _ERR_EOF                ((int32_t)-254)

//TODO: string mapping for the error codes for throwing nice exceptions



#define     TAG_LENGTH              4
#define     MKTAG(a,b,c,d)          ( (a << 24) | ( b << 16 ) | ( c << 8 ) | ( d ))
#define     sizeofarray(array)      sizeof(array)/sizeof(array[0])

// on 32 bit system we may not have the ntohll so we have to declare it
uint64_t _htonll(uint64_t value);


#define _ntohll(x) _htonll(x)


#define    STRING(x)                (uint8_t *)(x)
#define    MKSTRING(x)               (uint8_t *)(#x)
template<typename T> struct is_pointer { static const bool pointer = false; };
template<typename T> struct is_pointer<T*> { static const bool pointer = true; };


#endif