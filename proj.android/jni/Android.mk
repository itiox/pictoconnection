LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/CustomMenuItemLabel.cpp \
                   ../../Classes/NavigationBar.cpp \
                   ../../Classes/PickThemeScene.cpp \
                   ../../Classes/PictoDatabase.cpp \
                   ../../Classes/PictoDefs.cpp \
                   ../../Classes/PictogramGalleryScene.cpp \
                   ../../Classes/PictogramGridScene.cpp \
                   ../../Classes/PictogramNode.cpp \
                   ../../Classes/PictogramObject.cpp \
                   ../../Classes/PictogramScene.cpp \
                   ../../Classes/PictoTheme.cpp \
                   ../../Classes/SettingsScene.cpp \
                   ../../Classes/sqlite3.c

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../../boost_1_55_0 \
					$(LOCAL_PATH)/../../../utf8_v2_3_4/source

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
