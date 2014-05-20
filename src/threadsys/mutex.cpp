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

mutex_t mutex_create(bool initialOwner)
{
#if (defined WIN32 || defined WIN64)
	return CreateMutex(NULL, initialOwner, NULL);
#else
	mutex_t mt = (mutex_t) malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(mt, NULL);
	return mt;
#endif
}

uint32_t mutex_lock(mutex_t m)
{
#if (defined WIN32 || defined WIN64)
	return WaitForSingleObject(m, INFINITE);
#else
	return pthread_mutex_lock(m);
#endif
}

uint32_t mutex_try_lock(mutex_t m, unsigned int time)	// time should specify how long we will wait for the lock / how long we will try to get the given lock
{
#if (defined WIN32 || defined WIN64)
	return WaitForSingleObject(m, time);
#else
	return pthread_mutex_trylock(m);
#endif	
}

uint32_t mutex_unlock(mutex_t m)
{
#if (defined WIN32 || defined WIN64)
	return ReleaseMutex(m);
#else
	return pthread_mutex_unlock(m);
#endif
}

void mutex_destroy(mutex_t m)
{
#if (defined WIN32 || defined WIN64)
	CloseHandle(m);
#else
	pthread_mutex_destroy(m);
	free(m);
#endif
}