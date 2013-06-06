#!/bin/bash
# Builds and runs spatial_index_benchmark
source ./bin/ci/common.sh
pwd
echo "BOOST_PREFIX=${BOOST_PREFIX}"
mkdir -p _build
cd _build
cmake \
	-DCMAKE_VERBOSE_MAKEFILE=ON \
    -DBOOST_ROOT=${BOOST_PREFIX} \
	..
#cmake --build .
make -j ${NUMTHREADS}
