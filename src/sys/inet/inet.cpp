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

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>
#include "../../buffer/chunk.h"
#include "../../buffer/IBuffer.h"
#include "defs.h"
#include "log.h"
#include "sysreadwrite.h"
#include "sysnet.h"

struct ifaddrs *getNetworkInterfaces()
{
    struct ifaddrs *interfaces;
    if(getifaddrs(&interfaces) < 0)
    {
        mde_log(MDE_LOG_DEBUG, "%s: error getting interfaces.\n", __FILE__);
        return NULL;
    }
    return interfaces;
}

void freeNetworkInterfaces(struct ifaddrs *interfaces)
{
    freeifaddrs(interfaces);
}

int32_t getNetworkInterfacesCount()
{
    struct ifaddrs *interfaces,*ints;
    int32_t interface_count=0;
 
    interfaces = getNetworkInterfaces();
    if(interfaces == NULL) return _ERR;
    
    ints = interfaces;
    for(; ints != NULL;)
    {
       interface_count++;
       ints = ints->ifa_next;
    }
 
    freeNetworkInterfaces(interfaces);
    return interface_count;
}

uint8_t *getIPAddress(uint32_t interface)
{
    struct ifaddrs *interfaces, *ints;
    uint8_t *address = NULL;
    uint32_t i=0;
    
    interfaces = getNetworkInterfaces();
    ints = interfaces;
    
    if(interfaces == NULL) return NULL;
    
    for(; i < interface; i++)
    {
        if(ints == NULL) break;
        ints = ints->ifa_next;
    }

    if(ints != NULL)
    {
        switch(ints->ifa_addr->sa_family) {
            case AF_INET:
                address = (uint8_t *) malloc(INET_ADDRSTRLEN);
                inet_ntop(AF_INET, &(((struct sockaddr_in *)ints->ifa_addr)->sin_addr), (char *)address, INET_ADDRSTRLEN);
                break;
                
            case AF_INET6:
                address = (uint8_t *) malloc(INET6_ADDRSTRLEN);
                inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)ints->ifa_addr)->sin6_addr), (char *)address, INET6_ADDRSTRLEN);
                break;
                
            default:
                address = NULL;
                break;
        }
    }
    freeNetworkInterfaces(interfaces);
    return address;
}


uint8_t *getNetworkInterfaceName(uint32_t interface)
{
    struct ifaddrs *interfaces, *ints;
    uint8_t *name = NULL;
    uint32_t i=0;
    
    interfaces = getNetworkInterfaces();
    ints = interfaces;
    
    if(interfaces == NULL) return NULL;
    
    for(; i < interface; i++)
    {
        if(ints == NULL) break;
        ints = ints->ifa_next;
    }
    
    if(ints != NULL)
    {
        name = sys_strdup((uint8_t *)ints->ifa_name);
    }
    freeNetworkInterfaces(interfaces);
    return name;
}

/* TODO: check FLAGS to identify loopback device, ... */