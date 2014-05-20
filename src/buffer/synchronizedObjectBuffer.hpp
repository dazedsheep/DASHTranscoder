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

synchronizedObjectBuffer()
{
    eof = false;
    size = 0;
    bufferMutex = mutex_create(false);
    bufferFullEvent = event_create();
    bufferEmptyEvent = event_create();
    event_init(bufferFullEvent);
    event_init(bufferEmptyEvent);
};

~synchronizedObjectBuffer()
{
    uint32_t i;
    mutex_destroy(bufferMutex);
    event_destroy(bufferFullEvent);
    event_destroy(bufferEmptyEvent);
    
    if(container.size() > 0)
    {
        mde_log(MDE_LOG_DEBUG, "%s: there are still %d Samples in the buffer going to delete them.\n",__FILE__, container.size());
        for(i = 0; i < container.size(); i++)
        {
            delete (container.front());
            container.pop();
        }
    }
};

uint32_t write(T *object)
{
    mutex_lock(bufferMutex);
    
    if(size > 0)
    {
        event_wait_without_unlock(bufferEmptyEvent, bufferMutex);
    }
    event_reset(bufferEmptyEvent);
    
    size++;
    if(object == NULL) mde_log(MDE_LOG_ERROR, "objectBuffer: trying to store NULL object.\n");
    
    container.push(object);
    event_signal(bufferFullEvent);
    mutex_unlock(bufferMutex);
    return 0;
};

T *read()
{
    T *object;
    mutex_lock(bufferMutex);
    
    if(size==0)
    {
        /* we should wait until we get at least a single object */
        event_signal(bufferEmptyEvent);
        event_wait_without_unlock(bufferFullEvent, bufferMutex);
    }
    event_reset(bufferFullEvent);
    
    if(isEOF())
    {
        mutex_unlock(bufferMutex);
        return NULL;
    }
    
    size--;
    /* copy the object, since it is a pointer the class itself has to have an appropriate constructor */
    T* n = container.front();
    object =  new T(n);
    delete n;
    container.pop();
    
    mutex_unlock(bufferMutex);
    return object;
};

void setEOF()
{
    mutex_lock(bufferMutex);
    eof = true;
    mutex_unlock(bufferMutex);
};

bool isEOF()
{
    return eof;
};

uint32_t getSize()
{
    return size;
};

void notify()
{
    //NYI
};

void attach(IBufferObserver* observer)
{
    //NYI
};
