#!/gnuplot
################################################################################
# Results plotting configuration for spatial_index_benchmark
# https://github.com/mloskot/spatial_index_benchmark
################################################################################
# Copyright (C) 2013 Mateusz Loskot <mateusz@loskot.net>
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)
################################################################################
#
#set terminal wxt
set key on left horizontal
#set terminal pngcairo size 800,600 font ",10"
set terminal svg size 800,600 dynamic font ",10"
outfmt = ".svg"
libs = "bgi lsi"
algos = "linear quadratic rstar"

set xlabel "max capacity (min capacity = max * 0.5)"
#
# Plot loading times
#
set ylabel "load 1M objects in seconds"

set title "Iterative loading using R-tree balancing algorithms vs bulk loading (blk)"
set output "benchmark_rtree_load_itr_vs_blk".outfmt
plot for [l in libs] for [ m in algos." rstar_blk" ] \
  l."_".m.".dat" using 1:3 with lines title l."_".m

set title "Bulk loading (blk) times not affected by R-tree balancing algorithms"
set output "benchmark_rtree_load_blk_vs_balancing".outfmt
plot for [l in libs] for [m in algos] \
  l."_".m."_blk.dat" using 1:3 with lines title l."_".m."_blk"

#
# Plot querying times
#
set ylabel "query 100K of 1M objects in seconds"

set title "Query times for each of R-tree construction methods"
set output "benchmark_rtree_query_itr_vs_blk".outfmt
plot for [l in libs] for [ m in algos." rstar_blk" ] \
  l."_".m.".dat" using 1:4 with lines title l."_".m

set title "Query times not affected by R-tree bulk loading (blk) vs balancing algorithms"
set output "benchmark_rtree_query_blk_vs_balancing".outfmt
plot for [l in libs] for [ m in algos ] \
  l."_".m."_blk.dat" using 1:4 with lines title l."_".m."_blk"

#pause -1
#    EOF
