#!/bin/sh

if [[ ! -d $1 ]]; then
    echo "Cannot find '$1' build directory"
    exit 1
fi

for variant in linear quadratic rstar
do
    for benchmark in `find $1 -type f -name "*${variant}*" -executable | sort`
    do
        "$benchmark"
    done;
done;
