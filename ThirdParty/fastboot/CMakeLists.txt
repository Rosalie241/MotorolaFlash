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

# libfastboot
set(LIBFASTBOOT_SOURCES 
    android_system_core/fastboot/bootimg_utils.cpp
    android_system_core/fastboot/fastboot.cpp
    android_system_core/fastboot/fs.cpp
    android_system_core/fastboot/socket.cpp
    android_system_core/fastboot/tcp.cpp
    android_system_core/fastboot/udp.cpp
    android_system_core/fastboot/util.cpp
    android_system_core/fastboot/fastboot_driver.cpp
)
set(LIBFASTBOOT_INCLUDES
    android_system_core/mkbootimg/include/bootimg/
    android_system_core/base/include/
    include/
    android_system_core/fs_mgr/liblp/include/
    android_system_core/libsparse/include/
    android_system_core/libziparchive/include/
    android_system_core/diagnose_usb/include/
    android_system_core/libcutils/include/
    googletest/googletest/include/
)
set(LIBFASTBOOT_LIBRARIES
    base ziparchive cutils lp log sparse diagnose_usb ext4_utils
    ${OPENSSL_LIBRARIES}
    zlibstatic
)

# libbase
set(LIBBASE_SOURCES
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
)
set(LIBBASE_INCLUDES
    android_system_core/base/include/
)

# libziparchive
set(LIBZIPARCHIVE_SOURCES
    android_system_core/libziparchive/zip_archive.cc
    android_system_core/libziparchive/zip_archive_stream_entry.cc
    android_system_core/libziparchive/zip_writer.cc
)
set(LIBZIPARCHIVE_INCLUDES
    android_system_core/libziparchive/include/
    android_system_core/base/include/
    android_system_core/liblog/include/
)

# libcutils
set(LIBCUTILS_SOURCES
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
)
set(LIBCUTILS_INCLUDES
    android_system_core/libcutils/include/
)

# liblp
set(LIBLP_SOURCES
    android_system_core/fs_mgr/liblp/builder.cpp
    android_system_core/fs_mgr/liblp/images.cpp
    android_system_core/fs_mgr/liblp/partition_opener.cpp
    android_system_core/fs_mgr/liblp/reader.cpp
    android_system_core/fs_mgr/liblp/utility.cpp
    android_system_core/fs_mgr/liblp/writer.cpp
)
set(LIBLP_INCLUDES
    android_system_core/fs_mgr/liblp/include/
    android_system_core/base/include/
    android_system_core/libsparse/include/
    android_system_extras/ext4_utils/include/
)

# liblog
set(LIBLOG_SOURCES
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
)
set(LIBLOG_INCLUDES
    android_system_core/liblog/include/
    android_system_core/libcutils/include/
)

# libsparse
set(LIBSPARSE_SOURCES
    android_system_core/libsparse/backed_block.cpp
    android_system_core/libsparse/output_file.cpp
    android_system_core/libsparse/sparse.cpp
    android_system_core/libsparse/sparse_crc32.cpp
    android_system_core/libsparse/sparse_err.cpp
    android_system_core/libsparse/sparse_read.cpp
)
set(LIBSPARSE_INCLUDES
    android_system_core/libsparse/include/
    android_system_core/base/include/
)

# libdiagnose_usb
set(LIBDIAGNOSE_USB_SOURCES
    android_system_core/diagnose_usb/diagnose_usb.cpp
)
set(LIBDIAGNOSE_USB_INCLUDES
    android_system_core/diagnose_usb/include/
    android_system_core/base/include/
)

# libext4_utils
set(LIBEXT4_UTILS_SOURCES
    android_system_extras/ext4_utils/ext4_utils.cpp
    android_system_extras/ext4_utils/wipe.cpp
    android_system_extras/ext4_utils/ext4_sb.cpp
)
set(LIBEXT4_UTILS_INCLUDES
    android_system_extras/ext4_utils/include/
    android_system_core/base/include/
)

