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

#ifndef _SYSNET_H_
#define _SYSNET_H_

#if (defined WIN32 || defined win64)
#include <winsock.h>
#endif

#ifdef ANDROID
#include <sys/types.h>
#include "android/ifaddrs-android.h"
#include <netinet/in.h>
#else
#ifdef LINUX
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#endif
#endif

struct ifaddrs* getNetworkInterfaces();
void freeNetworkInterfaces(struct ifaddrs *interfaces);
int32_t getNetworkInterfacesCount();
uint8_t *getIPAddress(uint32_t interface);
uint8_t *getNetworkInterfaceName(uint32_t interface);

#endif