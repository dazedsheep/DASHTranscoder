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

#ifndef _MUTEX_H_
#define _MUTEX_H_


#if (defined WIN32 || defined WIN64)
    typedef void* mutex_t;			// HANDLE is already a type definition of (void*)
#else
    typedef pthread_mutex_t* mutex_t;
#endif


mutex_t mutex_create(bool initialOwner);	// should introduce flags
uint32_t mutex_lock(mutex_t m);
uint32_t mutex_try_lock(mutex_t m, uint32_t time);	// time should specify how long we will wait for the lock / how long we will try to get the given lock. NOTE: use checkError to identify if the mutex got locked or not because pthreads do not accept a waiting time ...
uint32_t mutex_unlock(mutex_t m);
void mutex_destroy(mutex_t m);


#endif