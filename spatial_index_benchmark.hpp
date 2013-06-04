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
#ifdef _MSC_VER
#define NOMINMAX
#endif
#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <chrono>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
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
std::size_t max_objects = 1000;
std::size_t max_iterations = 1000;

//
// Generators of random objects
//
typedef float coord_t;
typedef std::vector<coord_t> coords_t;
typedef std::tuple<coord_t, coord_t> point2d_t;
typedef std::vector<point2d_t> points2d_t;
typedef std::tuple<coord_t, coord_t, coord_t, coord_t> box2d_t;
typedef std::vector<box2d_t> boxes2d_t;

template <typename T>
struct random_generator
{
    typedef typename std::uniform_real_distribution<T>::result_type result_type;

    T const max;
    std::mt19937 gen;
    std::uniform_real_distribution<T> dis;

    random_generator(std::size_t n)
        : max(static_cast<T>(n / 2))
        , gen((unsigned int)std::chrono::system_clock::now().time_since_epoch().count())
        , dis(-max, max)
    {}
    
    result_type operator()()
    {
        return dis(gen);
    }

private:
    random_generator(random_generator const&) /*= delete*/;
    random_generator& operator=(random_generator const&) /*= delete*/;
};

inline coords_t generate_coordiantes(std::size_t n)
{
    random_generator<float> rg(n);
    coords_t coords;
    coords.reserve(n);
    for (decltype(n) i = 0; i < n; ++i)
    {
        coords.emplace_back(rg());
    }
    return std::move(coords);
}

inline points2d_t generate_points(std::size_t n)
{
    auto coords = generate_coordiantes(n * 2);
    points2d_t points;
    points.reserve(n);
    auto s = coords.size();
    for (decltype(s) i = 0; i < s; i += 2)
    {
        points.emplace_back(coords[i], coords[i + 1]);
    }
    return std::move(points);
}

inline boxes2d_t generate_boxes(std::size_t n)
{
    random_generator<float> rg(n);
    
    boxes2d_t boxes;
    boxes.reserve(n);
    for (decltype(n) i = 0; i < n; ++i)
    {
        auto const x = rg();
        auto const y = rg();
        boxes.emplace_back(x - 0.5f, y - 0.5f, x + 0.5f, y + 0.5f);
    }
    return std::move(boxes);   
}


//
// Benchmark running routines
//
struct result_info
{
    std::string step;
    double min;
    double max;
    std::size_t iterations;
    std::size_t operations;

    result_info(std::string step)
        : step(std::move(step))
        , min(-1), max(-1), iterations(0), operations(0)
    {}

    template <typename Timer>
    void set_mark(Timer const& t)
    {
        auto const m = t.elapsed();
        min = min < 0 ? m : (std::min)(m, min);
        max = max < 0 ? m : (std::max)(m, max);
    }
};

inline std::ostream& operator<<(std::ostream& os, result_info const& r)
{
    using std::get;
    os << r.iterations << " iterations of " << r.operations
       << " " << r.step << "(s) in " << r.min << " to " << r.max << " sec"
       << std::endl;
    return os;
}

template <typename Container, typename Operation>
inline result_info benchmark(std::string step, std::size_t iterations,
    Container const& objects, Operation op)
{
    result_info r(step);
    r.iterations = iterations;
    r.operations = objects.size(); // number of single step executions

    {
        util::high_resolution_timer t;
        for (decltype(iterations) i = 0U; i < iterations; ++i)
        {
            op(objects);
        }
        r.set_mark(t);
    }
    return std::move(r);
}

} // namespace sibench

#endif
