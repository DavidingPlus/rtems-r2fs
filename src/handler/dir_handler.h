#ifndef _DIR_HANDLER_H_
#define _DIR_HANDLER_H_

#include <rtems/fs.h>

#include <utf8proc/utf8proc.h>

#include <sys/stat.h>


int r2fsDirOpen(rtems_libio_t *iop, const char *path, int oflag, mode_t mode);

int r2fsDirClose(rtems_libio_t *iop);

ssize_t r2fsDirRead(rtems_libio_t *iop, void *buffer, size_t count);

int r2fsDirFstat(const rtems_filesystem_location_info_t *pathloc, struct stat *buf);


#endif
