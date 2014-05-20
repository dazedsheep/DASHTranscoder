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

#ifndef _SYSREADWRITE_H_
#define _SYSREADWRITE_H_

uint8_t sys_rb8(IBuffer *mem);

uint16_t sys_rb16(IBuffer *mem);

uint32_t sys_rb32(IBuffer *mem);

uint64_t sys_rb64(IBuffer *mem);

uint8_t *sys_rstring(IBuffer *mem);

void sys_wb8(IBuffer *mem, uint8_t b);

void sys_wb16(IBuffer *mem, uint16_t d);

void sys_wb32(IBuffer *mem, uint32_t d);

void sys_wb64(IBuffer *mem, uint64_t d);

uint8_t *sys_dup(uint8_t *data, uint32_t size);

uint8_t *sys_strdup(uint8_t *str);

uint8_t sys_strlen(uint8_t *str);

int32_t sys_strcompare(uint8_t *a, uint8_t *b);

#endif