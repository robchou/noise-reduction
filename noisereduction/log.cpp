#ifdef __ANDROID__
#include <android/log.h>
#else
#include <stdio.h>
#endif

#include "log.h"

Log::Log() {}

Log::~Log() {}

int Log::log_level_ = LOG_OFF;

void Log::SetLogLevel(int log_level) {
    log_level_ = log_level;
}

int Log::GetLogLevel() {
    return log_level_;
}

int Log::v(const char *log_tag, const char *format, ...) {

    int ret = 0;
    va_list ap;

    va_start(ap, format);
    ret = log_vprint(LOG_VERBOSE, log_tag, format, ap);
    va_end(ap);

    return ret;
}

int Log::d(const char *log_tag, const char *format, ...) {

    int ret = 0;
    va_list ap;

    va_start(ap, format);
    ret = log_vprint(LOG_DEBUG, log_tag, format, ap);
    va_end(ap);

    return ret;
}

int Log::i(const char *log_tag, const char *format, ...) {

    int ret = 0;
    va_list ap;

    va_start(ap, format);
    ret = log_vprint(LOG_INFO, log_tag, format, ap);
    va_end(ap);

    return ret;
}

int Log::w(const char *log_tag, const char *format, ...) {

    int ret = 0;
    va_list ap;

    va_start(ap, format);
    ret = log_vprint(LOG_WARN, log_tag, format, ap);
    va_end(ap);

    return ret;
}

int Log::e(const char *log_tag, const char *format, ...) {

    int ret = 0;
    va_list ap;

    va_start(ap, format);
    ret = log_vprint(LOG_ERROR, log_tag, format, ap);
    va_end(ap);

    return ret;
}

int Log::log_print(int prio, const char *log_tag, const char *format, ...) {

    if (prio < log_level_) {
        return 0;
    }

    int ret = 0;
    va_list ap;
    va_start(ap, format);
#ifdef __ANDROID__
    ret = __android_log_vprint(prio, log_tag, format, ap);
#else
    printf("%s: ", log_tag);
    ret = vprintf(format, ap);
#endif
    va_end(ap);
    return ret;
}

int Log::log_vprint(int prio, const char *log_tag, const char *format, va_list ap) {

    if (prio < log_level_) {
        return 0;
    }

#ifdef __ANDROID__
    return __android_log_vprint(prio, log_tag, format, ap);
#else
    printf("%s: ", log_tag);
    return vprintf(format, ap);
#endif
}
