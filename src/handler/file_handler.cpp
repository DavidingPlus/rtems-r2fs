#include "file_handler.h"

#include "fs/fs_manager.h"
#include "fs/inode.h"
#include "utils/exception_handler.h"

#include <rtems/libio.h>


int r2fs::r2fs_file_open(rtems_libio_t *iop, const char *pathname, int oflag, mode_t mode)
{
    // 一个文件系统应该只有一个 fs_manager 单例，因此在注册文件系统的时候需要将这个单例指针绑定到 rtems_filesystem_mount_table_entry_tt 结构的 fs_info 成员上去。
    file_system_manager *fs_manager = R2FS_GET_FSMANAGER_STRUCTURE(&iop->pathinfo);

    try
    {
        rwlock_guard freeze_lg(fs_manager->get_fs_freeze_lock(), rwlock_guard::lock_type::rdlock);
        std::unique_lock<std::mutex> meta_lg(fs_manager->get_fs_meta_lock());
        fs_manager->check_state();

        try
        {
            // 路径处理
            std::string abs_path = path_helper::extract_abs_path(pathname);
            std::string dir_path = path_helper::extract_dir_path(abs_path);
            std::string file_name = path_helper::extract_file_name(abs_path);

            // 检查参数
            if (dir_path.length() == 0 || file_name.length() == 0 || (flags & 3) == 3)
            {
                errno = EINVAL;
                return -1;
            }

            // 查找父目录
            path_lookup_processor proc(fs_manager);
            proc.set_abs_path(dir_path);
            dentry_handle dir_dentry = proc.do_path_lookup();

            if (!dir_dentry.is_exist() || dir_dentry->get_type() != R2FS_FT_DIR)
            {
                errno = ENOENT;
                return -1;
            }

            // 查找 / 创建目标文件
            proc.set_rel_path(dir_dentry, file_name);
            dentry_store_pos target_pos_hint;
            dentry_handle target_dentry = proc.do_path_lookup(&target_pos_hint);

            // 如果目标文件不存在，查看是否有 O_CREATE 标志
            if (!target_dentry.is_exist())
            {
                // 如果有则创建文件
                if (oflag & O_CREAT)
                {
                    R2FS_LOG(R2FS_LOG_DEBUG, "creating file %s.", abs_path.c_str());
                    directory dir(dir_dentry, fs_manager);
                    target_dentry = dir.create(file_name, R2FS_FT_REG_FILE, &target_pos_hint);
                }
                else
                {
                    errno = ENOENT;
                    return -1;
                }
            }
            // 如果目标文件存在，但不是普通文件，返回错误
            else if (target_dentry->get_type() != R2FS_FT_REG_FILE)
            {
                errno = EISDIR;
                return -1;
            }

            // 此时目标文件一定存在，获得目标文件的file对象
            file_handle file = file_cache_helper(fs_manager->get_file_obj_cache()).get_file_obj(target_dentry->get_ino());

            // 分配opened_file结构，增加file和dentry的fd引用计数
            auto p_opened_file = std::make_shared<opened_file>(flags, file);

            // 分配fd
            R2FS_LOG(R2FS_LOG_DEBUG, "opening file %s.", abs_path.c_str());
            fd_array *fds = fs_manager->get_fd_array();
            int fd = fds->alloc_fd(p_opened_file);

            // 如果带有O_TRUNC标志，则清空文件内容
            if (flags & O_TRUNC)
            {
                /*
                 * 加锁顺序是file_op_lock->fs_meta_lock，此处必须先解锁fs_meta_lock
                 * 先解锁不会出问题，因为整个open操作已经在fs_meta_lock保护下全部原子地完成，
                 * 此处只是对该文件进行截断
                 */

                meta_lg.unlock();
                rwlock_guard file_op_lg(file->get_file_op_lock(), rwlock_guard::lock_type::wrlock);
                meta_lg.lock();
                if (file->truncate(0))
                    file.mark_dirty();
            }

            // （改动）交给 RTEMS 管理 fd
            iop->data0 = p_opened_file.get();
            iop->data1 = new std::shared_ptr<opened_file>(p_opened_file);

            // 记录打开标志（RTEMS 需要）
            iop->flags |= LIBIO_FLAGS_OPEN;

            return 0;
        }
        catch (const std::exception &e)
        {
            return exception_handler(fs_manager, e).convert_to_errno(true);
        }
    }
    catch (const std::exception &e)
    {
        return exception_handler(fs_manager, e).convert_to_errno();
    }
}

int r2fs::r2fs_file_close(rtems_libio_t *iop)
{
}

ssize_t r2fs::r2fs_file_read(rtems_libio_t *iop, void *buffer, size_t count)
{
}

ssize_t r2fs::r2fs_file_write(rtems_libio_t *iop, const void *buffer, size_t count)
{
}

off_t r2fs::r2fs_file_lseek(rtems_libio_t *iop, off_t offset, int whence)
{
}

int r2fs::r2fs_file_ftruncate(rtems_libio_t *iop, off_t length)
{
}

int r2fs::r2fs_file_fdatasync(rtems_libio_t *iop)
{
}

int r2fs::r2fs_file_fstat(const rtems_filesystem_location_info_t *pathloc, struct stat *buf)
{
}


const rtems_filesystem_file_handlers_r r2fs_file_handlers = {
    .open_h = r2fs::r2fs_file_open,
    .close_h = r2fs::r2fs_file_close,
    .read_h = r2fs::r2fs_file_read,
    .write_h = r2fs::r2fs_file_write,
    .ioctl_h = rtems_filesystem_default_ioctl,
    .lseek_h = r2fs::r2fs_file_lseek,
    .fstat_h = r2fs::r2fs_file_fstat,
    .ftruncate_h = r2fs::r2fs_file_ftruncate,
    .fsync_h = r2fs::r2fs_file_fdatasync,
    .fdatasync_h = r2fs::r2fs_file_fdatasync,
    .fcntl_h = rtems_filesystem_default_fcntl,
    .poll_h = rtems_filesystem_default_poll,
    .kqfilter_h = rtems_filesystem_default_kqfilter,
    .readv_h = rtems_filesystem_default_readv,
    .writev_h = rtems_filesystem_default_writev,
    .mmap_h = rtems_filesystem_default_mmap,
};
