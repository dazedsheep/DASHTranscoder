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

#ifndef _BOXES_H_
#define _BOXES_H_

#define ISOM_MKTAG(a,b,c,d)             ( (a << 24) | ( b << 16 ) | ( c << 8 ) | ( d ))
#define BOX_STRING_TO_UINT32(x)         ( (((uint32_t) x[3]) << 24) | (((uint32_t) x[2]) << 16) | (((uint32_t) x[1]) << 8) | ((uint32_t) x[0]) )

#define FTYP_BOX_STRING             "ftyp"
#define MOOV_BOX_STRING             "moov"
#define TRAK_BOX_STRING             "trak"
#define TKHD_BOX_STRING             "tkhd"
#define MDIA_BOX_STRING             "mdia"
#define MINF_BOX_STRING             "minf"
#define DINF_BOX_STRING             "dinf"
#define STBL_BOX_STRING             "stbl"
#define MVEX_BOX_STRING             "mvex"
#define MOOF_BOX_STRING             "moof"
#define TRAF_BOX_STRING             "traf"
#define MDAT_BOX_STRING             "mdat"
#define MDHD_BOX_STRING             "mdhd"
#define MVHD_BOX_STRING             "mvhd"
#define HDLR_BOX_STRING             "hdlr"
#define STSD_BOX_STRING             "stsd"
#define STTS_BOX_STRING             "stts"
#define STSS_BOX_STRING             "stss"
#define STSZ_BOX_STRING             "stsz"
#define STSC_BOX_STRING             "stsc"
#define STCO_BOX_STRING             "stco"
#define VMHD_BOX_STRING             "vmhd"

/* handler types */

#define VIDEO_HANDLER_STRING        "vide"
#define AUDIO_HANDLER_STRING        "soun"
#define METADATA_HANDLER_STRING     "meta"
#define HINT_HANDLER_STRING         "hint"

#define VIDEO_HANDLER               ISOM_MKTAG('v','i','d','e')
#define AUDIO_HANDLER               ISOM_MKTAG('s','o','u','n')
#define METADATA_HANDLER            ISOM_MKTAG('m','e','t','a')
#define HINT_HANDLER                ISOM_MKTAG('h','n','i','t')

/* sample entry boxes */

#define AVC1_BOX_STRING             "avc1" /* holds the avc configuration box */
#define AVCC_BOX_STRING             "avcC" /* this box wraps the avc decoder configuration record */

#define FTYP_BOX                    ISOM_MKTAG('f','t','y','p')
#define MOOV_BOX                    ISOM_MKTAG('m','o','o','v')
#define MVHD_BOX                    ISOM_MKTAG('m','v','h','d')
#define TRAK_BOX                    ISOM_MKTAG('t','r','a','k')
#define TKHD_BOX                    ISOM_MKTAG('t','k','h','d')
#define MDIA_BOX                    ISOM_MKTAG('m','d','i','a')
#define MINF_BOX                    ISOM_MKTAG('m','i','n','f')
#define DINF_BOX                    ISOM_MKTAG('d','i','n','f')
#define DREF_BOX                    ISOM_MKTAG('d','r','e','f')
#define STBL_BOX                    ISOM_MKTAG('s','t','b','l')
#define MVEX_BOX                    ISOM_MKTAG('m','v','e','x')
#define TREX_BOX                    ISOM_MKTAG('t','r','e','x')
#define MOOF_BOX                    ISOM_MKTAG('m','o','o','f')
#define TRAF_BOX                    ISOM_MKTAG('t','r','a','f')
#define MDAT_BOX                    ISOM_MKTAG('m','d','a','t')
#define HDLR_BOX                    ISOM_MKTAG('h','d','l','r')
#define MDHD_BOX                    ISOM_MKTAG('m','d','h','d')
#define STSD_BOX                    ISOM_MKTAG('s','t','s','d')
#define STTS_BOX                    ISOM_MKTAG('s','t','t','s')
#define STSS_BOX                    ISOM_MKTAG('s','t','s','s')
#define STSZ_BOX                    ISOM_MKTAG('s','t','s','z')
#define STSC_BOX                    ISOM_MKTAG('s','t','s','c')
#define STCO_BOX                    ISOM_MKTAG('s','t','c','o')
#define STYP_BOX                    ISOM_MKTAG('s','t','y','p')
#define SIDX_BOX                    ISOM_MKTAG('s','i','d','x')
#define MFHD_BOX                    ISOM_MKTAG('m','f','h','d')
#define SIDX_BOX                    ISOM_MKTAG('s','i','d','x')
#define TFHD_BOX                    ISOM_MKTAG('t','f','h','d')
#define TFDT_BOX                    ISOM_MKTAG('t','f','d','t')
#define TRUN_BOX                    ISOM_MKTAG('t','r','u','n')
#define URL_BOX                     ISOM_MKTAG('u','r','l',' ')
#define OD_BOX                      ISOM_MKTAG('i','o','d','s')
#define VMHD_BOX                    ISOM_MKTAG('v','m','h','d')

