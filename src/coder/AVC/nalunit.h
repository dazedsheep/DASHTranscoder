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

#ifndef _NALUNIT_H_
#define _NALUNIT_H_

typedef struct{
    uint8_t type;
    const char *info;
}NALList_t;

#define AVC_NAL_SVC_EXTENSION_FLAG          0x0D
#define AVC_NAL_MVC_EXTENSION_FLAG          0x14

#define AVC_NAL_UNSPECIFIED                 0x00
#define AVC_NAL_SLICE_OF_NON_IDR_PIC        0x01
#define AVC_NAL_SLICE_DATA_PART_A           0x02
#define AVC_NAL_SLICE_DATA_PART_B           0x03
#define AVC_NAL_SLICE_DATA_PART_C           0x04
#define AVC_NAL_SLICE_OF_IDR_PIC            0x05
#define AVC_NAL_SUPPLEMENTAL_ENH_INF        0x06
#define AVC_NAL_SEQUENCE_PARAMETER_SET      0x07
#define AVC_NAL_PICTURE_PARAMETER_SET       0x08
#define AVC_NAL_ACCESS_UNIT_DELIMITER       0x09
#define AVC_NAL_END_OF_SEQUENCE             0x0A
#define AVC_NAL_END_OF_STREAM               0x0B
#define AVC_NAL_FILLER                      0x0C
#define AVC_NAL_SEQUENCE_PARAMETER_SET_EXT  0x0D
#define AVC_NAL_PREFIX_NAL_UNIT             0x0E
#define AVC_NAL_SUBSET_SEQUENCE_PARAMETER_SET   0x0F
/* 16 to 18 is reserved */
#define AVC_NAL_SLICE_EXTENSION             0x10

NALList_t nalUnitTypes[] = {
    { AVC_NAL_SLICE_OF_NON_IDR_PIC, "Slice of non IDR picture"},
    { AVC_NAL_SLICE_DATA_PART_A, "Slice containing data partition A"},
    { AVC_NAL_SLICE_DATA_PART_B, "Slice containing data partition B"},
    { AVC_NAL_SLICE_DATA_PART_C, "Slice containing data partition C"},
    { AVC_NAL_SLICE_OF_IDR_PIC, "Slice of IDR picture"},
    { AVC_NAL_SUPPLEMENTAL_ENH_INF, "Supplemental enhancement information"},
    { AVC_NAL_SEQUENCE_PARAMETER_SET, "Sequence parameter set"},
    { AVC_NAL_PICTURE_PARAMETER_SET, "Picture parameter set"},
    { AVC_NAL_ACCESS_UNIT_DELIMITER, "Access unit delimiter"},
    { AVC_NAL_END_OF_SEQUENCE, "End of sequence"},
    { AVC_NAL_END_OF_STREAM, "End of stream"},
    { AVC_NAL_FILLER, "Filler data"},
    { AVC_NAL_SEQUENCE_PARAMETER_SET_EXT, "Sequence parameter set extensions"},
    { AVC_NAL_PREFIX_NAL_UNIT, "Prefix NAL unit"},
    { AVC_NAL_SUBSET_SEQUENCE_PARAMETER_SET, "Subset sequence parameter set"},
    { AVC_NAL_SLICE_EXTENSION, "Slice extension"},
};



typedef struct{
    uint8_t zero_bit:1;
    uint8_t ref_idc:2;
    uint8_t nal_type:5;
    uint8_t nalHeaderSize;
}NALHeader_t;


typedef struct{
    uint32_t first_mb_in_slice;
    uint32_t slice_type;
    uint32_t pic_parameter_set_id;
    uint32_t color_plane_id:2;
    uint32_t frame_num;
    /* TODO extend */
}SliceHeader_t;

/* The PPS and SPS may be read from a raw byte stream, or may be parsed form the isom AVC sample entry */

typedef struct{
    uint32_t pic_parameter_set_id;
    uint32_t seq_parameter_set_id;
    uint8_t entropy_coding_mode_flag:1;
    uint8_t bottom_field_pic:1;
    uint32_t num_slice_groups_minus1;
}pps_t;

typedef struct{
    
    
}sps_t;

#endif