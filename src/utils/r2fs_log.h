#ifndef _R2FS_LOG_H_
#define _R2FS_LOG_H_


typedef enum r2fs_log_level
{
    R2FS_LOG_DEBUG,
    R2FS_LOG_INFO,
    R2FS_LOG_WARNING,
    R2FS_LOG_ERROR
} r2fs_log_level;

void r2fs_log_print(r2fs_log_level log_level, const char *funcname, unsigned int lineno, const char *fmt, ...);
void r2fs_log_errno(r2fs_log_level log_level, const char *funcname, unsigned int lineno, int err, const char *fmt, ...);

#define R2FS_LOG(log_level, format, ...) \
    r2fs_log_print(log_level, __func__, __LINE__, format, ##__VA_ARGS__)

#define R2FS_ERRNO_LOG(log_level, err, format, ...) \
    r2fs_log_errno(log_level, __func__, __LINE__, err, format, ##__VA_ARGS__)


#endif