/* "encoded" sample entry boxes */

#define AVC1_BOX                    ISOM_MKTAG('a','v','c','1')
#define AVCC_BOX                    ISOM_MKTAG('a','v','c','C')

#define IBMFF_USE_LARGE_SIZE        1

#define BOX_HEADER_SIZE             8
#define FULLBOX_HEADER_SIZE         12
#define FULLBOX_HEADER_V1_SIZE      16

static int32_t unityMatrix[9] = { 0x00010000,0,0,0,0x00010000,0,0,0,0x40000000 };


// we do not support user extensions
// TODO: usertype ...

typedef struct {
    uint64_t size;                      // size of this box
    uint32_t type;                      // type of this box
    uint8_t realSize;
}box_t;


// i know code dups but it is handier when we have to use full boxes
typedef struct {
    box_t header;    
    // sums up to 32 bit or 4 bytes
    uint8_t version;
    uint8_t flags[3];
}fullbox_t;

// file type box type == "ftyp"
typedef struct{
    box_t header;
    uint32_t major_brand;
    uint32_t minor_version;
    uint32_t *compatible_brands;        // this will eat up the remaining bytes of this box
}ftyp_t;

// boxes for segments
typedef ftyp_t styp_t;

typedef struct {
    uint32_t trackId;
    uint32_t decodingTime;
}trackcount_t;

typedef struct {
    uint32_t referenceType;
    uint32_t referenceSize;       /* LSB = referencType, 1 -31 = referenceSize */
    uint32_t subsegmentDuration;
    uint32_t startsWithSAP;
    uint32_t SAPType;          /* LSB = startsWithRAP,  1-31 = RAPDeltaTime */
    uint32_t SAPDeltaTime;
}refcount_t;

typedef struct{
    fullbox_t fullbox;
    uint32_t refId;
    uint32_t timescale;
    uint64_t earliestPresentationTime;
    uint64_t firstOffset;
    uint16_t reserved;                  //=0
    uint16_t reference_count;
    /* reference count */
    refcount_t *rc;
}sidx_t;

// ... the name says everything
typedef struct{
    box_t header;
    uint8_t *data;
}freespace_t;

// the actual media data
typedef struct{
    box_t header;
}mdat_t;

/*----- Boxes contained in the Movie Box (moov)
 * 
 *
 */

// the actual movie header
typedef struct{
    fullbox_t fullbox;
    uint64_t creationTime;
    uint64_t modificationTime;
    uint32_t timescale;
    uint64_t duration;
    int32_t rate;                   // default = 0x00010000
    int16_t volume;                 // default full volume = 0x0100
    int16_t reserved[5];            // = 0
    int32_t matrix[9];              // unity matrix
    uint32_t pre_defined[6];        // should be 0
    uint32_t nextTrackId;
}mvhd_t;


// track header
typedef struct{
    fullbox_t fullbox;
    uint64_t creationTime;
    uint64_t modificationTime;
    uint32_t trackId;
    uint32_t reserved;
    uint64_t duration;    
    uint32_t res[2];
    int16_t layer;
    int16_t alternate_group;        // =0
    int16_t volume;                 // audio track others =0
    uint16_t _reserved;
    int32_t matrix[9];                 // unity matrix should be { 0x00010000,0,0,0,0x00010000,0,0,0,0x40000000 }
    uint32_t width;
    uint32_t height;
}tkhd_t;

// track reference
typedef struct{
    box_t header;
}tref_t;

typedef struct{
    fullbox_t fullbox;
    uint64_t creationTime;
    uint64_t modificationTime;
    uint32_t timescale;
    uint64_t duration;
    uint16_t lang;                  //language (msb = 0)
    uint16_t pre_defined;           //=0
}mdhd_t;

typedef struct{
    fullbox_t fullbox;
    uint32_t pre_defined;
    uint32_t handler_type;
    uint32_t reserved;
    uint8_t *name;                  // name of the handler
}hdlr_t;

