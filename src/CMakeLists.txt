cmake_minimum_required(VERSION 3.14)
project(DemoAIModule)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/3rdparty)

set(SRC_INCLUDES
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/Instrumentation
        )
include_directories(${SRC_INCLUDES})

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR} *.cpp)

add_library(DemoAIModule SHARED ${SRC_FILES})

IF (MSVC)
    target_link_libraries(DemoAIModule BWAPILIB libzstd_static)
ELSE (MSVC)
    target_link_libraries(DemoAIModule -L/usr/local/Cellar/llvm/13.0.0_2/lib BWAPILIB libzstd_static)
ENDIF (MSVC)

target_include_directories(DemoAIModule PUBLIC ${SRC_INCLUDES})

IF (NOT MSVC)
    target_compile_options(DemoAIModule PRIVATE -Wall -Werror)
ENDIF (NOT MSVC)