#
# Add platform dependent stuff
#
if(WIN32)
    list(APPEND LIBFASTBOOT_SOURCES 
        android_system_core/fastboot/usb_windows.cpp
    )

    list(APPEND LIBFASTBOOT_INCLUDES
        android_development/host/windows/usb/api/
    )

    list(APPEND LIBFASTBOOT_LIBRARIES
        AdbWinApi 
        setupapi
    )

    list(APPEND LIBBASE_SOURCES
        android_system_core/base/errors_windows.cpp
        android_system_core/base/utf8.cpp
    )

    list(APPEND LIBCUTILS_SOURCES
        android_system_core/libcutils/socket_inaddr_any_server_windows.cpp
        android_system_core/libcutils/socket_network_client_windows.cpp
        android_system_core/libcutils/sockets_windows.cpp
        android_system_core/libcutils/trace-host.cpp
    )

    # AdbWinApi
    set(LIBADBWINAPI_SOURCES
        android_development/host/windows/usb/api/AdbWinApi.cpp
        android_development/host/windows/usb/api/AdbWinApi.rc
        android_development/host/windows/usb/api/adb_api.cpp
        android_development/host/windows/usb/api/adb_interface_enum.cpp
        android_development/host/windows/usb/api/adb_interface.cpp
        android_development/host/windows/usb/api/adb_object_handle.cpp
        android_development/host/windows/usb/api/adb_helper_routines.cpp
        android_development/host/windows/usb/api/adb_legacy_interface.cpp
        android_development/host/windows/usb/api/adb_legacy_endpoint_object.cpp
        android_development/host/windows/usb/api/adb_endpoint_object.cpp
        android_development/host/windows/usb/api/adb_legacy_io_completion.cpp
        android_development/host/windows/usb/api/adb_io_completion.cpp
        android_development/host/windows/usb/winusb/adb_generic_lock.cpp
    )
    set(LIBADBWINAPI_LIBRARIES
        setupapi
    )

    # AdbWinUsbApi
    set(LIBADBWINUSBAPI_SOURCES
        android_development/host/windows/usb/winusb/AdbWinUsbApi.cpp
        android_development/host/windows/usb/winusb/AdbWinUsbApi.rc
        android_development/host/windows/usb/winusb/adb_winusb_interface.cpp
        android_development/host/windows/usb/winusb/adb_winusb_endpoint_object.cpp
        android_development/host/windows/usb/winusb/adb_generic_lock.cpp
        android_development/host/windows/usb/winusb/adb_winusb_io_completion.cpp
    )
    set(LIBADBWINUSBAPI_LIBRARIES
        AdbWinApi
        winusb
    )
else(WIN32)
    list(APPEND LIBFASTBOOT_SOURCES 
        android_system_core/fastboot/usb_linux.cpp
    )

    list(APPEND LIBBASE_SOURCES
        android_system_core/base/errors_unix.cpp
        android_system_core/base/cmsg.cpp
    )

    list(APPEND LIBCUTILS_SOURCES
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

#
# Create libraries
#
add_library(fastboot STATIC ${LIBFASTBOOT_SOURCES})
add_library(base STATIC ${LIBBASE_SOURCES})
add_library(ziparchive STATIC ${LIBZIPARCHIVE_SOURCES})
add_library(cutils STATIC ${LIBCUTILS_SOURCES})
add_library(lp STATIC ${LIBLP_SOURCES})
add_library(log STATIC ${LIBLOG_SOURCES})
add_library(sparse STATIC ${LIBSPARSE_SOURCES})
add_library(diagnose_usb STATIC ${LIBDIAGNOSE_USB_SOURCES})
add_library(ext4_utils STATIC ${LIBEXT4_UTILS_SOURCES})
if(WIN32) # windows exclusive libs
add_library(AdbWinApi SHARED ${LIBADBWINAPI_SOURCES})
add_library(AdbWinUsbApi SHARED ${LIBADBWINUSBAPI_SOURCES})
endif(WIN32)

#
# Set include directories
#
target_include_directories(fastboot PRIVATE ${LIBFASTBOOT_INCLUDES})
target_include_directories(base PRIVATE ${LIBBASE_INCLUDES})
target_include_directories(ziparchive PRIVATE ${LIBZIPARCHIVE_INCLUDES})
target_include_directories(cutils PRIVATE ${LIBCUTILS_INCLUDES})
target_include_directories(lp PRIVATE ${LIBLP_INCLUDES})
target_include_directories(log PRIVATE ${LIBLOG_INCLUDES})
target_include_directories(sparse PRIVATE ${LIBSPARSE_INCLUDES})
target_include_directories(diagnose_usb PRIVATE ${LIBDIAGNOSE_USB_INCLUDES})
target_include_directories(ext4_utils PRIVATE ${LIBEXT4_UTILS_INCLUDES})

#
# Set compiling flags
#
target_compile_options(fastboot PRIVATE -DNDEBUG)
target_compile_options(lp PRIVATE -D_FILE_OFFSET_BITS=64)
target_compile_options(log PRIVATE -DFAKE_LOG_DEVICE=1)
if(WIN32)
target_compile_options(AdbWinUsbApi PRIVATE -DNDEBUG)
endif(WIN32)

#
# Set link dependencies
#
target_link_libraries(fastboot PRIVATE ${LIBFASTBOOT_LIBRARIES})
if(WIN32)
target_link_libraries(AdbWinApi PRIVATE ${LIBADBWINAPI_LIBRARIES})
target_link_libraries(AdbWinUsbApi PRIVATE ${LIBADBWINUSBAPI_LIBRARIES})
endif(WIN32)

#
# Set target properties
#
if(WIN32)
set_target_properties(AdbWinApi PROPERTIES PREFIX "")
set_target_properties(AdbWinUsbApi PROPERTIES PREFIX "")
endif(WIN32)