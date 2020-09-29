cmake_minimum_required(VERSION 3.14)
project(StardustDevEnvironment)

set(CMAKE_CXX_STANDARD 17)

IF(MSVC)
    add_definitions(-DWITHOUT_POSIX)
    add_subdirectory(3rdparty/BWAPILIB)
ELSE(MSVC)
    set(OPENBW_DIR ${CMAKE_SOURCE_DIR}/3rdparty/openbw/openbw)

    add_subdirectory(3rdparty/openbw/bwapi)
ENDIF(MSVC)

add_subdirectory(3rdparty/zstd/build/cmake)
add_subdirectory(src)

IF(NOT MSVC)
    add_subdirectory(test)
ENDIF(NOT MSVC)
