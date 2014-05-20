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

#if (defined WIN32 || WIN64)
#include <Windows.h>
#else
#include <pthread.h>
#endif

#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "log.h"
#include "fragmentedIsom.h"
#include "sysnet.h"

fragmentedIsom::fragmentedIsom(Context *ctx)
{
    ctx->getDict(MKSTRING(global.settings.filename), &this->baseName);
    ctx->getDict(MKSTRING(global.settings.segmentSizeInSeconds), &this->segmentSizeInSeconds);
    ctx->getDict(MKSTRING(global.settings.fps), &this->fps);
    this->ctx = ctx;
    this->segmentSize = segmentSizeInSeconds * fps;
    this->sequence = 1;
    samples = new std::vector<Sample *>();
    do_once = 0;
    this->frame_number = 0;
    this->parts = 0;
    createInitSegment();
}

fragmentedIsom::~fragmentedIsom()
{
    if(samples != NULL)
    {
        if(samples->size() > 0)
        {
            mde_log(MDE_LOG_DEBUG, "%s: there are still %d sample(s) in the buffer.\n ",__FILE__);
            std::vector<Sample*>::iterator nq;
            for(nq = samples->begin(); nq != samples->end(); ++nq) delete *nq;
        }
        delete samples;
    }
}

int32_t fragmentedIsom::createInitSegment()
{
    uint32_t mvhd_timescale;
    uint32_t width, height;
    dictionary inputParams;
    chunk *c;
    if(ctx->getStream()->getDict(MKSTRING(mvhd.timescale), &mvhd_timescale) < 0)
    {
        mde_log(MDE_LOG_DEBUG, "%s: could not find timescale for mvhd defaulting to %d.\n", __FILE__, MVHD_DEFAULT_TIMESCALE);
        mvhd_timescale = MVHD_DEFAULT_TIMESCALE;
    }
    
    //if(ctx->getStream()->getDict(MKSTRING(track.timescale), &this->timescale) < 0) this->timescale = 96; /* should get it from the access units ... */
    this->timescale = mvhd_timescale;
    
    std::string filename((char *)baseName);
    filename.append("init.mp4");
    this->file = new fdinout(filename.c_str(), true);
    /* write the segment init file with the all the necessary decoder information and so on */
    /* the access module for the codec will provide us with the necessary codec informations (hopefully) */
    
    IBox *ftyp = BoxFactory::create(ctx, FTYP_BOX);
    inputParams.addDict<uint32_t>(STRING("ftyp.major_brand"), MKTAG('i','s','o','m'));
    inputParams.addDict<uint32_t>(STRING("ftyp.minor_version"), 0);
    
    IBox *avcc = BoxFactory::create(ctx, AVCC_BOX);
    IBox *avc1 = BoxFactory::create(ctx, AVC1_BOX);
    avc1->addBox(avcc);
    
    IBox *stsd = BoxFactory::create(ctx, STSD_BOX);
    stsd->addBox(avc1);
    
    IBox *stts = BoxFactory::create(ctx, STTS_BOX);
    // IBox *stss = BoxFactory::create(ctx, STSS_BOX);
    IBox *stsz = BoxFactory::create(ctx, STSZ_BOX);
    IBox *stco = BoxFactory::create(ctx, STCO_BOX);
    IBox *stbl = BoxFactory::create(ctx, STBL_BOX);
    IBox *stsc = BoxFactory::create(ctx, STSC_BOX);
    
    stbl->addBox(stsd);
    stbl->addBox(stts);
    stbl->addBox(stsc);
    // stbl->addBox(stss);
    stbl->addBox(stsz);
    stbl->addBox(stco);
    
    IBox *dref = BoxFactory::create(ctx, DREF_BOX);
    inputParams.addDict<uint32_t>(STRING("dref.entry_count"), 1);
    IBox *url = BoxFactory::create(ctx, URL_BOX);
    dref->addBox(url);
    
    IBox *dinf = BoxFactory::create(ctx, DINF_BOX);
    dinf->addBox(dref);
    
    IBox *vmhd = BoxFactory::create(ctx, VMHD_BOX);
    
    IBox *minf = BoxFactory::create(ctx, MINF_BOX);
    minf->addBox(vmhd);
    minf->addBox(dinf);
    minf->addBox(stbl);
    
    IBox *hdlr = BoxFactory::create(ctx, HDLR_BOX);
    inputParams.addDict(STRING("hdlr.handler_type"), MKTAG('v','i','d','e'));
    inputParams.addDict(STRING("hdlr.name"), STRING("created with MDE - Copyright by Benjamin Rainer"), sys_strlen(STRING("created with MDE - Copyright by Benjamin Rainer")));
    
    IBox *mdhd = BoxFactory::create(ctx, MDHD_BOX);
    inputParams.addDict<uint64_t>(STRING("mdhd.creationTime"), (uint64_t) 0);
    inputParams.addDict<uint64_t>(STRING("mdhd.modificationTime"), (uint64_t) 0);
    inputParams.addDict<uint32_t>(STRING("mdhd.timescale"), (uint32_t) timescale);
    inputParams.addDict<uint64_t>(STRING("mdhd.duration"), (uint64_t) 0);
    inputParams.addDict<uint16_t>(STRING("mdhd.langCode"), (uint16_t) 0x55C4);
    
    IBox *mdia = BoxFactory::create(ctx, MDIA_BOX);
    mdia->addBox(mdhd);
    mdia->addBox(hdlr);
    mdia->addBox(minf);
    
    IBox *tkhd = BoxFactory::create(ctx, TKHD_BOX);
    inputParams.addDict<uint32_t>(STRING("tkhd.trackId"), 1);
    inputParams.addDict<uint64_t>(STRING("tkhd.creationTime"), (uint64_t) 1);
    inputParams.addDict<uint64_t>(STRING("tkhd.modificationTime"), (uint64_t) 1);
    inputParams.addDict<uint64_t>(STRING("tkhd.duration"), (uint64_t) 0);
    inputParams.addDict<int16_t>(STRING("tkhd.layer"), 0);
    inputParams.addDict<int16_t>(STRING("tkhd.alternate_group"), 0);
    inputParams.addDict<int16_t>(STRING("tkhd.volume"), 0);
    inputParams.addDict<uint8_t>(STRING("tkhd.hintFlag"), (uint8_t)0x01);
    /* just for now ... */
    ctx->getStream()->getDict(STRING("video.width"), &width);
    ctx->getStream()->getDict(STRING("video.height"), &height);
    /* .... */
    inputParams.addDict<uint32_t>(STRING("tkhd.width"), width);
    inputParams.addDict<uint32_t>(STRING("tkhd.height"), height);
       
    IBox *trak = BoxFactory::create(ctx, TRAK_BOX);
    trak->addBox(tkhd);
    trak->addBox(mdia);
    
    IBox *mvhd = BoxFactory::create(ctx, MVHD_BOX);
    inputParams.addDict<uint64_t>(STRING("mvhd.creationTime"), (uint64_t) 0);
    inputParams.addDict<uint64_t>(STRING("mvhd.modificationTime"), (uint64_t) 0);
    inputParams.addDict<uint32_t>(STRING("mvhd.timescale"), (uint32_t) mvhd_timescale);
    inputParams.addDict<uint64_t>(STRING("mvhd.duration"), (uint64_t) 0);
    inputParams.addDict<uint32_t>(STRING("mvhd.rate"), (uint32_t) 0x00010000);
    inputParams.addDict<int16_t>(STRING("mvhd.volume"), (int16_t) 0x0100);
    inputParams.addDict<uint32_t>(STRING("mvhd.nextTrackId"), 2);
    
    IBox *mvex = BoxFactory::create(ctx, MVEX_BOX);
    IBox *trex = BoxFactory::create(ctx, TREX_BOX);
    
    inputParams.addDict<uint32_t>(STRING("trex.trackId"), 1);
    inputParams.addDict<uint32_t>(STRING("trex.default_sample_description_index"), 1); /* this should be the same as in the sidx box for this track (use the trackId?) */
    inputParams.addDict<uint32_t>(STRING("trex.default_sample_duration"), timescale / fps);
    inputParams.addDict<uint32_t>(STRING("trex.default_sample_size"), 0);
    inputParams.addDict<uint32_t>(STRING("trex.default_sample_flags"), 0x00010000);
    
    IBox *moov = BoxFactory::create(ctx, MOOV_BOX);
    IBox *iods = BoxFactory::create(ctx, OD_BOX);
    
    moov->addBox(mvhd);
    moov->addBox(iods);
    mvex->addBox(trex);
    moov->addBox(mvex);
    moov->addBox(trak);
    
    c = ftyp->dumpBox(&inputParams);
    file->put(c->getPointer(), c->getSize());
    delete c;
    c = moov->dumpBox(&inputParams);
    file->put(c->getPointer(), c->getSize());
    delete c;
    
    delete file;
    delete ftyp;
    delete moov; /* will cleanup all included boxes */
    
    
    return 0;
}

