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

#if (defined WIN32 || WIN64)
#include <Windows.h>
#else
#include <pthread.h>
#endif

#include "containers.h"
#include "log.h"
#include "fragmentedIsom.h"
#include "isom.h"
/* extend this to register the containers */

CONTAINER_CREATE_EXTERNAL_CONSTRUCTOR(isom);
CONTAINER_CREATE_EXTERNAL_CONSTRUCTOR(fragmentedIsom);

struct container_definition containers[] = {
    { MKTAG('i','s','o','m'), 0, CONTAINER_REF_CONSTRUCTOR(isom)},
    { MKTAG('i','s','o','m'), MKTAG('s','e','g','m'), CONTAINER_REF_CONSTRUCTOR(fragmentedIsom)}
};

IFormat *getContainer(Context *ctx, uint32_t containerType, uint32_t containerFlags)
{
    uint32_t i;
    for(i = 0; i < sizeofarray(containers); i++) if(containers[i].containerType == containerType && containers[i].containerFlags == containerFlags) return containers[i].ctor(ctx);
    mde_log(MDE_LOG_ERROR, "containers: could not find any suitable container\n");
    return NULL;
}