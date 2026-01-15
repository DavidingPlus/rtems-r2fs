#ifndef _DIR_HANDLER_H_
#define _DIR_HANDLER_H_


namespace r2fs
{
    int r2fs_dir_open(rtems_libio_t *iop, const char *path, int oflag, mode_t mode);

    int r2fs_dir_close(rtems_libio_t *iop);

    ssize_t r2fs_dir_read(rtems_libio_t *iop, void *buffer, size_t count);

    int r2fs_dir_fstat(const rtems_filesystem_location_info_t *pathloc, struct stat *buf);
}


#endif
