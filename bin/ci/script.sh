#!/bin/bash
# Builds and runs spatial_index_benchmark
source ./bin/ci/common.sh
mkdir -p _build
cd _build
echo "Calling cmake -DBOOST_PREFIX=${BOOST_PREFIX}"
cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DBOOST_ROOT=${BOOST_PREFIX} \
    ..
#cmake --build .
make -j ${NUMTHREADS}
ctest -C Release -V --output-on-failure .  
