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

#ifndef _WORKERPOOL_H_
#define _WORKERPOOL_H_

#include "mutex.h"
#include "event.h"
#include "thread.h"

#define ANDROID_MAX_THREADS     4

typedef struct workerParam_t{
    
    thread* wThread;     // thread for this work item
    
	unsigned int threadID;		// the number of the thread, this is the internal thread number
	unsigned int workers;		// the number of workers/threads within this worker pool
	
	mutex_t threadMutex;
	
	threadEvent *tEvent;
	barrier *workerFinished;
    
	bool running;
	bool synchronized;
	
    	void *threadData;
	int (*threadFunc)(void *params, workerParam_t *wParam);


}workerParam;


class workerPool
{
private:
	threadId *threadIDs;
	uint32_t workers;
	workerParam *wParam;
	thread **threads;
	
    mutex_t poolMutex;
  
	barrier *threadsReady;
	bool running;
	void initThreads();   
public:
	workerPool(void);
	workerPool(unsigned int workers);
	~workerPool(void);

	static threadFuncReturnType worker(threadFuncParamType params);
	int attachAndWait(void *threadFunc,void *params);
	int attach(void *threadFunc, void *params);
};

typedef int (*ThreadFunction)(void *params,workerParam *wParam);

#endif