Sample *fragmentedIsom::readSample()
{
    return NULL;
}

int32_t fragmentedIsom::writeSample(Sample *sample)
{
    int32_t rc = 1;
    if(sample == NULL)
    {
        mde_log(MDE_LOG_ERROR, "%s: got NULL sample.\n", __FILE__);
        return -1;
    }
    if(sample->data() == NULL)
    {
        mde_log(MDE_LOG_ERROR, "%s: got sample with empty data.\n", __FILE__);
        return -1;
    }
    /* count the frames ... */

    if(sample->getSampleType() == SAMPLE_IS_FRAME)
    {
        this->frame_number++;
    }else{
        this->parts++;
    }
    
    /* add the sample to our container */
    samples->push_back(sample);
    
    
    /* check if we have reached the given segment size */
    if(this->frame_number == this->segmentSize)
    {
        rc = end(1);
        this->frame_number=0;
         this->parts = 0;
        return rc;
    }
    return rc;
}

int32_t fragmentedIsom::end(uint32_t trackId)
{
	uint32_t width, height;
    std::string filename((char *)baseName);
    uint32_t datasize=0, i, dataoffset, rawdatasize=0;
    uint8_t sequenceNum[10];
    chunk *c;
    //if(ctx->getStream()->getDict(MKSTRING(track.timescale), &this->timescale) < 0) this->timescale = 96; /* should get it from the access units ... */
    sprintf((char*)sequenceNum, "%d", sequence);
    filename.append((char*)sequenceNum);
    filename.append(".m4s");
    for(i = 0; i < samples->size(); i++) rawdatasize += samples->at(i)->size();
    
    datasize = rawdatasize + sizeof(uint32_t)*samples->size(); /* do not forget the size of the header for each access unit ... */
        
    this->file = new fdinout(filename.c_str(), true);
    /* write or segment things */
    dictionary params;
    IBox *styp = BoxFactory::create(ctx, STYP_BOX);
    params.addDict<uint32_t>(STRING("styp.major_brand"), MKTAG('m','s','d','h'));
    params.addDict<uint32_t>(STRING("styp.minor_version"), 0);
    
    IBox *sidx = BoxFactory::create(ctx, SIDX_BOX);
    params.addDict<uint32_t>(STRING("sidx.refId"), 1);
    params.addDict<uint32_t>(STRING("sidx.timescale"), timescale);
    params.addDict<uint64_t>(STRING("sidx.earliestPresentationTime"), timescale * (sequence-1) * segmentSizeInSeconds);
    params.addDict<uint64_t>(STRING("sidx.firstOffset"), 0);
    params.addDict<uint16_t>(STRING("sidx.reference_count"), 1);
   
    params.addDict(MKSTRING(sidx.reference_counts[0].referenceType), 0);
    params.addDict(MKSTRING(sidx.reference_counts[0].subsegmentDuration), timescale * segmentSizeInSeconds);
    params.addDict(MKSTRING(sidx.reference_counts[0].startsWithSAP), 1);
    params.addDict(MKSTRING(sidx.reference_counts[0].SAPType), 0);
    params.addDict(MKSTRING(sidx.reference_counts[0].SAPDeltaTime), 0);
    
    IBox *mfhd = BoxFactory::create(ctx, MFHD_BOX);
    mde_log(MDE_LOG_DEBUG, "%s: setting sequence number in mfhd to %d. \n", __FILE__, sequence);
    params.addDict<uint32_t>(STRING("mfhd.sequenceNumber"), sequence);
   
    IBox *tfhd = BoxFactory::create(ctx, TFHD_BOX);
    params.addDict<uint32_t>(STRING("tfhd.trackId"), trackId);
    
    IBox *tfdt = BoxFactory::create(ctx, TFDT_BOX);
    params.addDict<uint64_t>(STRING("tfdt.decodeTime"), (uint64_t)(timescale * (sequence-1) * segmentSizeInSeconds));
    
    IBox *trun = BoxFactory::create(ctx, TRUN_BOX);
    params.addDict<uint32_t>(STRING("trun.sample_count"), samples->size());
    params.addDict<uint32_t>(STRING("trun.trackId"), trackId);
    
    IBox *traf = BoxFactory::create(ctx, TRAF_BOX);
    traf->addBox(tfhd);
    traf->addBox(tfdt);
    traf->addBox(trun);
    
    IBox *moof = BoxFactory::create(ctx, MOOF_BOX);
    moof->addBox(mfhd);
    moof->addBox(traf);
    
    IBox *mdat = BoxFactory::create(ctx, MDAT_BOX);
    params.addDict<uint32_t>(STRING("mdat.size"), datasize);
    
    dataoffset = moof->estimateSize(&params) + mdat->estimateSize(&params); /* the demuxer will concat the segments to a fragmented iso ... tell him where he can then find the data with implicitly adding the base data offset from the tfhd */
    
    params.addDict<uint32_t>(STRING("trun.offset"), dataoffset);
    params.addDict(MKSTRING(sidx.reference_counts[0].referenceSize), rawdatasize);
   
    ctx->getStream(trackId-1)->setSamples(samples);
    
    c = styp->dumpBox(&params);
    file->put(c->getPointer(), c->getSize());
    delete c;
    c = sidx->dumpBox(&params);
    file->put(c->getPointer(), c->getSize());
    delete c;
    c = moof->dumpBox(&params);   /* will dump all other boxes included in this box -> mfhd, traf -> tfhd, trun*/
    file->put(c->getPointer(), c->getSize());
    delete c;
    c = mdat->dumpBox(&params);
    file->put(c->getPointer(), c->getSize());
    delete c;
    uint32_t sampleSize;
    /* now the data */
    for(i = 0; i < samples->size(); i++)
    {
        sampleSize = htonl(samples->at(i)->data()->getSize());
        file->put((uint8_t *)&sampleSize, sizeof(uint32_t));
        file->put(samples->at(i)->data()->getPointer(), samples->at(i)->data()->getSize());
    }
    
    std::vector<Sample*>::iterator nq;
    for(nq = samples->begin(); nq != samples->end(); ++nq)
    {
        delete *nq;
    }
    
    /* cleanup? - Yep, this is the last station for samples, clear up our container for the next bunch of samples */
    samples->clear();
    mde_log(MDE_LOG_DEBUG, "%s: wrote isom segment, frames in segment: %d and continuous parts: %d, number %d, to file: %s.\n", __FILE__, this->frame_number, this->parts, sequence, filename.c_str());
    sequence++;
    
    delete styp;
    delete sidx;
    delete moof;
    delete mdat;
    delete file;
    
    /* HACK!!!!!! */
    if(do_once==0)
    {
    do_once = 1;
    int32_t nets = getNetworkInterfacesCount();
    if(nets == _ERR)
    {
        mde_log(MDE_LOG_DEBUG, "%s: error getting network interfaces.\n", __FILE__);
    }else
    {
        mde_log(MDE_LOG_DEBUG, "%s: found %d network interfaces.\n", __FILE__, nets);
        
        for( i = 0; i < (uint32_t)nets; i++)
        {
            uint8_t *addr = getIPAddress(i);
            uint8_t *name = getNetworkInterfaceName(i);
            if(addr != NULL)
            {
            /* set the begin time */
                time_t t;
                struct tm *timeinfo;
                time(&t);
                timeinfo = localtime(&t);
                
                strftime(currentTime, sizeof(currentTime), "%Y-%m-%dT%T", timeinfo);
                std::string fname((char *)baseName);
                fname.append((char *)addr);
                fname.append(".mpd");
                file = new fdinout(fname.c_str(), true);
                std::string mpd("<?xml version=\"1.0\"?> <MPD xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns=\"urn:mpeg:DASH:schema:MPD:2011\" xsi:schemaLocation=\"urn:mpeg:DASH:schema:MPD:2011 DASH-MPD.xsd\" type=\"dynamic\" minimumUpdatePeriod=\"PT2S\" timeShiftBufferDepth=\"PT30M\" availabilityStartTime=\"");
                mpd.append(currentTime);
                mpd.append("\" minBufferTime=\"PT10S\" profiles=\"urn:mpeg:dash:profile:isoff-live:2011\"> <BaseURL>http://");
                uint8_t buffy_width[10],buffy_height[10];
                mpd.append((char *)addr);
                mpd.append(":8080/live/</BaseURL><Period>                <!-- Video -->                <AdaptationSet mimeType=\"video/mp4\" codecs=\"avc1.4D401F\" frameRate=\"30000/1001\" segmentAlignment=\"true\" startWithSAP=\"1\">              <SegmentTemplate timescale=\"90000\" initialization=\"liveinit.mp4\" media=\"live$Number$.m4s\" duration=\"180000\" startNumber=\"");
                
               ctx->getStream()->getDict(STRING("video.width"), &width);
   			   ctx->getStream()->getDict(STRING("video.height"), &height);
               sprintf((char *)buffy_width, "%d", width);
               sprintf((char *)buffy_height, "%d", height);
                mpd.append("1\"> </SegmentTemplate><Representation id=\"v0\" width=\"");
                mpd.append((const char*)buffy_width);
                mpd.append("\" height=\"");
                mpd.append((const char*)buffy_height);
                mpd.append("\" bandwidth=\"1000000\"/></AdaptationSet></Period></MPD>");
                
                file->put((uint8_t *)mpd.c_str(), mpd.length());
                
                delete file;
                
                free(name);
                free(addr);
                
            }
        }
    
        
    }
    }
    return 0;
}
