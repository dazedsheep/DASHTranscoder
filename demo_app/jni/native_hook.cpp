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

#include "native_hook.h"
#include "mobileDashEncoder/include/defs.h"
#include "mobileDashEncoder/src/mobileDashEncoder.h"

// just to outline that this funcs are JNI funcs ...
#define JNIFNDEFINE(fname)  Java_itec_android_DASH_DASHEncoder_NativeInterface_##fname


// we are aiming for c++ so get rid of the sym extensions for exporting ...
extern "C" {
    JNIEXPORT jint JNICALL JNIFNDEFINE(dumbStreamToFile)(JNIEnv* env, jclass cl, jstring fileName, jobject inFD);
    JNIEXPORT jint JNICALL JNIFNDEFINE(startNativeMobileDashEncoder)(JNIEnv* env, jclass cl, jstring setupFile, jstring baseName, jobject inFD, jobject bitrate, jint fps, jint segmentLength);
    JNIEXPORT jint JNICALL JNIFNDEFINE(stopNativeMobileDashEncoder)(JNIEnv* env, jclass cl);
};

mobileDashEncoder *mde = NULL;

int32_t grepNativeFdFromJava(JNIEnv *env, jclass cl, jobject fd)
{
    jclass c;
    jfieldID fid;
    
    if(!(c = env->GetObjectClass(fd)) || !(fid = env->GetFieldID(c,"descriptor","I"))) return _ERR;
    
    return (int32_t)env->GetIntField(fd, fid);
    
}

JNIEXPORT jint JNICALL JNIFNDEFINE(dumbStreamToFile)(JNIEnv *env, jclass cl, jstring fileName, jobject inFD)
{
    int fd = grepNativeFdFromJava(env, cl, inFD);
    if(fd == -1) return _NATIVE_FD_ERROR;
    return 0;
}

JNIEXPORT jint JNICALL JNIFNDEFINE(startNativeMobileDashEncoder)(JNIEnv* env, jclass cl, jstring setupFile, jstring baseName, jobject inFD, jobject bitrate, jint fps, jint segmentLength)
{
    char const *str;
    const char *sbn;
    int _fps;
    int _segmentLength;
    int fd = grepNativeFdFromJava(env, cl, inFD);
    if(fd == -1) return _NATIVE_FD_ERROR;
    str = env->GetStringUTFChars(setupFile, 0);
    sbn = env->GetStringUTFChars(baseName, 0);
    // init our delivery and decision logic (transcoding included)
    mde = new mobileDashEncoder(fd, sbn, segmentLength, fps);
    mde->start(str);
    env->ReleaseStringUTFChars(setupFile, str);
    env->ReleaseStringUTFChars(baseName, sbn);
    return _NOERR;
}

JNIEXPORT jint JNICALL JNIFNDEFINE(stopNativeMobileDashEncoder)(JNIEnv* env, jclass cl)
{
    if(mde!=NULL) delete mde;    
}
