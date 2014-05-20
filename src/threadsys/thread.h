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
 
#ifndef _THREAD_H_
#define _THREAD_H_

#if (defined WIN32 || defined WIN64)
typedef HANDLE  h_thread;
typedef DWORD*  (*threadfunc)(void *params);
typedef DWORD   threadId;
typedef DWORD   threadFuncReturnType;
typedef LPVOID*  threadFuncParamType;
#else
typedef void*       h_thread;
typedef pthread_t   threadId;
typedef void* (*threadfunc)(void *params);
typedef void*       threadFuncReturnType;
typedef void*       threadFuncParamType;
#endif


struct thread {

    h_thread thread_handle;
    threadfunc function;
    threadId id;
    
};

thread* thread_create(threadfunc tf, void *params);
void *thread_join(thread* t);
void thread_destroy(thread* t);

#endif
