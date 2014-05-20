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

#include "mutex.h"
#include "event.h"

threadEvent *event_create()
{
    threadEvent* event;
    event = (threadEvent *) calloc(1, sizeof(threadEvent));
    return event;
}

void event_init(threadEvent* event)
{
#if (defined WIN32 || defined WIN64)
    event->ev = CreateEvent(NULL, TRUE, FALSE, NULL);
#else
    event->evmutex = mutex_create(false);
    pthread_cond_init(&event->ev,0);
    event->set = false;
#endif
}

void event_wait(threadEvent* event)
{
#if (defined WIN32 || defined WIN64)
    WaitForSingleObject(event->ev,INFINTE);
#else
    mutex_lock(event->evmutex);
    while(!event->set) pthread_cond_wait(&event->ev, event->evmutex);
    mutex_unlock(event->evmutex);
#endif
}

void event_wait(threadEvent* event, mutex_t mutex)
{
#if (defined WIN32 || defined WIN64)
    WaitForSingleObject(event->ev,INFINTE);
#else
    mutex_lock(mutex);
    while(!event->set) pthread_cond_wait(&event->ev, mutex);
    mutex_unlock(mutex);
#endif
}


void event_wait_without_unlock(threadEvent* event)
{
#if (defined WIN32 || defined WIN64)
    WaitForSingleObject(event->ev,INFINTE);
#else
    mutex_lock(event->evmutex);
    while(!event->set) pthread_cond_wait(&event->ev, event->evmutex);
#endif
}

void event_wait_without_unlock(threadEvent* event, mutex_t mutex)
{
#if (defined WIN32 || defined WIN64)
    WaitForSingleObject(event->ev,INFINTE);
    mutex_lock(mutex);
#else
    while(!event->set) pthread_cond_wait(&event->ev, mutex);
#endif
}

void event_unlock_event_mutex(threadEvent* event)
{
    mutex_unlock(event->evmutex);
}


void event_reset(threadEvent* event)
{
#if (defined WIN32 || defined WIN64)
    ResetEvent(event->ev);
#else
    mutex_lock(event->evmutex);
    event->set = false;
    mutex_unlock(event->evmutex);
#endif
}

void event_reset(threadEvent* event, mutex_t mutex)
{
#if (defined WIN32 || defined WIN64)
    ResetEvent(event->ev);
#else
    mutex_lock(mutex);
    event->set = false;
    mutex_unlock(mutex);
#endif
}

void event_signal(threadEvent* event)
{
#if (defined WIN32 || defined WIN64)
    SetEvent(event->ev);
#else
    mutex_lock(event->evmutex);
    event->set = true;
    pthread_cond_signal(&event->ev);
    mutex_unlock(event->evmutex);
#endif
}

void event_signal(threadEvent* event, mutex_t mutex)
{
#if (defined WIN32 || defined WIN64)
    SetEvent(event->ev);
#else
    
    event->set = true;
    pthread_cond_signal(&event->ev);

#endif
}

void event_destroy(threadEvent* event)
{
    if(event == NULL) return;
#if (defined WIN32 || defined WIN64)
    CloseHandle(event->ev);
#else
    mutex_destroy(event->evmutex);
    pthread_cond_destroy(&event->ev);
#endif
    free(event);
}

barrier* event_barrier_create(uint32_t threads)
{
    barrier* b = (struct barrier*)calloc(1, sizeof(barrier));
    
    b->barrier_mutex = mutex_create(false);
    b->barrier_event = event_create();
    event_init(b->barrier_event);
    b->max = threads;
    
    return b;
}

void event_barrier_sync(barrier* b)
{
    mutex_lock(b->barrier_mutex);
    b->waiting++;
    mutex_unlock(b->barrier_mutex);
    if(b->waiting >= b->max) event_signal(b->barrier_event,b->barrier_mutex);
}

// the implementation of the barrier sync for windows is not very nice ... but it should do it

void event_barrier_wait(barrier* b)
{
        
     event_wait(b->barrier_event,b->barrier_mutex);

}

void event_barrier_reset(barrier* b)
{
    b->waiting = 0;
}

void event_barrier_destroy(barrier* b)
{
    
    mutex_destroy(b->barrier_mutex);
    event_destroy(b->barrier_event);
    free(b);
    
}
