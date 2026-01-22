#ifndef _FILE_HANDLER_H_
#define _FILE_HANDLER_H_

#include <rtems/fs.h>

#include <utf8proc/utf8proc.h>

#include <sys/stat.h>


int r2fsFileOpen(rtems_libio_t *iop, const char *pathname, int oflag, mode_t mode);

int r2fsFileClose(rtems_libio_t *iop);

ssize_t r2fsFileRead(rtems_libio_t *iop, void *buffer, size_t count);

ssize_t r2fsFileWrite(rtems_libio_t *iop, const void *buffer, size_t count);

off_t r2fsFileLseek(rtems_libio_t *iop, off_t offset, int whence);

int r2fsFileFtruncate(rtems_libio_t *iop, off_t length);

int r2fsFileFdatasync(rtems_libio_t *iop);

int r2fsFileFstat(const rtems_filesystem_location_info_t *pathloc, struct stat *buf);


#endif
