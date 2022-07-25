HYCLONE_SERVERCALL2(connect, int, int)
HYCLONE_SERVERCALL0(disconnect)
HYCLONE_SERVERCALL2(register_image, void*, size_t)
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
HYCLONE_SERVERCALL1(fork, int)
HYCLONE_SERVERCALL3(create_port, int, const char*, size_t)
HYCLONE_SERVERCALL6(write_port_etc, int, int, const void*, size_t, unsigned int, unsigned long long)
HYCLONE_SERVERCALL2(find_port, const char*, size_t)
HYCLONE_SERVERCALL2(get_port_info, int, void*)
HYCLONE_SERVERCALL3(create_sem, int, const char*, size_t)
HYCLONE_SERVERCALL1(delete_sem, int)
HYCLONE_SERVERCALL0(get_system_sem_count)