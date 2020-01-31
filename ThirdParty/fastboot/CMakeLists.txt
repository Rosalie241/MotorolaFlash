#
# libfastboot CMakeLists.txt
#
project(fastboot)

# required
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# required dep
find_package(OpenSSL REQUIRED)

# zlib
add_subdirectory(zlib)

set(SOURCES 
    # libfastboot
    android_system_core/fastboot/bootimg_utils.cpp
    android_system_core/fastboot/fastboot.cpp
    android_system_core/fastboot/fs.cpp
    android_system_core/fastboot/socket.cpp
    android_system_core/fastboot/tcp.cpp
    android_system_core/fastboot/udp.cpp
    android_system_core/fastboot/util.cpp
    android_system_core/fastboot/fastboot_driver.cpp
    # libbase
    android_system_core/base/chrono_utils.cpp
    android_system_core/base/file.cpp
    android_system_core/base/logging.cpp
    android_system_core/base/mapped_file.cpp
    android_system_core/base/parsenetaddress.cpp
    android_system_core/base/properties.cpp
    android_system_core/base/quick_exit.cpp
    android_system_core/base/stringprintf.cpp
    android_system_core/base/strings.cpp
    android_system_core/base/threads.cpp
    android_system_core/base/test_utils.cpp
    # libziparchive
    android_system_core/libziparchive/zip_archive.cc
    android_system_core/libziparchive/zip_archive_stream_entry.cc
    android_system_core/libziparchive/zip_writer.cc
    # libcutils
    android_system_core/libcutils/config_utils.cpp
    android_system_core/libcutils/canned_fs_config.cpp
    android_system_core/libcutils/iosched_policy.cpp
    android_system_core/libcutils/load_file.cpp
    android_system_core/libcutils/native_handle.cpp
    android_system_core/libcutils/record_stream.cpp
    android_system_core/libcutils/sockets.cpp
    android_system_core/libcutils/strdup16to8.cpp
    android_system_core/libcutils/strdup8to16.cpp
    android_system_core/libcutils/strlcpy.c
    android_system_core/libcutils/threads.cpp
    # liblp
    android_system_core/fs_mgr/liblp/builder.cpp
    android_system_core/fs_mgr/liblp/images.cpp
    android_system_core/fs_mgr/liblp/partition_opener.cpp
    android_system_core/fs_mgr/liblp/reader.cpp
    android_system_core/fs_mgr/liblp/utility.cpp
    android_system_core/fs_mgr/liblp/writer.cpp
    # liblog
    android_system_core/liblog/config_read.cpp
    android_system_core/liblog/config_write.cpp
    android_system_core/liblog/log_event_list.cpp
    android_system_core/liblog/log_event_write.cpp
    android_system_core/liblog/logger_lock.cpp
    android_system_core/liblog/logger_name.cpp
    android_system_core/liblog/logger_read.cpp
    android_system_core/liblog/logger_write.cpp
    android_system_core/liblog/logprint.cpp
    android_system_core/liblog/stderr_write.cpp
    android_system_core/liblog/fake_log_device.cpp
    android_system_core/liblog/fake_writer.cpp
    # libsparse
    android_system_core/libsparse/backed_block.cpp
    android_system_core/libsparse/output_file.cpp
    android_system_core/libsparse/sparse.cpp
    android_system_core/libsparse/sparse_crc32.cpp
    android_system_core/libsparse/sparse_err.cpp
    android_system_core/libsparse/sparse_read.cpp
    # diagnose_usb
    android_system_core/diagnose_usb/diagnose_usb.cpp
    # libext4_utils
    android_system_extras/ext4_utils/ext4_utils.cpp
    android_system_extras/ext4_utils/wipe.cpp
    android_system_extras/ext4_utils/ext4_sb.cpp
)

if(WIN32)
    list(APPEND SOURCES 
        android_system_core/fastboot/usb_windows.cpp
        android_system_core/base/errors_windows.cpp
        android_system_core/base/utf8.cpp
        android_system_core/libcutils/socket_inaddr_any_server_windows.cpp
        android_system_core/libcutils/socket_network_client_windows.cpp
        android_system_core/libcutils/sockets_windows.cpp
        android_system_core/libcutils/trace-host.cpp
    )
else()
    list(APPEND SOURCES 
        android_system_core/fastboot/usb_linux.cpp
        android_system_core/base/errors_unix.cpp
        android_system_core/base/cmsg.cpp
        android_system_core/libcutils/ashmem-host.cpp
        android_system_core/libcutils/fs_config.cpp
        android_system_core/libcutils/trace-host.cpp
        android_system_core/libcutils/fs.cpp
        android_system_core/libcutils/hashmap.cpp
        android_system_core/libcutils/multiuser.cpp
        android_system_core/libcutils/socket_inaddr_any_server_unix.cpp
        android_system_core/libcutils/socket_local_client_unix.cpp
        android_system_core/libcutils/socket_local_server_unix.cpp
        android_system_core/libcutils/socket_network_client_unix.cpp
        android_system_core/libcutils/sockets_unix.cpp
        android_system_core/libcutils/str_parms.cpp
    )
endif(WIN32)

add_library(fastboot STATIC ${SOURCES})

# set include directories
target_include_directories(fastboot PRIVATE
    android_system_core/mkbootimg/include/bootimg/
    android_system_core/base/include/
    android_system_core/fastboot/
    android_system_core/fs_mgr/liblp/include/
    android_system_core/libsparse/include/
    android_system_core/libziparchive/include/
    android_system_core/diagnose_usb/include/
    android_system_core/libcutils/include/
    android_system_core/liblog/include/
    android_system_core/libutils/include/
    android_system_extras/ext4_utils/include/
    zlib/
    googletest/googletest/include/
    include/
)

target_compile_options(fastboot PRIVATE -D_FILE_OFFSET_BITS=64 -DFAKE_LOG_DEVICE=1)
target_link_libraries(fastboot PRIVATE ${OPENSSL_LIBRARIES} zlibstatic)
