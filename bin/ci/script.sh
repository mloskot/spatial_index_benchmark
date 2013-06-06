#!/bin/bash
# Builds and runs spatial_index_benchmark
mkdir -p _build
cd _build
echo "$(tmstamp) *** script::cmake-config starting $(date) ***"
cmake \
	-DCMAKE_VERBOSE_MAKEFILE=ON \
    -DBOOST_DIR=${PWD} \
	..
echo "$(tmstamp) *** script::cmake-config finished $(date) ***"

echo "$(tmstamp) *** script::cmake-build starting $(date) ***"
#cmake --build .
make
echo "$(tmstamp) *** script::cmake-build finished $(date) ***"

echo "$(tmstamp) *** script::benchmark starting $(date) ***"
ls
echo "$(tmstamp) *** script::benchmark finished $(date) ***"
