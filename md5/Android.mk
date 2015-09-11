LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES:= md5c.c
LOCAL_C_INCLUDES := $(JNI_H_INCLUDE)
LOCAL_PRELINK_MODULE := false
LOCAL_MODULE := libzkmd5

include $(BUILD_SHARED_LIBRARY)

include $(call all-makefiles-under,$(LOCAL_PATH))