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

```
bgi - boost::geometry::index (_rt is dynamic variant: L,Q,R etc. parameters specified at run-time)
lsi - libspatialindex
L - linear
Q - quadratic
R - rstar
insert 1000000 - number of objects small random boxes
query   100000 - number of instersection-based queries with random boxes 10x larger than those inserted
stats generated using lsi's API and purposely written visitor for Boost.Geometry (not yet in Boost trunk)
```

Results
------

```
bgi(L)      : insert 1000000 in 1.91014 to 1.91014 sec
bgi(L) stats: levels=5, nodes=242, leaves=14973, values=1000000, values_min=50, values_max=100
bgi(L)      : query 100000 in 2.34366 to 2.34366 sec
bgi(L) stats: found=100056
bgi_rt(L)   : insert 1000000 in 1.96369 to 1.96369 sec
bgi_rt(L) stats: levels=5, nodes=252, leaves=14952, values=1000000, values_min=50, values_max=100
bgi_rt(L)   : query 100000 in 2.12329 to 2.12329 sec
bgi_rt(L) stats: found=100045
bgi(Q)      : insert 1000000 in 3.62814 to 3.62814 sec
bgi(Q) stats: levels=5, nodes=223, leaves=14699, values=1000000, values_min=50, values_max=100
bgi(Q)      : query 100000 in 0.646551 to 0.646551 sec
bgi(Q) stats: found=100038
bgi_rt(Q)   : insert 1000000 in 3.74065 to 3.74065 sec
bgi_rt(Q) stats: levels=5, nodes=217, leaves=14686, values=1000000, values_min=50, values_max=100
bgi_rt(Q)   : query 100000 in 0.672224 to 0.672224 sec
bgi_rt(Q) stats: found=100042
bgi(R)      : insert 1000000 in 49.0042 to 49.0042 sec
bgi(R) stats: levels=5, nodes=185, leaves=13656, values=1000000, values_min=50, values_max=100
bgi(R)      : query 100000 in 0.139827 to 0.139827 sec
bgi(R) stats: found=100043
bgi_rt(R)   : insert 1000000 in 51.3237 to 51.3237 sec
bgi_rt(R) stats: levels=5, nodes=185, leaves=13624, values=1000000, values_min=50, values_max=100
bgi_rt(R)   : query 100000 in 0.148757 to 0.148757 sec
bgi_rt(R) stats: found=100046
lsi(L)      : insert 1000000 in 30.0873 to 30.0873 sec
lsi(L) stats: reads=3689046, writes=1118290, nodes=15198, ndata=1000000
lsi(L)      : query 100000 in 55.74 to 55.74 sec
lsi(L) stats: found=100048
lsi(Q)      : insert 1000000 in 51.7956 to 51.7956 sec
lsi(Q) stats: reads=3656193, writes=1136972, nodes=14622, ndata=1000000
lsi(Q)      : query 100000 in 8.31695 to 8.31695 sec
lsi(Q) stats: found=100039
lsi(R)      : insert 1000000 in 66.9438 to 66.9438 sec
lsi(R) stats: reads=5383341, writes=1741968, nodes=14741, ndata=1000000
lsi(R)      : query 100000 in 3.7448 to 3.7448 sec
lsi(R) stats: found=100049
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
