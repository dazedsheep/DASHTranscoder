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

#ifndef _EVENT_H_
#define _EVENT_H_


// for windows we just use the available methods for events, for *nix we implement a little work around

struct threadEvent{

#if (defined WIN32 || WIN64)
    HANDLE ev;
#else
    mutex_t evmutex;
    pthread_cond_t ev;
    bool set;
#endif
    
};

struct barrier{
    mutex_t barrier_mutex;
    threadEvent *barrier_event;
    uint32_t waiting;
    uint32_t max;
};

threadEvent* event_create(); // creates a new event
void event_init(threadEvent* event); // initializes the event
void event_wait(threadEvent* event); // waits for a given event, you will have to unlock the mutex ...
void event_wait(threadEvent* event, mutex_t mutex); // this will use your mutex
void event_wait_without_unlock(threadEvent* event, mutex_t mutex);
void event_wait_without_unlock(threadEvent* event);
void event_unlock_event_mutex(threadEvent* event);
void event_reset(threadEvent* event); // resets the given event
void event_reset(threadEvent* event, mutex_t mutex);
void event_signal(threadEvent* event); // signals the given event
void event_signal(threadEvent* event, mutex_t mutex);
void event_destroy(threadEvent* event); // destroys the given event, incl. mem cleanup
void event_barrier_sync(barrier* b);
struct barrier* event_barrier_create(uint32_t threads);
void event_barrier_destroy(barrier* b);
void event_barrier_wait(barrier* b);
void event_barrier_reset(barrier* b);
#endif
