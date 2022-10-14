LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE:= libobjmaterial
LOCAL_SRC_FILES := libs/libobjmaterial.a
include $(PREBUILT_STATIC_LIBRARY)
#include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
APP_DEBUG := false
APP_OPTIM := release
LOCAL_MODULE := samp
LOCAL_LDLIBS := -llog
LOCAL_STATIC_LIBRARIES := libobjmaterial
#include $(PREBUILT_STATIC_LIBRARY)
#include $(BUILD_STATIC_LIBRARY)

# samp
FILE_LIST := $(wildcard $(LOCAL_PATH)/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/game/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/net/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/util/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/game/RW/RenderWare.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/gui/*.cpp)

# --- vendor --- #
# RakNet
FILE_LIST += $(wildcard $(LOCAL_PATH)/vendor/raknet/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/vendor/raknet/SAMP/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/vendor/raknet/src/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/vendor/raknet/src/crypto/*.cpp)
# Libs
FILE_LIST += $(wildcard $(LOCAL_PATH)/vendor/inih/cpp/INIReader.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/vendor/inih/ini.c)
FILE_LIST += $(wildcard $(LOCAL_PATH)/vendor/imgui/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/vendor/hash/md5.cpp)

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

# RakNet
LOCAL_C_INCLUDES := $(wildcard $(LOCAL_PATH)/vendor/raknet)

# LOCAL_CFLAGS += -UNDEBUG
# clang:
LOCAL_CPPFLAGS := -w -s -fvisibility=hidden -pthread -Wall -fpack-struct=1 -O2 -std=c++14 -fexceptions -ferror-limit=5
# gcc TODO: deprecate using gcc, move to clang
# LOCAL_CPPFLAGS := -w -s -fvisibility=hidden -pthread -Wall -fpack-struct=1 -O2 -std=c++14 -fexceptions -ferror-limit=5

include $(BUILD_SHARED_LIBRARY)