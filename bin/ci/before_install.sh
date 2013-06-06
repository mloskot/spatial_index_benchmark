#!/bin/bash
# Installs requirements for spatial_index_benchmark
source ./bin/ci/common.sh
pwd
echo "$(tmstamp) *** before_install::apt-get starting $(date) ***"
sudo apt-key adv --recv-keys --keyserver keyserver.ubuntu.com 16126D3A3E5C1192
sudo apt-get update -qq
sudo apt-get install -qq libspatialindex-dev subversion
echo "$(tmstamp) *** before_install::apt-get finished $(date) ***"

echo "$(tmstamp) *** before_install::svn co boost starting $(date) ***"
# Boost 1.55+ or trunk is required
mkdir -p ${BOOST_PREFIX}
echo "Running svn co ${BOOST_SVN} ${BOOST_HEADERS}"
svn checkout ${BOOST_SVN} ${BOOST_HEADERS} > /dev/null
echo "$(tmstamp) *** before_install::svn co boost finished  $(date) ***"
