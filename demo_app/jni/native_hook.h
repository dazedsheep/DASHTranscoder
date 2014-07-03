#ifndef _NATIVE_HOOK_H_
#define _NATIVE_HOOK_H_

#include <stdint.h>
#include <jni.h>
#include <string>
#include <android/log.h>
// some defs for logging

#define LOG_TAG     "native-mobileDashEncoder"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)


#endif