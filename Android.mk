# File: Android.mk
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES:= StartServer.cpp
LOCAL_C_INCLUDES := $(JNI_H_INCLUDE)
LOCAL_SHARED_LIBRARIES := libutils libbinder libzksuservice liblog libcutils
LOCAL_PRELINK_MODULE := false
LOCAL_MODULE := startzksu

include $(BUILD_EXECUTABLE)

include $(call all-makefiles-under,$(LOCAL_PATH))