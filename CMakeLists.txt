# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.3)

# 项目信息
project("Socketfun")

# include路径
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories("${PROJECT_BINARY_DIR}")

# 设置可执行文件生成路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/)

# 生成debug版本
SET(CMAKE_BUILD_TYPE "release")
if (CMAKE_BUILD_TYPE STREQUAL debug)
    add_definitions(-D_DEBUG)
endif ()
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb ")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall ")

set (Socketfun_VERSION_MAJOR 1)
set (Socketfun_VERSION_MINOR 0)

# 配置一个头文件，从而将CMake的一些设置传递到源文件中。
# 以Config.h.in为模板，生成Config.h
configure_file (
  "${PROJECT_SOURCE_DIR}/SocketfunConfig.h.in"
  "${PROJECT_BINARY_DIR}/SocketfunConfig.h"
  )


# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(./src DIR_SRCS)

# 指定生成目标
add_executable(Socketfun ${DIR_SRCS})
