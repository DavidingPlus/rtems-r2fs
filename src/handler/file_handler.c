#include "file_handler.h"

#include "fs/inode.h"

#include <rtems/libio.h>


int r2fsFileOpen(rtems_libio_t *iop, const char *pathname, int oflag, mode_t mode)
{
}

int r2fsFileClose(rtems_libio_t *iop)
{
}

ssize_t r2fsFileRead(rtems_libio_t *iop, void *buffer, size_t count)
{
}

ssize_t r2fsFileWrite(rtems_libio_t *iop, const void *buffer, size_t count)
{
}

off_t r2fsFileLseek(rtems_libio_t *iop, off_t offset, int whence)
{
}

int r2fsFileFtruncate(rtems_libio_t *iop, off_t length)
{
}

int r2fsFileFdatasync(rtems_libio_t *iop)
{
}

int r2fsFileFstat(const rtems_filesystem_location_info_t *pathloc, struct stat *buf)
{
}


const rtems_filesystem_file_handlers_r r2fsFilehandlers = {
    .open_h = r2fsFileOpen,
    .close_h = r2fsFileClose,
    .read_h = r2fsFileRead,
    .write_h = r2fsFileWrite,
    .ioctl_h = rtems_filesystem_default_ioctl,
    .lseek_h = r2fsFileLseek,
    .fstat_h = r2fsFileFstat,
    .ftruncate_h = r2fsFileFtruncate,
    .fsync_h = r2fsFileFdatasync,
    .fdatasync_h = r2fsFileFdatasync,
    .fcntl_h = rtems_filesystem_default_fcntl,
    .poll_h = rtems_filesystem_default_poll,
    .kqfilter_h = rtems_filesystem_default_kqfilter,
    .readv_h = rtems_filesystem_default_readv,
    .writev_h = rtems_filesystem_default_writev,
    .mmap_h = rtems_filesystem_default_mmap,
};