typedef struct{
    
    fullbox_t fullbox;
    uint8_t *location;
    
}dataEntryUrlBox_t;

typedef struct{
    
    fullbox_t header;
    uint8_t *name;
    uint8_t *location;
    
}dataEntryUrnBox_t;

// data refs in the sources
typedef struct{
    fullbox_t fullbox;
    uint32_t entry_count;
}dref_t;

// data information box
typedef struct{
    
    box_t header;
    dref_t *dataReferences;
    
}dinf_t;


typedef struct{
    
    box_t header;
    
    uint8_t reserved[6];                //=0
    uint16_t dataRefIdx;
    
    
}sampleEntry_t;

typedef struct{
    
    sampleEntry_t sampleEntry;
    
    uint8_t *data;
    
}hintSampleEntry_t;

typedef struct{
    
    box_t header;
    
    uint32_t bufferSizeDB;
    uint32_t maxBitrate;
    uint32_t avgBitrate;
    
}bitRateBox_t;

typedef struct{
    
    sampleEntry_t sampleEntry;
    
}metaDataSampleEntry_t;

typedef struct{
    
    metaDataSampleEntry_t *metaDataSampleEntry;
    
    uint8_t *contentEncoding;
    uint8_t *nspace;
    uint8_t *schemaLocation;          //optional
    bitRateBox_t bitRateBox;         //optional
    
}xmlMetaDataSampleEntry_t;

typedef struct{
    
    metaDataSampleEntry_t metaDataSampleEntry;
    
    uint8_t *contentEncoding;           //optional
    uint8_t *mimeFormat;
    bitRateBox_t bitRateBox;           //optional
    
}textMetaDataSampleEntry_t;

typedef struct{
    
    box_t header;
    
    uint32_t hSpacing;
    uint32_t vSpacing;
    
}pixelAspectRatioBox_t;

typedef struct{
    
    box_t header;
    
    uint32_t cleanApertureWidthN;
    uint32_t cleanApertureWidthD;
    uint32_t cleanApertureHeightN;
    uint32_t cleanApertureHeightD;
    
    uint32_t horizOffN;
    uint32_t horizOffD;
    uint32_t vertOffN;
    uint32_t vertOffD;
    
    
}cleanApertureBox_t;


typedef struct{
    sampleEntry_t sampleEntry;
    
    uint16_t pre_defined;
    uint16_t reserved;
    uint32_t pre_defined_[3];
    uint16_t width;
    uint16_t height;
    uint32_t horizResolution;               //=0x00480000 72dpi
    uint32_t vertResolution;                //=0x00480000 72dpi
    
    uint32_t _reserved;
    uint16_t frameCount;                    //=1
    uint8_t  compressorname[32];
    uint16_t depth;                         //0x0018
    int16_t  predef;                                        //-1
    cleanApertureBox_t cleanApertureBox;                   //optional
    pixelAspectRatioBox_t pixelAspectRatioBox;             //optional
    
}visualSampleEntry_t;


typedef struct{
    
    sampleEntry_t *sampleEntry;
    
    uint32_t reserved[2];
    uint16_t channelcount;                  // 2
    uint16_t samplesize;                    // 16
    uint16_t predef;                        //-1
    uint16_t _reserved;
    uint32_t sampleRate;                    //{default samplerate of media}<<16
    
    
}audioSampleEntry_t;

typedef struct{
    fullbox_t   fullbox;
    
}vmhd_t;

// AVC specific boxes included in stsd

typedef struct{
    
    uint16_t sequenceParameterSetLength;
    uint8_t *parameterSet;
    
}sequenceParameterSetNalUnit_t;

typedef struct{
    
    uint16_t pictureParameterSetLength;
    uint8_t *pictureSet;
    
}pictureParameterSetNalUnit_t;

typedef struct{
    
    uint8_t configurationVesrion;                           //=1
    uint8_t AVCProfileIndication;
    uint8_t profileCompatibility;
    uint8_t AVCLevelIndication;
    uint8_t lengthSizeMinusOne;                             // high order 6 bits are 1: 111111SS size=SS+1 => (00000011 & 111111SS)+1
    uint8_t numOfSequenceParameterSets;                     // high order 3 bits are 1
    // depending on the numOfSequenceParameterSets
    sequenceParameterSetNalUnit_t *sequenceParameterSets;
    uint8_t numOfPictureParameterSets;
    pictureParameterSetNalUnit_t *pictureParameterSets;
}AVCDecoderConfigurationRecord_t;

