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

#include "dictionary.h"
#include "log.h"

dictionary::dictionary()
{
}

dictionary::~dictionary()
{
    std::map<uint8_t *, uint8_t *>::iterator it;
    
    for(it = dict.begin(); it != dict.end(); ++it)
    {
        //  mde_log(MDE_LOG_DEBUG, "%s: freeing %s.\n", __FILE__, (*it).first);
        free((*it).second);
        free((*it).first);
    }
}

int32_t dictionary::addDictStr(uint8_t *entry_name, uint8_t* value)
{
    dict.insert(std::make_pair(sys_strdup(entry_name), sys_strdup(value)));
    return 0;
}

template<> int32_t dictionary::addDict<uint8_t*>(uint8_t *entry_name, uint8_t* value)
{
    dict.insert(std::make_pair(sys_strdup(entry_name), sys_strdup(value)));
    return 0;
}

/*template<> int32_t dictionary::getDict<uint8_t *>(uint8_t *entry_name, uint8_t **pointer)
{
    std::map<uint8_t *, uint8_t *>::iterator it;
    
    for (it = dict.begin(); it != dict.end(); ++it)
    {
        if(sys_strcompare((*it).first, entry_name) == 0){
            *pointer = ;
            return 0;
        }
    }
    return -1;
}*/

void dictionary::dumbDictEntries()
{
    std::map<uint8_t *, uint8_t *>::iterator it;
    
    mde_log(MDE_LOG_DEBUG, "%s: Dumb of entries (no data):\n", __FILE__);
    
    for(it = dict.begin(); it != dict.end(); ++it)
    {
        mde_log(MDE_LOG_DEBUG, "%s\n", (*it).first);
    }
}