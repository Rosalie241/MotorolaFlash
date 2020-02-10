#
# MotorolaFlash CMakeLists.txt
#
cmake_minimum_required(VERSION 3.13)

# we use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(MotorolaFlash)

# libtinyxml2, required for parsing xml
add_subdirectory(ThirdParty/tinyxml2/tinyxml2)

# libfastboot
add_subdirectory(ThirdParty/fastboot)

# enable automatic stuff
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets Core REQUIRED)

add_executable(MotorolaFlash
    # Window(s)
    Source/MotorolaFlash.ui

    # Source code
    Source/Flasher.cpp
    Source/MotorolaFlash.cpp
    Source/Fastboot.cpp
    Source/main.cpp
)

# set include directories
target_include_directories(MotorolaFlash PRIVATE
    ThirdParty/tinyxml2/tinyxml2/
    # fastboot
    ThirdParty/fastboot/android_system_core/
    ThirdParty/fastboot/android_system_core/base/include/
    # bootimg.h
    ThirdParty/fastboot/android_system_core/mkbootimg/include/bootimg/
    # sparse/sparse.h
    ThirdParty/fastboot/android_system_core/libsparse/include/
)

# link to tinyxml2 and qt
target_link_libraries(MotorolaFlash Qt5::Widgets fastboot tinyxml2)