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
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include "fdinout.h"
#include "log.h"


// this class is supposed to not use fstream because we want direct access to the file descriptor and furthermore, it is used to wrap an existing fd

fdinout::fdinout(int32_t fd)
{
    this->fd = fd;
    // should check whether the fd is valid or not
    this->selfInit = false;
}

fdinout::fdinout(std::string file)
{
    this->fd = open(file.c_str(), O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO);
    if(this->fd < 0) mde_log(MDE_LOG_ERROR, "%s: could not open file (%s), invalid fd: %d, error: %s.\n", __FILE__, file.c_str(), this->fd, strerror(errno));
    this->selfInit = true;    
}

fdinout::fdinout(std::string file, bool create)
{
    this->fd  = open(file.c_str(), O_CREAT | O_RDWR, S_IRWXU | S_IRWXG | S_IRWXO);
    if(this->fd < 0) mde_log(MDE_LOG_ERROR, "%s: could not open file (%s), invalid fd: %d, error: %s.\n", __FILE__, file.c_str(), this->fd, strerror(errno));
    this->selfInit = true;
}

fdinout::~fdinout()
{
    // if we get the fd we wont touch it ...
    if(this->selfInit) close(fd);
}

int32_t fdinout::get(uint8_t *buf, uint32_t len)
{
    int32_t ret = 0, got=0;
    fd_set readset;
    
    do{
        FD_ZERO(&readset);
        FD_SET(this->fd, &readset);
        ret = select(this->fd + 1, &readset, NULL, NULL, NULL);
    }while(ret == -1 && errno == EINTR);
    
    if(ret < 0)
    {
        mde_log(MDE_LOG_ERROR, "fdinout: error: %s before read\n.", strerror(errno));
        return -errno;
    }
    ret = 0;

    if(FD_ISSET(this->fd, &readset))
    {
        while(len > 0)
        {
            got = read(this->fd, buf + ret, len);
            if(this->fd < 0) return _ERR_INVALID_FD;
            if(got < 0) return _ERR_READING_FROM_FD;
            if(got == 0) return _ERR_EOF;
            ret += got;
            len -= got;
        };
    }
    return ret;
}

int32_t fdinout::put(uint8_t *buf, uint32_t len)
{
    int32_t ret = 0, got=0;
    if(this->fd == -1 ) mde_log(MDE_LOG_ERROR, "%s: invalid file descriptor: %d.\n", __FILE__, this->fd);
    while (len > 0)
    {
        got = write(this->fd, buf + ret, len);
        ret+=got;
        if(got < 0) return _ERR_WRITING_TO_FD;
        if(this->fd < 0) return _ERR_INVALID_FD;
        len -= ret;
    };
    return ret;    
}
