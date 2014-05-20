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
#include <iostream>
#include <stdint.h>


#if (defined WIN32 || defined WIN64)
#include <Windows.h>
#endif

#include "workerPool.h"


workerPool::workerPool(void)
{
	// check how many CPUs / cores we have
#if (defined WIN32 || defined WIN64)
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	// create our worker pool
	workers = sysInfo.dwNumberOfProcessors;

#else
    
#if (defined ANDROID)
    workers = ANDROID_MAX_THREADS;
#else
    workers = sysconf(_SC_NPROCESSORS_ONLN);
#endif
    
#endif

	initThreads();

}


workerPool::workerPool(unsigned int workers)
{

	this->workers = workers;

    initThreads();
    
}

void workerPool::initThreads()
{
    
    threads = (thread**) new thread[workers]();
	threadIDs = new threadId[workers]();
	    
	wParam = (workerParam*) calloc(workers,sizeof(workerParam));
    
    threadsReady = event_barrier_create(workers);
    
    for(uint32_t i=0;i<workers;i++)
	{
		// create mutex for every thread, events, ...
		wParam[i].threadData = NULL;
		wParam[i].tEvent = event_create();
		event_init(wParam[i].tEvent);
		wParam[i].threadID = i;
		wParam[i].threadMutex = mutex_create(false);
		wParam[i].threadFunc = NULL;
		wParam[i].running = true;
		threads[i] = thread_create(&worker, &wParam[i]);
		wParam[i].wThread = threads[i];
		
		wParam[i].workerFinished = threadsReady;
		wParam[i].workers = workers;
	}
    
	poolMutex = mutex_create(false);
}


threadFuncReturnType workerPool::worker(threadFuncParamType params)
{
	workerParam *wParam = (workerParam*) params;
	
	while(wParam->running)
	{
			
		event_wait(wParam->tEvent);
		
        
        event_reset(wParam->tEvent);
		
        mutex_lock(wParam->threadMutex);
		
		// call our function
		if(wParam->threadFunc != NULL) wParam->threadFunc(wParam->threadData,wParam);
		
		mutex_unlock(wParam->threadMutex);
        
		if(wParam->synchronized) event_barrier_sync(wParam->workerFinished);
	
	};
	return (threadFuncReturnType)0;
}


int workerPool::attachAndWait(void *threadFunc,void *params)
{
    
	if(threadFunc == NULL) return -1;

	mutex_lock(poolMutex);

	for(unsigned int i=0;i<workers;i++)
	{
		mutex_lock(wParam[i].threadMutex);		// lock thread
		wParam[i].threadData = params;
		wParam[i].synchronized = true;
		wParam[i].threadFunc = (ThreadFunction)threadFunc;
		event_signal(wParam[i].tEvent);	// tell him that there is work
		mutex_unlock(wParam[i].threadMutex);	// unlock thread
	}
	
	// wait on all threads, create a barrier for this ... because in the pthread library conditions do not have a memory
	event_barrier_wait(threadsReady);

    event_barrier_reset(threadsReady);

	mutex_unlock(poolMutex);
    

	return 0;
}

int workerPool::attach(void *threadFunc, void *params)
{
	if(threadFunc == NULL) return -1;

	mutex_lock(poolMutex);

	for(unsigned int i=0;i<workers;i++)
	{
		mutex_lock(wParam[i].threadMutex);		// lock thread
		wParam[i].threadData = params;
		wParam[i].synchronized = false;
		wParam[i].threadFunc = (ThreadFunction)threadFunc;
		event_signal(wParam[i].tEvent);	// tell him that there is work
		mutex_unlock(wParam[i].threadMutex);	// unlock thread
	}
	
	mutex_unlock(poolMutex);

	return 0;

}

workerPool::~workerPool(void)
{
    mutex_destroy(poolMutex);

	for(unsigned int i=0;i<workers;i++)
	{
        wParam[i].running = false;
        
        mutex_lock(wParam[i].threadMutex);		// lock thread
		wParam[i].threadData = NULL;
		wParam[i].synchronized = false;
		wParam[i].threadFunc = NULL;
		event_signal(wParam[i].tEvent);	// tell him that there is work
		mutex_unlock(wParam[i].threadMutex);	// unlock thread
        
        thread_join(wParam[i].wThread);         //  join the thread and wait until it is done
                
        thread_destroy(wParam[i].wThread);
        event_destroy(wParam[i].tEvent);
        mutex_destroy(wParam[i].threadMutex);
        
	}

	free(wParam);
	event_barrier_destroy(threadsReady);
	delete []threads;
	delete []threadIDs;

}
