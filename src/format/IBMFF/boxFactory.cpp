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

#include "boxFactory.h"

/* declare our boxes */

BOX_CREATE_EXTERNAL_CONSTRUCTOR(FTYPBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(MOOVBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(MINFBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(TRAKBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(HDLRBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(MDIABox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(DINFBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(DREFBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(STBLBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(STSDBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(STSSBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(STTSBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(STSZBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(STSCBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(STCOBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(MOOFBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(TRAFBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(TKHDBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(MFHDBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(TFHDBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(TRUNBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(STYPBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(SIDXBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(MDHDBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(MVHDBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(MDATBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(TFDTBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(URLBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(MVEXBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(TREXBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(ODBox);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(VMHDBox);

/* sample entry boxes */
BOX_CREATE_EXTERNAL_CONSTRUCTOR(AVC1Box);
BOX_CREATE_EXTERNAL_CONSTRUCTOR(AVCCBox);


/* ref our boxes to their corresponding tag */

struct de_box_definition supportedBoxes[] = {
    { FTYP_BOX, BOX_REF_CONSTRUCTOR(FTYPBox) },
    { MOOV_BOX, BOX_REF_CONSTRUCTOR(MOOVBox) },
    { TRAK_BOX, BOX_REF_CONSTRUCTOR(TRAKBox) },
    { HDLR_BOX, BOX_REF_CONSTRUCTOR(HDLRBox) },
    { MDIA_BOX, BOX_REF_CONSTRUCTOR(MDIABox) },
    { MINF_BOX, BOX_REF_CONSTRUCTOR(MINFBox) },
    { DINF_BOX, BOX_REF_CONSTRUCTOR(DINFBox) },
    { DREF_BOX, BOX_REF_CONSTRUCTOR(DREFBox) },
    { STBL_BOX, BOX_REF_CONSTRUCTOR(STBLBox) },
    { STSD_BOX, BOX_REF_CONSTRUCTOR(STSDBox) },
    { STSS_BOX, BOX_REF_CONSTRUCTOR(STSSBox) },
    { STTS_BOX, BOX_REF_CONSTRUCTOR(STTSBox) },
    { STSZ_BOX, BOX_REF_CONSTRUCTOR(STSZBox) },
    { STSC_BOX, BOX_REF_CONSTRUCTOR(STSCBox) },
    { STCO_BOX, BOX_REF_CONSTRUCTOR(STCOBox) },
    { MOOF_BOX, BOX_REF_CONSTRUCTOR(MOOFBox) },
    { TRAF_BOX, BOX_REF_CONSTRUCTOR(TRAFBox) },
    { TKHD_BOX, BOX_REF_CONSTRUCTOR(TKHDBox) },
    { MFHD_BOX, BOX_REF_CONSTRUCTOR(MFHDBox) },
    { TFHD_BOX, BOX_REF_CONSTRUCTOR(TFHDBox) },
    { TRUN_BOX, BOX_REF_CONSTRUCTOR(TRUNBox) },
    { STYP_BOX, BOX_REF_CONSTRUCTOR(STYPBox) },
    { SIDX_BOX, BOX_REF_CONSTRUCTOR(SIDXBox) },
    { MDHD_BOX, BOX_REF_CONSTRUCTOR(MDHDBox) },
    { MVHD_BOX, BOX_REF_CONSTRUCTOR(MVHDBox) },
    { MDAT_BOX, BOX_REF_CONSTRUCTOR(MDATBox) },
    { TFDT_BOX, BOX_REF_CONSTRUCTOR(TFDTBox) },
    { URL_BOX , BOX_REF_CONSTRUCTOR(URLBox)  },
    { MVEX_BOX, BOX_REF_CONSTRUCTOR(MVEXBox) },
    { TREX_BOX, BOX_REF_CONSTRUCTOR(TREXBox) },
    { OD_BOX,   BOX_REF_CONSTRUCTOR(ODBox)   },
    { VMHD_BOX, BOX_REF_CONSTRUCTOR(VMHDBox)   },
    
    /* sample entries */
    { AVC1_BOX, BOX_REF_CONSTRUCTOR(AVC1Box) },
    { AVCC_BOX, BOX_REF_CONSTRUCTOR(AVCCBox) }
    
};

IBox *BoxFactory::create(Context *ctx, uint32_t boxType)
{
    uint32_t i;
    for( i = 0; i < sizeofarray(supportedBoxes); i++)
    {
        if(boxType == supportedBoxes[i].boxType)
            return supportedBoxes[i].ctor(ctx, NULL, NULL);
    }
    return NULL;
}

IBox *BoxFactory::parse(Context *ctx, uint32_t boxType, IBuffer *buffer, box_t *boxHeader)
{
    uint32_t i;
    IBox *ret = NULL;
    uint64_t cpos = buffer->getOffset();
    
    for( i = 0; i < sizeofarray(supportedBoxes); i++)
    {
        if(boxType == supportedBoxes[i].boxType)
        {
            ret = supportedBoxes[i].ctor(ctx, buffer, boxHeader);
            break;
        }
    }
    
    if(!ret)
        return NULL;
    
    if(ret->parse() < 0)
    {
        mde_log(MDE_LOG_ERROR, "isom: could not parse box (stubs are available, but did not read anything/an unrecoverable error occured, for more info see following lines or set log level to DEBUG).\n");
    }
    
    if((buffer->getOffset() - cpos) > (boxHeader->size - boxHeader->realSize))
        mde_log(MDE_LOG_DEBUG, "isom: read: %d bytes, should read %d bytes, aborting.\n",buffer->getOffset() - cpos,(boxHeader->size - boxHeader->realSize));

    if((boxHeader->size - boxHeader->realSize) > (buffer->getOffset() - cpos))
    {
        
        mde_log(MDE_LOG_WARNING, "isom: box read %d bytes, but has size: %d bytes (without header); going to digest left bytes (%d bytes).\n", (buffer->getOffset() - cpos), (boxHeader->size - boxHeader->realSize), (boxHeader->size - boxHeader->realSize) - (buffer->getOffset() - cpos));
        
        buffer->digest((boxHeader->size - boxHeader->realSize) - (buffer->getOffset() - cpos));
    }
    return ret;
}