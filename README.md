spatial_index_benchmark
=======================

Simple benchmark for spatial index implementations.

List of currently measured libraries:
* [Boost.Geometry](http://www.boost.org/libs/geometry/)
* [libspatialindex](http://libspatialindex.github.io)

[![Build Status](https://travis-ci.org/mloskot/spatial_index_benchmark.png?branch=master)](https://travis-ci.org/mloskot/spatial_index_benchmark)

Requirements
------------

* C++11 compiler
* CMake
* Boost headers from its SVN trunk or Boost 1.53+ with copy of [statistics.hpp](https://gist.github.com/mloskot/5715329)
added to trunk in [r84649](https://svn.boost.org/trac/boost/changeset/84649).
Note, this is internal utility and it is not a part of the Boost.Geometry public interface.
* [libspatialindex](https://github.com/libspatialindex/libspatialindex) headers and libraries
(for Windows, use [OSGeo4W](http://trac.osgeo.org/osgeo4w/).

Overview
--------

First prototype, API usage and parameters matched as much as I could, hopefully without major bugs.

*TODO: explain details*

Legend
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

Results
------

```
bgi_ct(L,ITR): insert 1000000 in 1.66157 to 1.66157 sec
bgi_ct(L,ITR) stats: levels=5, nodes=241, leaves=14985, values=1000000, values_min=50, values_max=100
bgi_ct(L,ITR): query 100000 in 2.42952 to 2.42952 sec
bgi_ct(L,ITR) stats: found=100041
bgi_rt(L,ITR): insert 1000000 in 1.74438 to 1.74438 sec
bgi_rt(L,ITR) stats: levels=5, nodes=241, leaves=14985, values=1000000, values_min=50, values_max=100
bgi_rt(L,ITR): query 100000 in 2.62583 to 2.62583 sec
bgi_rt(L,ITR) stats: found=100041
bgi_ct(L,STR): insert 1000000 in 1.66323 to 1.66323 sec
bgi_ct(L,STR) stats: levels=5, nodes=358, leaves=18758, values=1000000, values_min=50, values_max=100
bgi_ct(L,STR): query 100000 in 0.107911 to 0.107911 sec
bgi_ct(L,STR) stats: found=100036
bgi_rt(L,STR): insert 1000000 in 1.74423 to 1.74423 sec
bgi_rt(L,STR) stats: levels=5, nodes=358, leaves=18758, values=1000000, values_min=50, values_max=100
bgi_rt(L,STR): query 100000 in 0.112801 to 0.112801 sec
bgi_rt(L,STR) stats: found=100036
lsi(L,ITR)  : insert 1000000 in 24.327 to 24.327 sec
lsi(L,ITR) stats: reads=3686147, writes=1128391, nodes=15135, ndata=1000000
lsi(L,ITR)  : query 100000 in 54.6093 to 54.6093 sec
lsi(L,ITR) stats: found=100041
lsi(L,STR)  : insert 1000000 in 5.13961 to 5.13961 sec
lsi(L,STR) stats: reads=1, writes=20410, nodes=20409, ndata=1000000
lsi(L,STR)  : query 100000 in 3.25601 to 3.25601 sec
lsi(L,STR) stats: found=100041
bgi_ct(Q,ITR): insert 1000000 in 3.43475 to 3.43475 sec
bgi_ct(Q,ITR) stats: levels=5, nodes=209, leaves=14447, values=1000000, values_min=50, values_max=100
bgi_ct(Q,ITR): query 100000 in 0.364684 to 0.364684 sec
bgi_ct(Q,ITR) stats: found=100041
bgi_rt(Q,ITR): insert 1000000 in 3.53917 to 3.53917 sec
bgi_rt(Q,ITR) stats: levels=5, nodes=209, leaves=14447, values=1000000, values_min=50, values_max=100
bgi_rt(Q,ITR): query 100000 in 0.381359 to 0.381359 sec
bgi_rt(Q,ITR) stats: found=100041
bgi_ct(Q,STR): insert 1000000 in 3.86699 to 3.86699 sec
bgi_ct(Q,STR) stats: levels=5, nodes=285, leaves=17203, values=1000000, values_min=50, values_max=100
bgi_ct(Q,STR): query 100000 in 0.18763 to 0.18763 sec
bgi_ct(Q,STR) stats: found=100036
bgi_rt(Q,STR): insert 1000000 in 4.0216 to 4.0216 sec
bgi_rt(Q,STR) stats: levels=5, nodes=285, leaves=17203, values=1000000, values_min=50, values_max=100
bgi_rt(Q,STR): query 100000 in 0.196158 to 0.196158 sec
bgi_rt(Q,STR) stats: found=100036
lsi(Q,ITR)  : insert 1000000 in 37.5014 to 37.5014 sec
lsi(Q,ITR) stats: reads=3629754, writes=1134191, nodes=14597, ndata=1000000
lsi(Q,ITR)  : query 100000 in 7.53134 to 7.53134 sec
lsi(Q,ITR) stats: found=100041
lsi(Q,STR)  : insert 1000000 in 4.98813 to 4.98813 sec
lsi(Q,STR) stats: reads=1, writes=20410, nodes=20409, ndata=1000000
lsi(Q,STR)  : query 100000 in 3.32984 to 3.32984 sec
lsi(Q,STR) stats: found=100041
bgi_ct(R,ITR): insert 1000000 in 69.0967 to 69.0967 sec
bgi_ct(R,ITR) stats: levels=5, nodes=180, leaves=13646, values=1000000, values_min=50, values_max=100
bgi_ct(R,ITR): query 100000 in 0.222986 to 0.222986 sec
bgi_ct(R,ITR) stats: found=100041
bgi_rt(R,ITR): insert 1000000 in 67.2497 to 67.2497 sec
bgi_rt(R,ITR) stats: levels=5, nodes=180, leaves=13646, values=1000000, values_min=50, values_max=100
bgi_rt(R,ITR): query 100000 in 0.22847 to 0.22847 sec
bgi_rt(R,ITR) stats: found=100041
bgi_ct(R,STR): insert 1000000 in 63.7845 to 63.7845 sec
bgi_ct(R,STR) stats: levels=5, nodes=327, leaves=18232, values=1000000, values_min=50, values_max=100
bgi_ct(R,STR): query 100000 in 0.0739523 to 0.0739523 sec
bgi_ct(R,STR) stats: found=100036
bgi_rt(R,STR): insert 1000000 in 65.1467 to 65.1467 sec
bgi_rt(R,STR) stats: levels=5, nodes=327, leaves=18232, values=1000000, values_min=50, values_max=100
bgi_rt(R,STR): query 100000 in 0.07797 to 0.07797 sec
bgi_rt(R,STR) stats: found=100036
lsi(R,ITR)  : insert 1000000 in 59.4917 to 59.4917 sec
lsi(R,ITR) stats: reads=5422163, writes=1756203, nodes=14735, ndata=1000000
lsi(R,ITR)  : query 100000 in 3.42801 to 3.42801 sec
lsi(R,ITR) stats: found=100041
lsi(R,STR)  : insert 1000000 in 5.07948 to 5.07948 sec
lsi(R,STR) stats: reads=1, writes=20410, nodes=20409, ndata=1000000
lsi(R,STR)  : query 100000 in 3.3185 to 3.3185 sec
lsi(R,STR) stats: found=100041
```

TODO
----

* find & fix bugs
* test random points
* test nearest neighbor and other types of queries
* test misc values for Guttman's rtree ```M``` and ```m<=M/2``` bounds

Disclaimer
==========

I created this benchmark driven by curiosity and for my own purposes, with hope to 
obtain useful and interesting results, for myself and others too.
I do not have any objective of making ultimate performance shootout.
This is not a rocket science, but a simple set of C++ programs, with likelyhood
of bugs or inconsistencies. Found any, please report. Comments and improvements
are always welcome!

License
=======

Distributed under the Boost Software License, Version 1.0.
See accompanying file LICENSE_1_0.txt or copy at 
http://www.boost.org/LICENSE_1_0.txt.

Copyright
=========

Copyright 2013 Mateusz Loskot <mateusz at loskot dot net>

and other hackers (see copyright headers in source files).
