//
// Copyright (C) 2013 Mateusz Loskot <mateusz@loskot.net>
// Copyright (c) 2011-2013 Adam Wulkiewicz, Lodz, Poland.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy 
// at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef MLOSKOT_SPATIAL_INDEX_BENCHMARK_HPP_INCLUDED
#define MLOSKOT_SPATIAL_INDEX_BENCHMARK_HPP_INCLUDED
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>
#include <utility>
#include "high_resolution_timer.hpp"

namespace sibench
{

//
// Default benchmark settings
//
std::size_t max_marks = 2; // 10;
std::size_t max_iterations = 1000;

//
// Generators of random objects
//


inline std::vector<std::pair<float, float>> generate_coordinates(std::size_t n)
{
    float const max_val = static_cast<float>(n / 2);

    std::random_device rdev;
    std::mt19937 rgen(rdev());
    std::uniform_real_distribution<float> rdist(-max_val, max_val);

    std::vector<std::pair<float, float>> coords;
    coords.reserve(n);
    coords.emplace_back(rdist(rgen), rdist(rgen));
    return std::move(coords);
}

//
// Benchmark running routines
//

} // namespace sibench

#endif