typedef struct{
    
    box_t header;
    AVCDecoderConfigurationRecord_t AVCDecoderConfig;
    
    
}AVCConfigBox_t;

typedef struct{
    
    
    
}MPEG4ExtensionDesc_t;

typedef struct{
    
    visualSampleEntry_t entry;
    AVCConfigBox_t config;
    bitRateBox_t bitRate; /* optional */
    MPEG4ExtensionDesc_t MPEG4ExtensionDescriptors; /* optional */
    
}AVCSampleEntry_t;

// sample description, we will need this because it includes: codec types, initialization parameters and so on!!
typedef struct{
    fullbox_t fullbox;
    uint32_t entry_count;
}stsd_t;

typedef struct{
    fullbox_t fullbox;
    uint32_t entry_count;
    uint32_t *sampleCount;  // depends on entry_count
    uint32_t *sampleDelta;
}stts_t;

typedef struct{
    uint32_t first_chunk;
    uint32_t samples_per_chunk;
    uint32_t sample_description_index;
}stsc_entry_t;

typedef struct{
    fullbox_t fullbox;
    uint32_t entry_count;
    stsc_entry_t *entries;
}stsc_t;

typedef struct{
    fullbox_t fullbox;
    uint32_t entry_count;
    uint32_t *chunk_offsets;
}stco_t;

typedef struct{
    fullbox_t fullbox;
    uint32_t entry_count;
    uint64_t *chunk_offsets;
}co64_t;

typedef struct{
    fullbox_t fullbox;
    uint32_t sample_size;
    uint32_t sample_count;
    uint32_t *sample_sizes;
}stsz_t;

// sample table box...
typedef struct{
    box_t header;
    stsd_t *sampleDescription;
    stts_t *timeToSample;       // for decoding
    stsc_t *sampleToChunk;      // sample to chunk, partial data offset information
    stco_t *chunkOffset;        // chunk offset offset, --
}stbl_t;

// again a container
typedef struct{
    box_t header;
    dinf_t *dataInformation;
    stbl_t *sampleTable;
}minf_t;

// media headers ... very important box ...
typedef struct{
    box_t header;
    mdhd_t *mediaHeader;
    hdlr_t *handler;
}mdia_t;

typedef struct{
    box_t   header;
    tkhd_t  trackHeader;
}trak_t;

// the actual Movie Box, just has a header 
typedef struct{
    box_t header;
}moov_t;

/*----- Boxes contained in the Movie Fragment box/container
 *
 *  only mandatory ones
 */
  
// for segmented IBMFF we will need this!

typedef struct{
    fullbox_t fullbox;
    uint32_t sequenceNumber;
}mfhd_t;

typedef struct{
    fullbox_t fullbox;
    uint32_t trackId;
    uint64_t base_data_offset;
    uint32_t sample_description_index;
    uint32_t default_sample_duration;
    uint32_t default_sample_size;
    uint32_t default_sample_flags;
}tfhd_t;

typedef struct{
    fullbox_t fullbox;
    uint64_t decodeTime;
}tfdt_t;

typedef struct{
    box_t header;
}traf_t;


typedef struct{
    uint32_t sample_duration;
    uint32_t sample_size;
    uint32_t sample_flags;
    uint32_t sample_composition_time_offset;
}trun_entry_t;

typedef struct{
    fullbox_t fullbox;
    uint32_t sample_count;
    /* the next to fields are optional and their existance is determined by the flags in the header (full box) */
    int32_t data_offset;
    uint32_t first_sample_flags;
    /* existance determined by sample_count */
    trun_entry_t *entries;
}trun_t;

typedef struct{
    box_t header;
}moof_t;

typedef struct{
    box_t header;
}mvex_t;

typedef struct{
    fullbox_t fullbox;
    uint32_t trackId;
    uint32_t default_sample_description_index;
    uint32_t default_sample_duration;
    uint32_t default_sample_size;
    uint32_t default_sample_flags;
}trex_t;


/*----- Boxes contained in the Movie Fragment Random Access box/container
 *
 *  only mandatory ones
 */

typedef struct{
    fullbox_t header;
    uint32_t size;
}mfro_t;

typedef struct{
    box_t header;
    mfro_t *movieFragmentRAOffset;
}mfra_t;

/*
 * Boxes for object descriptors
 */

typedef struct{
    fullbox_t   fullbox;
    
}od_t;

#endif
