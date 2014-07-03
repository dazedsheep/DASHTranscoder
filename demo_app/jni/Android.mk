LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE:= libnetutils
LOCAL_C_INCLUDES	:= $(LOCAL_PATH)
LOCAL_C_INCLUDES	+= $(LOCAL_PATH)/mobileDASHEncoder/include
LOCAL_SRC_FILES:= \
        mobileDashEncoder/src/android_libs/libnetutils/ifc_utils.cpp \

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE		:= mobileDASHEncoder
LOCAL_C_INCLUDES	:= $(LOCAL_PATH)
LOCAL_C_INCLUDES	+= $(LOCAL_PATH)/mobileDASHEncoder/include
LOCAL_SHARED_LIBRARIES := libnetutils
LOCAL_LDLIBS		:= -llog
LOCAL_SRC_FILES		:=	mobileDashEncoder/include/android/ifaddrs-android.cpp \
						mobileDashEncoder/src/transcode.cpp \
						mobileDashEncoder/src/inoutformat/fdinout.cpp \
						mobileDashEncoder/src/mobileDashEncoder.cpp \
						mobileDashEncoder/src/buffer/chunk.cpp \
						mobileDashEncoder/src/buffer/chunkBuffer.cpp \
						mobileDashEncoder/src/buffer/synchronizedBuffer.cpp \
						mobileDashEncoder/src/buffer/synchronizedFileBuffer.cpp \
						mobileDashEncoder/src/buffer/synchronizedObjectBuffer.cpp \
						mobileDashEncoder/src/coder/codecs.cpp \
						mobileDashEncoder/src/coder/AVC/avc.cpp \
						mobileDashEncoder/src/format/containers.cpp \
						mobileDashEncoder/src/format/demux.cpp \
						mobileDashEncoder/src/format/fragmentedIsom.cpp \
						mobileDashEncoder/src/format/isom.cpp \
						mobileDashEncoder/src/format/mux.cpp \
						mobileDashEncoder/src/format/Sample.cpp \
						mobileDashEncoder/src/format/IBMFF/AVC1Box.cpp \
						mobileDashEncoder/src/format/IBMFF/AVCCBox.cpp \
						mobileDashEncoder/src/format/IBMFF/boxFactory.cpp \
						mobileDashEncoder/src/format/IBMFF/boxReader.cpp \
						mobileDashEncoder/src/format/IBMFF/DINFBox.cpp \
						mobileDashEncoder/src/format/IBMFF/DREFBox.cpp \
						mobileDashEncoder/src/format/IBMFF/FTYPBox.cpp \
						mobileDashEncoder/src/format/IBMFF/HDLRBox.cpp \
						mobileDashEncoder/src/format/IBMFF/MDATBox.cpp \
						mobileDashEncoder/src/format/IBMFF/MDHDBox.cpp \
						mobileDashEncoder/src/format/IBMFF/MDIABox.cpp \
						mobileDashEncoder/src/format/IBMFF/MFHDBox.cpp \
						mobileDashEncoder/src/format/IBMFF/MINFbox.cpp \
						mobileDashEncoder/src/format/IBMFF/MOOFBox.cpp \
						mobileDashEncoder/src/format/IBMFF/MOOVBox.cpp \
						mobileDashEncoder/src/format/IBMFF/MVEXBox.cpp \
						mobileDashEncoder/src/format/IBMFF/MVHDBox.cpp \
						mobileDashEncoder/src/format/IBMFF/ODBox.cpp \
						mobileDashEncoder/src/format/IBMFF/SIDXBox.cpp \
						mobileDashEncoder/src/format/IBMFF/SSIXBox.cpp \
						mobileDashEncoder/src/format/IBMFF/STBLBox.cpp \
						mobileDashEncoder/src/format/IBMFF/STCOBox.cpp \
						mobileDashEncoder/src/format/IBMFF/STSDBox.cpp \
						mobileDashEncoder/src/format/IBMFF/STSCBox.cpp \
						mobileDashEncoder/src/format/IBMFF/STSSBox.cpp \
						mobileDashEncoder/src/format/IBMFF/STTSBox.cpp \
						mobileDashEncoder/src/format/IBMFF/STSZBox.cpp \
						mobileDashEncoder/src/format/IBMFF/STYPBox.cpp \
						mobileDashEncoder/src/format/IBMFF/TFDTBox.cpp \
						mobileDashEncoder/src/format/IBMFF/TFHDBox.cpp \
						mobileDashEncoder/src/format/IBMFF/TKHDBox.cpp \
						mobileDashEncoder/src/format/IBMFF/TRAFBox.cpp \
						mobileDashEncoder/src/format/IBMFF/TRAKBox.cpp \
						mobileDashEncoder/src/format/IBMFF/TREXBox.cpp \
						mobileDashEncoder/src/format/IBMFF/TRUNBox.cpp \
						mobileDashEncoder/src/format/IBMFF/URLBox.cpp \
						mobileDashEncoder/src/format/IBMFF/VMHDBox.cpp \
						mobileDashEncoder/src/sys/log.cpp \
						mobileDashEncoder/src/sys/sys.cpp \
						mobileDashEncoder/src/sys/inet/inet.cpp \
						mobileDashEncoder/src/threadsys/event.cpp \
						mobileDashEncoder/src/threadsys/mutex.cpp \
						mobileDashEncoder/src/threadsys/runable.cpp \
						mobileDashEncoder/src/threadsys/thread.cpp \
						mobileDashEncoder/src/threadsys/workerPool.cpp \
						mobileDashEncoder/src/utils/Context.cpp \
						mobileDashEncoder/src/utils/dictionary.cpp \
						mobileDashEncoder/src/utils/Stream.cpp
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := nativehook
LOCAL_SRC_FILES := native_hook.cpp
LOCAL_SHARED_LIBRARIES := libnetutils mobileDASHEncoder 
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include

LOCAL_LDLIBS := -L$(NDK_PLATFORMS_ROOT)/$(TARGET_PLATFORM)/arch-arm/usr/lib -L$(LOCAL_PATH) -llog -ljnigraphics -lz -ldl -lgcc

include $(BUILD_SHARED_LIBRARY)