#include "dir_handler.h"


int r2fs_dir_open(rtems_libio_t *iop, const char *path, int oflag, mode_t mode)
{
}

int r2fs_dir_close(rtems_libio_t *iop)
{
}

ssize_t r2fs_dir_read(rtems_libio_t *iop, void *buffer, size_t count)
{
}

int r2fs_dir_fstat(const rtems_filesystem_location_info_t *pathloc, struct stat *buf)
{
}


const rtems_filesystem_file_handlers_r mixfs_dir_handlers = {
    .open_h = r2fs_dir_open,
    .close_h = r2fs_dir_close,
    .read_h = r2fs_dir_read,
    .write_h = rtems_filesystem_default_write,
    .ioctl_h = rtems_filesystem_default_ioctl,
    .lseek_h = rtems_filesystem_default_lseek_directory,
    .fstat_h = r2fs_dir_fstat,
    .ftruncate_h = rtems_filesystem_default_ftruncate,
    .fsync_h = rtems_filesystem_default_fsync_or_fdatasync_success,
    .fdatasync_h = rtems_filesystem_default_fsync_or_fdatasync_success,
    .fcntl_h = rtems_filesystem_default_fcntl,
    .kqfilter_h = rtems_filesystem_default_kqfilter,
    .mmap_h = rtems_filesystem_default_mmap,
    .poll_h = rtems_filesystem_default_poll,
    .readv_h = rtems_filesystem_default_readv,
    .writev_h = rtems_filesystem_default_writev};
