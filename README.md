# spatial_index_benchmark

Simple non-academic performance comparison of available open source
implementations of R-tree spatial index using *linear*, *quadratic and R-star
balancing algorithms as well as bulk loading (Sort-Tile-Recurse or combined methods).

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
bgi_ct(L,ITR)  : insert 1000000 in 1.94096 to 1.94096 sec
bgi_ct(L,ITR) stats: levels=5, nodes=241, leaves=14985, values=1000000, values_min=50, values_max=100
bgi_ct(L,ITR)  : query 100000 in 1.63834 to 1.63834 sec
bgi_ct(L,ITR) stats: found=100041
bgi_rt(L,ITR)  : insert 1000000 in 2.08466 to 2.08466 sec
bgi_rt(L,ITR) stats: levels=5, nodes=241, leaves=14985, values=1000000, values_min=50, values_max=100
bgi_rt(L,ITR)  : query 100000 in 1.65157 to 1.65157 sec
bgi_rt(L,ITR) stats: found=100041
bgi_ct(L,STR)  : insert 1 in 1.16072 to 1.16072 sec
bgi_ct(L,STR) stats: levels=3, nodes=101, leaves=10000, values=1000000, values_min=100, values_max=100
bgi_ct(L,STR)  : query 100000 in 0.0926232 to 0.0926232 sec
bgi_ct(L,STR) stats: found=100041
bgi_ct(Q,ITR)  : insert 1000000 in 3.73827 to 3.73827 sec
bgi_ct(Q,ITR) stats: levels=5, nodes=209, leaves=14447, values=1000000, values_min=50, values_max=100
bgi_ct(Q,ITR)  : query 100000 in 0.229942 to 0.229942 sec
bgi_ct(Q,ITR) stats: found=100041
bgi_rt(Q,ITR)  : insert 1000000 in 3.86477 to 3.86477 sec
bgi_rt(Q,ITR) stats: levels=5, nodes=209, leaves=14447, values=1000000, values_min=50, values_max=100
bgi_rt(Q,ITR)  : query 100000 in 0.263479 to 0.263479 sec
bgi_rt(Q,ITR) stats: found=100041
bgi_ct(Q,STR)  : insert 1 in 1.35275 to 1.35275 sec
bgi_ct(Q,STR) stats: levels=3, nodes=101, leaves=10000, values=1000000, values_min=100, values_max=100
bgi_ct(Q,STR)  : query 100000 in 0.0993803 to 0.0993803 sec
bgi_ct(Q,STR) stats: found=100041
bgi_ct(R,ITR)  : insert 1000000 in 49.5993 to 49.5993 sec
bgi_ct(R,ITR) stats: levels=5, nodes=180, leaves=13646, values=1000000, values_min=50, values_max=100
bgi_ct(R,ITR)  : query 100000 in 0.145886 to 0.145886 sec
bgi_ct(R,ITR) stats: found=100041
bgi_rt(R,ITR)  : insert 1000000 in 52.7011 to 52.7011 sec
bgi_rt(R,ITR) stats: levels=5, nodes=180, leaves=13646, values=1000000, values_min=50, values_max=100
bgi_rt(R,ITR)  : query 100000 in 0.146533 to 0.146533 sec
bgi_rt(R,ITR) stats: found=100041
bgi_ct(R,STR)  : insert 1 in 1.1649 to 1.1649 sec
bgi_ct(R,STR) stats: levels=3, nodes=101, leaves=10000, values=1000000, values_min=100, values_max=100
bgi_ct(R,STR)  : query 100000 in 0.0915414 to 0.0915414 sec
bgi_ct(R,STR) stats: found=100041
lsi(L,ITR)     : insert 1000000 in 29.6271 to 29.6271 sec
lsi(L,ITR) stats: reads=3686147, writes=1128391, nodes=15135, ndata=1000000
lsi(L,ITR)     : query 100000 in 63.7928 to 63.7928 sec
lsi(L,ITR) stats: found=100041
lsi(L,STR)     : insert 1000000 in 5.14438 to 5.14438 sec
lsi(L,STR) stats: reads=1, writes=20410, nodes=20409, ndata=1000000
lsi(L,STR)     : query 100000 in 3.31617 to 3.31617 sec
lsi(L,STR) stats: found=100041
lsi(Q,ITR)     : insert 1000000 in 50.8474 to 50.8474 sec
lsi(Q,ITR) stats: reads=3629754, writes=1134191, nodes=14597, ndata=1000000
lsi(Q,ITR)     : query 100000 in 8.28458 to 8.28458 sec
lsi(Q,ITR) stats: found=100041
lsi(Q,STR)     : insert 1000000 in 5.05832 to 5.05832 sec
lsi(Q,STR) stats: reads=1, writes=20410, nodes=20409, ndata=1000000
lsi(Q,STR)     : query 100000 in 3.3102 to 3.3102 sec
lsi(Q,STR) stats: found=100041
lsi(R,ITR)     : insert 1000000 in 67.4284 to 67.4284 sec
lsi(R,ITR) stats: reads=5422163, writes=1756203, nodes=14735, ndata=1000000
lsi(R,ITR)     : query 100000 in 3.91276 to 3.91276 sec
lsi(R,ITR) stats: found=100041
lsi(R,STR)     : insert 1000000 in 5.04004 to 5.04004 sec
lsi(R,STR) stats: reads=1, writes=20410, nodes=20409, ndata=1000000
lsi(R,STR)     : query 100000 in 3.31009 to 3.31009 sec
lsi(R,STR) stats: found=100041
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
