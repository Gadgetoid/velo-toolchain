cmake_minimum_required(VERSION 3.20)
project(debugmgr C)

include(VeloCE)

if(VELO_ARCH STREQUAL "sh3")
    velo_add_executable(debugmgr OUTPUT velo-debugmgr.exe debugmgr.c)
else()
    velo_add_executable(debugmgr debugmgr.c)
endif()
