cmake_minimum_required (VERSION 2.6)
project(spatial_index_benchmark)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules/")

find_package(SpatialIndex 1.8.0)
if (SPATIALINDEX_FOUND)
  add_executable(benchmark_spatialindex benchmark_spatialindex.cpp)
  target_link_libraries(benchmark_spatialindex ${SPATIALINDEX_LIBRARY})
  set_property(TARGET benchmark_spatialindex APPEND
    PROPERTY INCLUDE_DIRECTORIES ${SPATIALINDEX_INCLUDE_DIR})
endif()

#add_executable(benchmark_boost_geometry benchmark_boost_geometry.cpp)
