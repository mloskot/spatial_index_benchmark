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

enable_testing()

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
  set(CXX_COMMON_FLAGS "-pedantic -Wpointer-arith -Wcast-align -Wcast-qual -Wfloat-equal")
    # "-pedantic -ansi -Wall -Wpointer-arith -Wcast-align -Wcast-qual -Wfloat-equal -Wredundant-decls")

  if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC ${CXX_COMMON_FLAGS} -std=c++0x")
  elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" OR "${CMAKE_CXX_COMPILER}" MATCHES "clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_COMMON_FLAGS} -std=c++11")
  else()
    message(FATAL_ERROR "CMake is unable to recognize compilation toolset")
  endif()
endif()

# Dependencies
find_package(Boost 1.54)
if (NOT Boost_FOUND)
  message(FATAL_ERROR "Cannot find Boost")
endif()
include_directories(${Boost_INCLUDE_DIRS})

find_package(SpatialIndex 1.7.0)
if (NOT SPATIALINDEX_FOUND)
  message(FATAL_ERROR "Cannot find SpatialIndex")
endif()

set(SRC_COMMON
    spatial_index_benchmark.hpp
    high_resolution_timer.hpp)

macro(msg T)
  message(STATUS "Configuring ${T}")
endmacro()

if (CMAKE_HOST_UNIX)
  set(EXTRA_LIBS -lrt)
endif()

################################################################################
# benchmark: libspatialindex 
set(BSI benchmark_lsi)
## Linear
set(TARGET_BSI ${BSI}_linear)
msg(${TARGET_BSI})
add_executable(${TARGET_BSI} ${SRC_COMMON} benchmark_spatialindex.cpp)
target_link_libraries(${TARGET_BSI} ${EXTRA_LIBS} ${SPATIALINDEX_LIBRARY})
set_property(TARGET ${TARGET_BSI} APPEND PROPERTY INCLUDE_DIRECTORIES ${SPATIALINDEX_INCLUDE_DIR})
set_property(TARGET ${TARGET_BSI} APPEND PROPERTY COMPILE_DEFINITIONS RTREE_VARIANT_LINEAR=1)
add_test(NAME ${TARGET_BSI} CONFIGURATIONS Release COMMAND ${TARGET_BSI})
## Quadratic
set(TARGET_BSI ${BSI}_quadratic)
msg(${TARGET_BSI})
add_executable(${TARGET_BSI} ${SRC_COMMON} benchmark_spatialindex.cpp)
target_link_libraries(${TARGET_BSI} ${EXTRA_LIBS} ${SPATIALINDEX_LIBRARY})
set_property(TARGET ${TARGET_BSI} APPEND PROPERTY INCLUDE_DIRECTORIES ${SPATIALINDEX_INCLUDE_DIR})
set_property(TARGET ${TARGET_BSI} APPEND PROPERTY COMPILE_DEFINITIONS RTREE_VARIANT_QUADRATIC=1)
## RStar
set(TARGET_BSI ${BSI}_rstar)
msg(${TARGET_BSI})
add_executable(${TARGET_BSI} ${SRC_COMMON} benchmark_spatialindex.cpp)
target_link_libraries(${TARGET_BSI} ${EXTRA_LIBS} ${SPATIALINDEX_LIBRARY})
set_property(TARGET ${TARGET_BSI} APPEND PROPERTY INCLUDE_DIRECTORIES ${SPATIALINDEX_INCLUDE_DIR})
set_property(TARGET ${TARGET_BSI} APPEND PROPERTY COMPILE_DEFINITIONS RTREE_VARIANT_RSTAR=1)

################################################################################
# benchmark: Boost.Geometry
set(BGI benchmark_bgi)
## Linear
### compile-time parameters
set(TARGET_BGI ${BGI}_linear)
msg(${TARGET_BGI})
add_executable(${TARGET_BGI} ${SRC_COMMON} benchmark_boost_geometry.cpp)
target_link_libraries(${TARGET_BGI} ${EXTRA_LIBS})
set_property(TARGET ${TARGET_BGI} APPEND PROPERTY COMPILE_DEFINITIONS RTREE_VARIANT_LINEAR=1)
### run-time parameters
set(TARGET_BGI ${TARGET_BGI}_rt)
msg(${TARGET_BGI})
add_executable(${TARGET_BGI} ${SRC_COMMON} benchmark_boost_geometry.cpp)
target_link_libraries(${TARGET_BGI} ${EXTRA_LIBS})
set_property(TARGET ${TARGET_BGI} APPEND PROPERTY COMPILE_DEFINITIONS RTREE_VARIANT_LINEAR=1)
set_property(TARGET ${TARGET_BGI} APPEND PROPERTY COMPILE_DEFINITIONS BGI_DYNAMIC_RTREE=1)
## Quadratic
### compile-time parameters
set(TARGET_BGI ${BGI}_quadratic)
msg(${TARGET_BGI})
add_executable(${TARGET_BGI} ${SRC_COMMON} benchmark_boost_geometry.cpp)
target_link_libraries(${TARGET_BGI} ${EXTRA_LIBS})
set_property(TARGET ${TARGET_BGI} APPEND PROPERTY COMPILE_DEFINITIONS RTREE_VARIANT_QUADRATIC=1)
### run-time parameters
set(TARGET_BGI ${TARGET_BGI}_rt)
msg(${TARGET_BGI})
add_executable(${TARGET_BGI} ${SRC_COMMON} benchmark_boost_geometry.cpp)
target_link_libraries(${TARGET_BGI} ${EXTRA_LIBS})
set_property(TARGET ${TARGET_BGI} APPEND PROPERTY COMPILE_DEFINITIONS RTREE_VARIANT_QUADRATIC=1)
set_property(TARGET ${TARGET_BGI} APPEND PROPERTY COMPILE_DEFINITIONS BGI_DYNAMIC_RTREE=1)
## RStar
### compile-time parameters
set(TARGET_BGI ${BGI}_rstar)
msg(${TARGET_BGI})
add_executable(${TARGET_BGI} ${SRC_COMMON} benchmark_boost_geometry.cpp)
target_link_libraries(${TARGET_BGI} ${EXTRA_LIBS})
set_property(TARGET ${TARGET_BGI} APPEND PROPERTY COMPILE_DEFINITIONS RTREE_VARIANT_RSTAR=1)
### run-time parameters
set(TARGET_BGI ${TARGET_BGI}_rt)
msg(${TARGET_BGI})
add_executable(${TARGET_BGI} ${SRC_COMMON} benchmark_boost_geometry.cpp)
target_link_libraries(${TARGET_BGI} ${EXTRA_LIBS})
set_property(TARGET ${TARGET_BGI} APPEND PROPERTY COMPILE_DEFINITIONS RTREE_VARIANT_RSTAR=1)
set_property(TARGET ${TARGET_BGI} APPEND PROPERTY COMPILE_DEFINITIONS BGI_DYNAMIC_RTREE=1)
