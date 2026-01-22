#ifndef _FILE_HANDLER_H_
#define _FILE_HANDLER_H_

#include <rtems/fs.h>

#include <utf8proc/utf8proc.h>

#include <sys/stat.h>


int r2fs_file_open(rtems_libio_t *iop, const char *pathname, int oflag, mode_t mode);

int r2fs_file_close(rtems_libio_t *iop);

ssize_t r2fs_file_read(rtems_libio_t *iop, void *buffer, size_t count);

ssize_t r2fs_file_write(rtems_libio_t *iop, const void *buffer, size_t count);

off_t r2fs_file_lseek(rtems_libio_t *iop, off_t offset, int whence);

int r2fs_file_ftruncate(rtems_libio_t *iop, off_t length);

int r2fs_file_fdatasync(rtems_libio_t *iop);

int r2fs_file_fstat(const rtems_filesystem_location_info_t *pathloc, struct stat *buf);


#endif
