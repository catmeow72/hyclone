#include "export.h"
#include "linux_debug.h"

#define SYSCALL_STUB(name)      \
    MONIKA_EXPORT void name()   \
    {                           \
        panic("stub: " #name);  \
    }

extern "C"
{

//SYSCALL_STUB(_kern_accept)
//SYSCALL_STUB(_kern_access)
SYSCALL_STUB(_kern_acquire_sem)
SYSCALL_STUB(_kern_acquire_sem_etc)
SYSCALL_STUB(_kern_analyze_scheduling)
//SYSCALL_STUB(_kern_area_for)
//SYSCALL_STUB(_kern_bind)
SYSCALL_STUB(_kern_block_thread)
SYSCALL_STUB(_kern_cancel_thread)
SYSCALL_STUB(_kern_change_root)
SYSCALL_STUB(_kern_clear_caches)
SYSCALL_STUB(_kern_clear_debugger_breakpoint)
SYSCALL_STUB(_kern_clone_area)
//SYSCALL_STUB(_kern_close)
SYSCALL_STUB(_kern_close_port)
//SYSCALL_STUB(_kern_connect)
//SYSCALL_STUB(_kern_cpu_enabled)
//SYSCALL_STUB(_kern_create_area)
SYSCALL_STUB(_kern_create_child_partition)
//SYSCALL_STUB(_kern_create_dir)
SYSCALL_STUB(_kern_create_dir_entry_ref)
//SYSCALL_STUB(_kern_create_fifo)
//SYSCALL_STUB(_kern_create_index)
//SYSCALL_STUB(_kern_create_link)
//SYSCALL_STUB(_kern_create_pipe)
//SYSCALL_STUB(_kern_create_port)
//SYSCALL_STUB(_kern_create_sem)
//SYSCALL_STUB(_kern_create_symlink)
SYSCALL_STUB(_kern_create_timer)
//SYSCALL_STUB(_kern_debug_output)
SYSCALL_STUB(_kern_debug_thread)
//SYSCALL_STUB(_kern_debugger)
SYSCALL_STUB(_kern_defragment_partition)
//SYSCALL_STUB(_kern_delete_area)
SYSCALL_STUB(_kern_delete_child_partition)
//SYSCALL_STUB(_kern_delete_port)
//SYSCALL_STUB(_kern_delete_sem)
SYSCALL_STUB(_kern_delete_timer)
SYSCALL_STUB(_kern_disable_debugger)
//SYSCALL_STUB(_kern_dup)
//SYSCALL_STUB(_kern_dup2)
SYSCALL_STUB(_kern_entry_ref_to_path)
SYSCALL_STUB(_kern_estimate_max_scheduling_latency)
//SYSCALL_STUB(_kern_exec)
//SYSCALL_STUB(_kern_exit_team)
//SYSCALL_STUB(_kern_exit_thread)
//SYSCALL_STUB(_kern_fcntl)
SYSCALL_STUB(_kern_find_area)
SYSCALL_STUB(_kern_find_disk_device)
SYSCALL_STUB(_kern_find_disk_system)
SYSCALL_STUB(_kern_find_file_disk_device)
SYSCALL_STUB(_kern_find_partition)
//SYSCALL_STUB(_kern_find_port)
//SYSCALL_STUB(_kern_find_thread)
//SYSCALL_STUB(_kern_flock)
//SYSCALL_STUB(_kern_fork)
SYSCALL_STUB(_kern_frame_buffer_update)
//SYSCALL_STUB(_kern_fsync)
SYSCALL_STUB(_kern_generic_syscall)
//SYSCALL_STUB(_kern_get_area_info)
SYSCALL_STUB(_kern_get_clock)
//SYSCALL_STUB(_kern_get_cpu_info)
//SYSCALL_STUB(_kern_get_cpu_topology_info)
SYSCALL_STUB(_kern_get_cpuid)
SYSCALL_STUB(_kern_get_current_team)
SYSCALL_STUB(_kern_get_disk_device_data)
SYSCALL_STUB(_kern_get_disk_system_info)
SYSCALL_STUB(_kern_get_extended_team_info)
SYSCALL_STUB(_kern_get_file_disk_device_path)
//SYSCALL_STUB(_kern_get_image_info)
SYSCALL_STUB(_kern_get_memory_properties)
//SYSCALL_STUB(_kern_get_next_area_info)
SYSCALL_STUB(_kern_get_next_disk_device_id)
SYSCALL_STUB(_kern_get_next_disk_system_info)
SYSCALL_STUB(_kern_get_next_fd_info)
//SYSCALL_STUB(_kern_get_next_image_info)
SYSCALL_STUB(_kern_get_next_port_info)
SYSCALL_STUB(_kern_get_next_sem_info)
SYSCALL_STUB(_kern_get_next_socket_stat)
SYSCALL_STUB(_kern_get_next_team_info)
SYSCALL_STUB(_kern_get_next_thread_info)
//SYSCALL_STUB(_kern_get_port_info)
SYSCALL_STUB(_kern_get_port_message_info_etc)
SYSCALL_STUB(_kern_get_real_time_clock_is_gmt)
SYSCALL_STUB(_kern_get_safemode_option)
SYSCALL_STUB(_kern_get_scheduler_mode)
SYSCALL_STUB(_kern_get_sem_count)
SYSCALL_STUB(_kern_get_sem_info)
//SYSCALL_STUB(_kern_get_system_info)
SYSCALL_STUB(_kern_get_team_info)
//SYSCALL_STUB(_kern_get_team_usage_info)
//SYSCALL_STUB(_kern_get_thread_info)
SYSCALL_STUB(_kern_get_timer)
//SYSCALL_STUB(_kern_get_timezone)
//SYSCALL_STUB(_kern_getcwd)
//SYSCALL_STUB(_kern_getgid)
//SYSCALL_STUB(_kern_getgroups)
//SYSCALL_STUB(_kern_getpeername)
//SYSCALL_STUB(_kern_getrlimit)
//SYSCALL_STUB(_kern_getsockname)
//SYSCALL_STUB(_kern_getsockopt)
//SYSCALL_STUB(_kern_getuid)
SYSCALL_STUB(_kern_has_data)
//SYSCALL_STUB(_kern_image_relocated)
SYSCALL_STUB(_kern_initialize_partition)
SYSCALL_STUB(_kern_install_default_debugger)
SYSCALL_STUB(_kern_install_team_debugger)
//SYSCALL_STUB(_kern_ioctl)
//SYSCALL_STUB(_kern_is_computer_on)
SYSCALL_STUB(_kern_kernel_debugger)
SYSCALL_STUB(_kern_kill_team)
SYSCALL_STUB(_kern_kill_thread)
SYSCALL_STUB(_kern_ktrace_output)
//SYSCALL_STUB(_kern_listen)
//SYSCALL_STUB(_kern_load_image)
//SYSCALL_STUB(_kern_loading_app_failed)
SYSCALL_STUB(_kern_lock_node)
//SYSCALL_STUB(_kern_map_file)
SYSCALL_STUB(_kern_memory_advice)
//SYSCALL_STUB(_kern_mlock)
SYSCALL_STUB(_kern_mount)
SYSCALL_STUB(_kern_move_partition)
SYSCALL_STUB(_kern_munlock)
//SYSCALL_STUB(_kern_mutex_lock)
SYSCALL_STUB(_kern_mutex_sem_acquire)
SYSCALL_STUB(_kern_mutex_sem_release)
//SYSCALL_STUB(_kern_mutex_switch_lock)
//SYSCALL_STUB(_kern_mutex_unlock)
SYSCALL_STUB(_kern_next_device)
//SYSCALL_STUB(_kern_normalize_path)
//SYSCALL_STUB(_kern_open)
SYSCALL_STUB(_kern_open_attr)
//SYSCALL_STUB(_kern_open_attr_dir)
//SYSCALL_STUB(_kern_open_dir)
//SYSCALL_STUB(_kern_open_dir_entry_ref)
//SYSCALL_STUB(_kern_open_entry_ref)
SYSCALL_STUB(_kern_open_index_dir)
SYSCALL_STUB(_kern_open_parent_dir)
SYSCALL_STUB(_kern_open_query)
//SYSCALL_STUB(_kern_poll)
//SYSCALL_STUB(_kern_port_buffer_size_etc)
SYSCALL_STUB(_kern_port_count)
SYSCALL_STUB(_kern_preallocate)
//SYSCALL_STUB(_kern_process_info)
//SYSCALL_STUB(_kern_read)
SYSCALL_STUB(_kern_read_attr)
//SYSCALL_STUB(_kern_read_dir)
//SYSCALL_STUB(_kern_read_fs_info)
//SYSCALL_STUB(_kern_read_index_stat)
SYSCALL_STUB(_kern_read_kernel_image_symbols)
//SYSCALL_STUB(_kern_read_link)
//SYSCALL_STUB(_kern_read_port_etc)
//SYSCALL_STUB(_kern_read_stat)
//SYSCALL_STUB(_kern_readv)
SYSCALL_STUB(_kern_realtime_sem_close)
SYSCALL_STUB(_kern_realtime_sem_get_value)
//SYSCALL_STUB(_kern_realtime_sem_open)
SYSCALL_STUB(_kern_realtime_sem_post)
SYSCALL_STUB(_kern_realtime_sem_unlink)
SYSCALL_STUB(_kern_realtime_sem_wait)
SYSCALL_STUB(_kern_receive_data)
//SYSCALL_STUB(_kern_recv)
//SYSCALL_STUB(_kern_recvfrom)
SYSCALL_STUB(_kern_recvmsg)
SYSCALL_STUB(_kern_register_file_device)
//SYSCALL_STUB(_kern_register_image)
SYSCALL_STUB(_kern_register_messaging_service)
SYSCALL_STUB(_kern_register_syslog_daemon)
SYSCALL_STUB(_kern_release_sem)
SYSCALL_STUB(_kern_release_sem_etc)
SYSCALL_STUB(_kern_remove_attr)
//SYSCALL_STUB(_kern_remove_dir)
SYSCALL_STUB(_kern_remove_index)
SYSCALL_STUB(_kern_remove_team_debugger)
//SYSCALL_STUB(_kern_rename)
SYSCALL_STUB(_kern_rename_attr)
SYSCALL_STUB(_kern_rename_thread)
SYSCALL_STUB(_kern_repair_partition)
//SYSCALL_STUB(_kern_reserve_address_range)
//SYSCALL_STUB(_kern_resize_area)
SYSCALL_STUB(_kern_resize_partition)
SYSCALL_STUB(_kern_restore_signal_frame)
//SYSCALL_STUB(_kern_resume_thread)
//SYSCALL_STUB(_kern_rewind_dir)
//SYSCALL_STUB(_kern_seek)
//SYSCALL_STUB(_kern_select)
//SYSCALL_STUB(_kern_send)
SYSCALL_STUB(_kern_send_data)
//SYSCALL_STUB(_kern_send_signal)
SYSCALL_STUB(_kern_sendmsg)
//SYSCALL_STUB(_kern_sendto)
//SYSCALL_STUB(_kern_set_area_protection)
SYSCALL_STUB(_kern_set_clock)
SYSCALL_STUB(_kern_set_cpu_enabled)
SYSCALL_STUB(_kern_set_debugger_breakpoint)
//SYSCALL_STUB(_kern_set_memory_protection)
SYSCALL_STUB(_kern_set_partition_content_name)
SYSCALL_STUB(_kern_set_partition_content_parameters)
SYSCALL_STUB(_kern_set_partition_name)
SYSCALL_STUB(_kern_set_partition_parameters)
SYSCALL_STUB(_kern_set_partition_type)
//SYSCALL_STUB(_kern_set_port_owner)
SYSCALL_STUB(_kern_set_real_time_clock)
SYSCALL_STUB(_kern_set_real_time_clock_is_gmt)
SYSCALL_STUB(_kern_set_scheduler_mode)
SYSCALL_STUB(_kern_set_sem_owner)
//SYSCALL_STUB(_kern_set_signal_mask)
//SYSCALL_STUB(_kern_set_signal_stack)
//SYSCALL_STUB(_kern_set_thread_priority)
//SYSCALL_STUB(_kern_set_timer)
SYSCALL_STUB(_kern_set_timezone)
//SYSCALL_STUB(_kern_setcwd)
SYSCALL_STUB(_kern_setgroups)
//SYSCALL_STUB(_kern_setpgid)
SYSCALL_STUB(_kern_setregid)
SYSCALL_STUB(_kern_setreuid)
//SYSCALL_STUB(_kern_setrlimit)
//SYSCALL_STUB(_kern_setsid)
//SYSCALL_STUB(_kern_setsockopt)
//SYSCALL_STUB(_kern_shutdown)
SYSCALL_STUB(_kern_shutdown_socket)
//SYSCALL_STUB(_kern_sigaction)
SYSCALL_STUB(_kern_sigpending)
SYSCALL_STUB(_kern_sigsuspend)
SYSCALL_STUB(_kern_sigwait)
//SYSCALL_STUB(_kern_snooze_etc)
SYSCALL_STUB(_kern_sockatmark)
//SYSCALL_STUB(_kern_socket)
//SYSCALL_STUB(_kern_socketpair)
//SYSCALL_STUB(_kern_spawn_thread)
SYSCALL_STUB(_kern_start_watching)
SYSCALL_STUB(_kern_start_watching_disks)
SYSCALL_STUB(_kern_start_watching_system)
//SYSCALL_STUB(_kern_stat_attr)
SYSCALL_STUB(_kern_stop_notifying)
SYSCALL_STUB(_kern_stop_watching)
SYSCALL_STUB(_kern_stop_watching_disks)
SYSCALL_STUB(_kern_stop_watching_system)
//SYSCALL_STUB(_kern_suspend_thread)
SYSCALL_STUB(_kern_switch_sem)
SYSCALL_STUB(_kern_switch_sem_etc)
SYSCALL_STUB(_kern_sync)
SYSCALL_STUB(_kern_sync_memory)
SYSCALL_STUB(_kern_system_profiler_next_buffer)
SYSCALL_STUB(_kern_system_profiler_recorded)
SYSCALL_STUB(_kern_system_profiler_start)
SYSCALL_STUB(_kern_system_profiler_stop)
SYSCALL_STUB(_kern_system_time)
//SYSCALL_STUB(_kern_thread_yield)
SYSCALL_STUB(_kern_transfer_area)
SYSCALL_STUB(_kern_unblock_thread)
SYSCALL_STUB(_kern_unblock_threads)
SYSCALL_STUB(_kern_uninitialize_partition)
//SYSCALL_STUB(_kern_unlink)
SYSCALL_STUB(_kern_unlock_node)
//SYSCALL_STUB(_kern_unmap_memory)
SYSCALL_STUB(_kern_unmount)
SYSCALL_STUB(_kern_unregister_file_device)
//SYSCALL_STUB(_kern_unregister_image)
SYSCALL_STUB(_kern_unregister_messaging_service)
//SYSCALL_STUB(_kern_unreserve_address_range)
//SYSCALL_STUB(_kern_wait_for_child)
SYSCALL_STUB(_kern_wait_for_debugger)
SYSCALL_STUB(_kern_wait_for_objects)
SYSCALL_STUB(_kern_wait_for_team)
//SYSCALL_STUB(_kern_wait_for_thread)
SYSCALL_STUB(_kern_wait_for_thread_etc)
//SYSCALL_STUB(_kern_write)
SYSCALL_STUB(_kern_write_attr)
SYSCALL_STUB(_kern_write_fs_info)
//SYSCALL_STUB(_kern_write_port_etc)
//SYSCALL_STUB(_kern_write_stat)
//SYSCALL_STUB(_kern_writev)
SYSCALL_STUB(_kern_writev_port_etc)
SYSCALL_STUB(_kern_xsi_msgctl)
SYSCALL_STUB(_kern_xsi_msgget)
SYSCALL_STUB(_kern_xsi_msgrcv)
SYSCALL_STUB(_kern_xsi_msgsnd)
SYSCALL_STUB(_kern_xsi_semctl)
SYSCALL_STUB(_kern_xsi_semget)
SYSCALL_STUB(_kern_xsi_semop)

}