# spatial_index_benchmark

Simple non-academic performance comparison of available open source
implementations of R-tree spatial index using *linear*, *quadratic and R-star
balancing algorithms as well as bulk loading (Sort-Tile-Recurse or combined methods).

List of currently measured libraries:
* [Boost.Geometry](http://www.boost.org/libs/geometry/)
* [libspatialindex](http://libspatialindex.github.io)

More libraries have been suggested, see the GitHub Issues.

[![Build Status](https://travis-ci.org/mloskot/spatial_index_benchmark.png?branch=master)](https://travis-ci.org/mloskot/spatial_index_benchmark)

## Requirements

* C++11 compiler
* CMake
* Boost headers current SVN trunk which includes required **internal** utilities:
  * ```boost/geometry/index/detail/rtree/utilities/statistics.hpp``` -
added in [r84649](https://svn.boost.org/trac/boost/changeset/84649)
  * ```boost/geometry/index/detail/rtree/pack_create.hpp``` - 
added in [r84720](https://svn.boost.org/trac/boost/changeset/84720)
* [libspatialindex](https://github.com/libspatialindex/libspatialindex) headers
and libraries (for Windows, use [OSGeo4W](http://trac.osgeo.org/osgeo4w/).

## Results

First prototype, API usage and parameters matched as much as I could, 
hopefully without major bugs.  

*TODO: explain details*

Complete set of result logs in [results](results) directory.

### Visual C++ 11.0 (32-bit build)

HW: Intel(R) Xeon(R) CPU E5-2687W 0 @ 3.10GHz, 16 GB RAM; OS: Windows 7 64-bit
SW: Visual Studio 2012

* Loading times for each of the R-tree construction methods

![load libspatialindex](https://raw.github.com/mloskot/spatial_index_benchmark/master/results/2/benchmark_rtree_load_itr_vs_blk_lsi.png)

![load boost::geometry](https://raw.github.com/mloskot/spatial_index_benchmark/master/results/2/benchmark_rtree_load_itr_vs_blk_bgi.png)

* Query times for each of the R-tree construction methods

![query libspatialindex](https://raw.github.com/mloskot/spatial_index_benchmark/master/results/2/benchmark_rtree_query_itr_vs_blk_lsi.png)

![query boost::geometry](https://raw.github.com/mloskot/spatial_index_benchmark/master/results/2/benchmark_rtree_query_itr_vs_blk_bgi.png)

### Legend
------

* ```bgi``` - boost::geometry::index (_rt is dynamic variant: L,Q,R etc. parameters specified at run-time)
* ```lsi``` - libspatialindex
* ```ct``` - Boost.Geometry-only, compile-time specification of rtree parameters
* ```rt``` (or non suffix) - Boost.Geometry-only, run-time specification of rtree parameters
* ```L``` - linear
* ```Q``` - quadratic
* ```R``` - rstar
* ```itr (or no suffix)```  - iterative insertion method of building rtree
* ```blk```  - bulk loading method of building R-tree (Split-Tile-Recurse for ```lsi```, custom algorithm for ```bgi```)
  
* insert 1000000 - number of objects small random boxes
* query   100000 - number of instersection-based queries with random boxes 10x larger than those inserted
* stats generated using lsi's API and purposely written visitor for Boost.Geometry (not yet in Boost trunk)

## Disclaimer

This project is driven by curiosity and for my own purposes, with hope to 
obtain useful and interesting results, for myself and others too.
I do not have any objective of making ultimate performance shootout.
This is not a rocket science, but a simple set of C++ programs, with likelyhood
of bugs or inconsistencies. Found any, please report. Comments and improvements
are always welcome!

## Authors

* Mateusz Loskot
* Adam Wulkiewicz

## License

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at 
http://www.boost.org/LICENSE_1_0.txt.
