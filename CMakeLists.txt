# CMake build for Windows, Linux, and macOS
# Updated by Liang-Jun Zhu
# zlj@lreis.ac.cn
cmake_minimum_required(VERSION 2.8)
project(UtilsClass)
IF (COMMAND CMAKE_POLICY)
    CMAKE_POLICY(SET CMP0015 NEW)
ENDIF (COMMAND CMAKE_POLICY)
SET(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})
################# OS platform   ##################
if (WIN32)
    add_definitions(-Dwindows)
    if (MSVC)
        add_definitions(-DMSVC)
    endif ()
elseif (APPLE)
    add_definitions(-Dmacos)
else ()
    add_definitions(-Dlinux)
endif ()
set(SOURCE_FILES main.cpp utils.cpp StringList.cpp ModelException.cpp util.cpp)
add_executable(UtilsClass ${SOURCE_FILES})
install(TARGETS ${UtilsClass} DESTINATION bin)
