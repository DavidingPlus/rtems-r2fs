#ifndef _LOG_H_
#define _LOG_H_


typedef enum R2fsLogLevel
{
    R2FS_LOG_DEBUG,
    R2FS_LOG_INFO,
    R2FS_LOG_WARNING,
    R2FS_LOG_ERROR
} R2fsLogLevel;

void r2fsLogPrint(R2fsLogLevel log_level, const char *funcname, unsigned int lineno, const char *fmt, ...);
void r2fsLogErrno(R2fsLogLevel log_level, const char *funcname, unsigned int lineno, int err, const char *fmt, ...);

#define R2FS_LOG(log_level, format, ...) \
    r2fsLogPrint(log_level, __func__, __LINE__, format, ##__VA_ARGS__)

#define R2FS_ERRNO_LOG(log_level, err, format, ...) \
    r2fsLogErrno(log_level, __func__, __LINE__, err, format, ##__VA_ARGS__)


#endif
