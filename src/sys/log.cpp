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

#include <stdint.h>
#include <stdio.h>
#include <cstdarg>

#include "log.h"


#ifdef ANDROID
#include <android/log.h>
/* TODO: introduce logging for android */
int32_t logger(const char *fmt, va_list ap)
{
    
    __android_log_vprint(ANDROID_LOG_DEBUG, ANDROID_LOG_TAG, fmt, ap);
    
}

#else
int32_t logger(const char *fmt, va_list ap)
{
    
    vprintf(fmt, ap);
    
}
#endif

int32_t mde_log(uint32_t logType, const char *fmt, ...)
{
    /* todo check logType ... */
    va_list args;
    
    switch(logType)
    {
        case MDE_LOG_ERROR:
            break;
        case MDE_LOG_DEBUG:
            break;
    }
    
    va_start(args, fmt);
    logger(fmt, args);
    va_end(args);
  
    return 0;
}
