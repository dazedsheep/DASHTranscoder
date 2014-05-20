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

#include <stdint.h>
#include <iostream>
#include <stdlib.h>

#if (defined WIN32 || defined WIN64)
#include <Windows.h>
#endif

#include "workerPool.h"
#include "runable.h"

runable::runable(unsigned int threads)
{
	rwP = new workerPool(threads);
}

runable::~runable()
{
	delete rwP; // will terminate all threads
}

void runable::run() 
{
	std::cout << " unblocked " << std::endl;	// just a debug message, when not overridden
}

void runable::run_blocked()
{
	std::cout << " blocking callee " << std::endl;  // just a debug message, when not overridden
}
