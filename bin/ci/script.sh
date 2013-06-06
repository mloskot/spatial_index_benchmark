#!/bin/bash
# Builds and runs spatial_index_benchmark
source ./bin/ci/common.sh
BOOST_PREFIX=${PWD}
mkdir -p _build
cd _build
echo "$(tmstamp) *** script::cmake starting $(date) ***"
cmake \
	-DCMAKE_VERBOSE_MAKEFILE=ON \
    -DBOOST_DIR=${BOOST_PREFIX} \
	..
echo "$(tmstamp) *** script::cmake finished $(date) ***"

echo "$(tmstamp) *** script::make -j ${NUMTHREADS} $(date) ***"
#cmake --build .
make
echo "$(tmstamp) *** script::make finished $(date) ***"

echo "$(tmstamp) *** script::benchmark starting $(date) ***"
ls
echo "$(tmstamp) *** script::benchmark finished $(date) ***"
