HYCLONE_SERVERCALL2(connect, int, int)
HYCLONE_SERVERCALL0(disconnect)
HYCLONE_SERVERCALL2(register_image, void*, size_t)
HYCLONE_SERVERCALL3(get_image_info, int, void*, size_t)
HYCLONE_SERVERCALL4(get_next_image_info, int, int*, void*, size_t)
HYCLONE_SERVERCALL1(unregister_image, int)
HYCLONE_SERVERCALL1(image_relocated, int)
HYCLONE_SERVERCALL1(shutdown, bool)
HYCLONE_SERVERCALL1(register_area, void*)
HYCLONE_SERVERCALL2(get_area_info, int, void*)
HYCLONE_SERVERCALL1(unregister_area, int)
HYCLONE_SERVERCALL2(set_area_protection, int, unsigned int)
HYCLONE_SERVERCALL2(resize_area, int, size_t)
HYCLONE_SERVERCALL1(area_for, void*)
HYCLONE_SERVERCALL2(unmap_memory, void*, size_t)
HYCLONE_SERVERCALL1(fork, int)
HYCLONE_SERVERCALL3(create_port, int, const char*, size_t)
HYCLONE_SERVERCALL1(delete_port, int)
HYCLONE_SERVERCALL2(find_port, const char*, size_t)
HYCLONE_SERVERCALL2(get_port_info, int, void*)
HYCLONE_SERVERCALL2(set_port_owner, int, int)
HYCLONE_SERVERCALL6(read_port_etc, int, int*, void*, size_t, unsigned int, unsigned long long)
HYCLONE_SERVERCALL6(write_port_etc, int, int, const void*, size_t, unsigned int, unsigned long long)
HYCLONE_SERVERCALL3(create_sem, int, const char*, size_t)
HYCLONE_SERVERCALL1(delete_sem, int)
HYCLONE_SERVERCALL0(get_system_sem_count)
HYCLONE_SERVERCALL2(read_fs_info, int, void*)
HYCLONE_SERVERCALL2(get_thread_info, int, void*)
HYCLONE_SERVERCALL1(register_thread_info, void*)
HYCLONE_SERVERCALL1(suspend_thread, int)
HYCLONE_SERVERCALL1(resume_thread, int)
HYCLONE_SERVERCALL0(wait_for_resume)
HYCLONE_SERVERCALL1(notify_loading_app, int)
HYCLONE_SERVERCALL1(wait_for_app_load, int)
//HYCLONE_SERVERCALL2(debug_process_nub_message, int, void*)