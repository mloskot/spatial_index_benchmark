################################################################################
# CMake build configuration for spatial_index_benchmark
################################################################################
# Copyright (C) 2013 Mateusz Loskot <mateusz@loskot.net>
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)
################################################################################
cmake_minimum_required (VERSION 2.6)
project(spatial_index_benchmark)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/")

# Compiler flags
if (MSVC)
  add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  add_definitions(-D_CRT_NONSTDC_NO_WARNING)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS)

  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()  
else()
  set(CXX_COMMON_FLAGS "-pedantic -ansi -Wpointer-arith -Wcast-align -Wcast-qual -Wfloat-equal")
    # "-pedantic -ansi -Wall -Wpointer-arith -Wcast-align -Wcast-qual -Wfloat-equal -Wredundant-decls")

  if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC ${CXX_COMMON_FLAGS} -std=c++11")
  elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" OR "${CMAKE_CXX_COMPILER}" MATCHES "clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_COMMON_FLAGS}")
  else()
    message(FATAL_ERROR "CMake is unable to recognize compilation toolset")
  endif()
endif()

# benchmark: libspatialindex 
set(TARGET_BSI benchmark_spatialindex)
find_package(SpatialIndex 1.7.0)
if (SPATIALINDEX_FOUND)
  message(STATUS "Configuring ${TARGET_BSI}")
  add_executable(${TARGET_BSI} benchmark_spatialindex.cpp)
  target_link_libraries(${TARGET_BSI} ${SPATIALINDEX_LIBRARY})
  set_property(TARGET ${TARGET_BSI} APPEND
    PROPERTY INCLUDE_DIRECTORIES ${SPATIALINDEX_INCLUDE_DIR})
endif()

# benchmark: Boost.Geometry
set(TARGET_BGI benchmark_boost_geometry)
#set(Boost_FIND_QUIETLY TRUE)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.54)
if (Boost_FOUND)
  message(STATUS "Configuring ${TARGET_BGI}")
  add_executable(${TARGET_BGI} benchmark_boost_geometry.cpp)
  set_property(TARGET ${TARGET_BGI} APPEND
    PROPERTY INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS})
endif()
