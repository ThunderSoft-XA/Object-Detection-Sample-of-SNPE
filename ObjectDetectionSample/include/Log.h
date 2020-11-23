/**
 * Copyright (c) 2018 Thunder Software Technology Co., Ltd.
 * All rights, including trade secret rights, reserved.
 * <http://www.thundersoft.com/>.
 */

#ifndef INCLUDE_TSLOG_H_
#define INCLUDE_TSLOG_H_


#ifdef ANDROID_TARGET
//#include <android/log.h>
#ifndef _TAG
#define _TAG "FACE_SDK"
#endif
#ifndef LOGD
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, _TAG, __VA_ARGS__)
#endif
#ifndef LOGI
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,  _TAG, __VA_ARGS__)
#endif
#ifndef LOGE
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, _TAG, __VA_ARGS__)
#endif
#ifndef LOGW
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN,  _TAG, __VA_ARGS__)
#endif
#else

#ifndef LOGI
#define LOGI(...) printf(__VA_ARGS__); printf("\n");
#endif
#ifndef LOGD
#define LOGD(...) printf(__VA_ARGS__); printf("\n");
#endif
#ifndef LOGW
#define LOGW(...) printf(__VA_ARGS__); printf("\n");
#endif
#ifndef LOGE
#define LOGE(...) printf(__VA_ARGS__); printf("\n");
#endif

#endif

#ifdef ANDROID_TARGET

#ifndef LOGD_IF
#define LOGD_IF(cond, ...) \
    ( ( cond ) \
    ? ((void)LOGD(__VA_ARGS__)) \
    : (void)0 )
#endif

#else  // LINUX_TARGET

#ifndef LOGD_IF
#define LOGD_IF(cond, ...) \
    ( ( cond ) \
    ? ((void)printf(__VA_ARGS__)) \
    : (void) 0 )
#endif

#endif



#endif