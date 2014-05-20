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

#ifndef _DICTIONARY_HPP_
#define _DICTIONARY_HPP_

template<typename T> int32_t addDict(uint8_t *entry_name, T* value, uint32_t size)
{
    dict.insert(std::make_pair(sys_strdup(entry_name), sys_dup((uint8_t *)value, size)));
    return 0;
};

template<typename T> int32_t addDict(uint8_t *entry_name, T value)
{
    T val = value;
    return addDict(entry_name, (uint8_t *)&val, sizeof(T));
};

template<typename T> int32_t getDict(uint8_t *entry_name, T *ret)
{
    std::map<uint8_t *, uint8_t *>::iterator it;
   
    for (it = dict.begin(); it != dict.end(); ++it)
    {
        if(sys_strcompare((*it).first, entry_name) == 0){
            *ret = *((T*)(*it).second);
            return 0;
        }
    }
    mde_log(MDE_LOG_ERROR, "%s: could not find entry: %s, within current dictionary.\n", __FILE__, entry_name);
    return -1;
};

template<typename T> int32_t getDict(uint8_t *entry_name, T* *ret)
{
    std::map<uint8_t *, uint8_t *>::iterator it;
    
    for (it = dict.begin(); it != dict.end(); ++it)
    {
        if(sys_strcompare((*it).first, entry_name) == 0){
            *ret = (T*)(*it).second;
            return 0;
        }
    }
    mde_log(MDE_LOG_ERROR,"%s: could not find entry: %s, within current dictionary.\n",__FILE__, entry_name);
    return -1;
};


#endif
