# spatial_index_benchmark

Simple benchmark for spatial index implementations.

List of currently measured libraries:
* [Boost.Geometry](http://www.boost.org/libs/geometry/)
* [libspatialindex](http://libspatialindex.github.io)

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

First prototype, API usage and parameters matched as much as I could, hopefully without major bugs.

*TODO: explain details*

### Legend
------

* ```bgi``` - boost::geometry::index (_rt is dynamic variant: L,Q,R etc. parameters specified at run-time)
* ```lsi``` - libspatialindex
* ```ct``` - Boost.Geometry-only, compile-time specification of rtree parameters
* ```rt``` - Boost.Geometry-only, run-time specification of rtree parameters
* ```L``` - linear
* ```Q``` - quadratic
* ```R``` - rstar
* ```ITR```  - iterative insertion method of building rtree
* ```STR```  - Split-Tile-Recurse (bulk loading) method of building rtree
    * not available in Boost.Geometry, where STR indicates objects sorting before insertion.
* insert 1000000 - number of objects small random boxes
* query   100000 - number of instersection-based queries with random boxes 10x larger than those inserted
* stats generated using lsi's API and purposely written visitor for Boost.Geometry (not yet in Boost trunk)

### Visual C++ 11.0 (32-bit build)

HW: Intel(R) Xeon(R) CPU E5-2687W 0 @ 3.10GHz, 16 GB RAM; OS: Windows 7 64-bit

```
bgi_ct(L,ITR)  : insert 1000000 in 1.95593 to 1.95593 sec
bgi_ct(L,ITR) stats: levels=5, nodes=241, leaves=14985, values=1000000, values_min=50, values_max=100
bgi_ct(L,ITR)  : query 100000 in 1.65373 to 1.65373 sec
bgi_ct(L,ITR) stats: found=100041
bgi_rt(L,ITR)  : insert 1000000 in 2.03737 to 2.03737 sec
bgi_rt(L,ITR) stats: levels=5, nodes=241, leaves=14985, values=1000000, values_min=50, values_max=100
bgi_rt(L,ITR)  : query 100000 in 1.65572 to 1.65572 sec
bgi_rt(L,ITR) stats: found=100041
bgi_ct(L,STR)  : insert 1 in 1.18915 to 1.18915 sec
bgi_ct(L,STR) stats: levels=3, nodes=101, leaves=10000, values=1000000, values_min=100, values_max=100
bgi_ct(L,STR)  : query 100000 in 0.0948628 to 0.0948628 sec
bgi_ct(L,STR) stats: found=100041
bgi_ct(Q,ITR)  : insert 1000000 in 3.64972 to 3.64972 sec
bgi_ct(Q,ITR) stats: levels=5, nodes=209, leaves=14447, values=1000000, values_min=50, values_max=100
bgi_ct(Q,ITR)  : query 100000 in 0.230989 to 0.230989 sec
bgi_ct(Q,ITR) stats: found=100041
bgi_rt(Q,ITR)  : insert 1000000 in 3.83483 to 3.83483 sec
bgi_rt(Q,ITR) stats: levels=5, nodes=209, leaves=14447, values=1000000, values_min=50, values_max=100
bgi_rt(Q,ITR)  : query 100000 in 0.254898 to 0.254898 sec
bgi_rt(Q,ITR) stats: found=100041
bgi_ct(Q,STR)  : insert 1 in 1.18952 to 1.18952 sec
bgi_ct(Q,STR) stats: levels=3, nodes=101, leaves=10000, values=1000000, values_min=100, values_max=100
bgi_ct(Q,STR)  : query 100000 in 0.0937152 to 0.0937152 sec
bgi_ct(Q,STR) stats: found=100041
bgi_ct(R,ITR)  : insert 1000000 in 49.5555 to 49.5555 sec
bgi_ct(R,ITR) stats: levels=5, nodes=180, leaves=13646, values=1000000, values_min=50, values_max=100
bgi_ct(R,ITR)  : query 100000 in 0.1379 to 0.1379 sec
bgi_ct(R,ITR) stats: found=100041
bgi_rt(R,ITR)  : insert 1000000 in 52.7014 to 52.7014 sec
bgi_rt(R,ITR) stats: levels=5, nodes=180, leaves=13646, values=1000000, values_min=50, values_max=100
bgi_rt(R,ITR)  : query 100000 in 0.148991 to 0.148991 sec
bgi_rt(R,ITR) stats: found=100041
bgi_ct(R,STR)  : insert 1 in 1.19898 to 1.19898 sec
bgi_ct(R,STR) stats: levels=3, nodes=101, leaves=10000, values=1000000, values_min=100, values_max=100
bgi_ct(R,STR)  : query 100000 in 0.0945675 to 0.0945675 sec
bgi_ct(R,STR) stats: found=100041

```

## TODO

* find & fix bugs
* test random points
* test nearest neighbor and other types of queries
* test misc values for Guttman's rtree ```M``` and ```m<=M/2``` bounds

## Disclaimer

I created this benchmark driven by curiosity and for my own purposes, with hope to 
obtain useful and interesting results, for myself and others too.
I do not have any objective of making ultimate performance shootout.
This is not a rocket science, but a simple set of C++ programs, with likelyhood
of bugs or inconsistencies. Found any, please report. Comments and improvements
are always welcome!

## License


Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at 
http://www.boost.org/LICENSE_1_0.txt.

## Copyright

Copyright 2013 Mateusz Loskot <mateusz at loskot dot net>

and other hackers (see copyright headers in source files).
