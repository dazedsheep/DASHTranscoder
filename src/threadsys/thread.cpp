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
#include <stdlib.h>
#include <stdint.h>

#if (defined WIN32 || defined WIN64)
#include <Windows.h>
#else
#include <pthread.h>
#endif

#include "thread.h"

thread* thread_create(threadfunc tf, void *params)
{
    struct thread *t = (thread*) calloc(1, sizeof(thread));
    
#if (defined WIN32 || WIN64)
    t->thread_handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) tf, params, NULL, &t->id);
#else
    
    pthread_create(&t->id, NULL, tf, params);
    t->thread_handle = NULL; // there is no such handle as with win threads
#endif
    return t;
}

void *thread_join(thread* t)
{
    void *ret = NULL;
#if (defined WIN32 || WIN64)
    WaitForSingleObject(t->thread_handle,INFINITE);
#else
    pthread_join(t->id, &ret);
#endif
    return ret;
    
}

void thread_destroy(thread* t)
{
#if (defined WIN32 || defined WIN64)
    TerminateThread(t->thread_handle);
    CloseHandler(t->thread_handle);    
#endif
    free(t);
}
