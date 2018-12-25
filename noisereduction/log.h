#include <stdarg.h>
#ifndef LOG_TAG
#define LOG_TAG "CloseliCoreService"
#endif /* ifndef LOG_TAG */

typedef enum {
    LOG_UNKNOWN = 0,
    LOG_DEFAULT,
    LOG_VERBOSE,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_OFF,
} LogLevel;

class Log
{
public:
    Log ();
    virtual ~Log ();
    static void SetLogLevel(int log_level);
    static int GetLogLevel();
    static int v(const char *log_tag, const char *format, ...);
    static int d(const char *log_tag, const char *format, ...);
    static int i(const char *log_tag, const char *format, ...);
    static int w(const char *log_tag, const char *format, ...);
    static int e(const char *log_tag, const char *format, ...);
private:
    static int log_print(int prio, const char *log_tag, const char *format, ...);
    static int log_vprint(int prio, const char *log_tag, const char *format, va_list ap);
    static int log_level_;

};
