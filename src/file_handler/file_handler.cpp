#include "file_handler.h"


int r2fs_file_open(rtems_libio_t *iop, const char *pathname, int oflag, mode_t mode)
{
}

int r2fs_file_close(rtems_libio_t *iop)
{
}

ssize_t r2fs_file_read(rtems_libio_t *iop, void *buffer, size_t count)
{
}

ssize_t r2fs_file_write(rtems_libio_t *iop, const void *buffer, size_t count)
{
}

off_t r2fs_file_lseek(rtems_libio_t *iop, off_t offset, int whence)
{
}

int r2fs_file_ftruncate(rtems_libio_t *iop, off_t length)
{
}

int r2fs_file_fdatasync(rtems_libio_t *iop)
{
}

int r2fs_file_fstat(const rtems_filesystem_location_info_t *pathloc, struct stat *buf)
{
}


const rtems_filesystem_file_handlers_r r2fs_file_handlers = {
    .open_h = r2fs_file_open,
    .close_h = r2fs_file_close,
    .read_h = r2fs_file_read,
    .write_h = r2fs_file_write,
    .ioctl_h = rtems_filesystem_default_ioctl,
    .lseek_h = r2fs_file_lseek,
    .fstat_h = r2fs_file_fstat,
    .ftruncate_h = r2fs_file_ftruncate,
    .fsync_h = r2fs_file_fdatasync,
    .fdatasync_h = r2fs_file_fdatasync,
    .fcntl_h = rtems_filesystem_default_fcntl,
    .kqfilter_h = rtems_filesystem_default_kqfilter,
    .mmap_h = rtems_filesystem_default_mmap,
    .poll_h = rtems_filesystem_default_poll,
    .readv_h = rtems_filesystem_default_readv,
    .writev_h = rtems_filesystem_default_writev};
