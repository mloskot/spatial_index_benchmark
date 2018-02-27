#!/bin/bash
# Functions
tmstamp() { echo -n "[$(date '+%H:%M:%S')]" ; }
# Environment
NUMTHREADS=4
if [[ -f /sys/devices/system/cpu/online ]]; then
	# Calculates 1.5 times physical threads
	NUMTHREADS=$(( ( $(cut -f 2 -d '-' /sys/devices/system/cpu/online) + 1 ) * 15 / 10  ))
fi
NUMTHREADS=1 # disable MP
export NUMTHREADS
export BOOST_SVN="https://svn.boost.org/svn/boost/trunk/boost"
export BOOST_PREFIX="${HOME}/trunk"
export BOOST_HEADERS="${HOME}/trunk/boost"
