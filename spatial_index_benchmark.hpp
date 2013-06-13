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
#if (_MSC_VER == 1700)
#define _VARIADIC_MAX 6
#endif
#define NOMINMAX
#endif // _MSC_VER
#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <chrono>
#include <exception>
#include <fstream>
#include <iostream>
#include <iomanip>
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
std::size_t const max_iterations = 1000000;
std::size_t const max_capacities = 100;
std::size_t const max_insertions = max_iterations;
std::size_t const max_queries =  std::size_t(max_insertions * 0.1);

enum class rtree_variant { rstar, linear, quadratic };

inline std::pair<rtree_variant, std::string> get_rtree_split_variant()
{
#ifdef SIBENCH_RTREE_SPLIT_LINEAR
    return std::make_pair(rtree_variant::linear, "L");
#elif SIBENCH_RTREE_SPLIT_QUADRATIC
    return std::make_pair(rtree_variant::quadratic, "Q");
#elif SIBENCH_RTREE_SPLIT_RSTAR
    return std::make_pair(rtree_variant::rstar, "R");
#else
#error Unknown rtree split algorithm
#endif
}

inline std::pair<rtree_variant, std::string> get_rtree_load_variant()
{
#ifdef SIBENCH_RTREE_LOAD_ITR
    return std::make_pair(rtree_variant::linear, "ITR");
#elif SIBENCH_RTREE_LOAD_BLK
    return std::make_pair(rtree_variant::quadratic, "BLK");
#else
#error Unknown rtree loading method
#endif
}

inline std::string get_banner(std::string const& lib)
{
    return lib 
        + "(" 
        + get_rtree_split_variant().second 
        + ','
        + get_rtree_load_variant().second
        + ")";
}

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
        , gen(1) // generate the same succession of results for everyone 
                 // (unsigned int)std::chrono::system_clock::now().time_since_epoch().count())
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

inline coords_t generate_coordinates(std::size_t n)
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
    auto coords = generate_coordinates(n * 2);
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
    std::size_t min_capacity;
    std::size_t max_capacity;
    std::size_t iterations;

    result_info(char const* step = "")
        : step(step)
        , min(-1), max(-1), min_capacity(0), max_capacity(0), iterations(0)
    {}

    void accumulate(result_info const& r)
    {
        min = min < 0 ? r.min : (std::min)(min, r.min);
        max = max < 0 ? r.max :  (std::max)(max, r.max);

        assert(min <= max);
    }

    template <typename Timer>
    void set_mark(Timer const& t)
    {
        auto const m = t.elapsed();
        min = min < 0 ? m : (std::min)(m, min);
        max = max < 0 ? m : (std::max)(m, max);

        assert(min <= max);
    }
};

inline std::ostream& operator<<(std::ostream& os, result_info const& r)
{
    os << r.step << " " << r.iterations << " in " << r.min << " to " << r.max << " sec"
       << std::endl;
    return os;
}

inline std::ostream& print_result(std::ostream& os, std::string const& lib, result_info const& r)
{
    os << std::setw(15) << std::setfill(' ') << std::left
       << sibench::get_banner(lib) << ": " << r;
    return os;
}

inline std::ostream& print_result(std::ostream& os, std::string const& /*lib*/, result_info const& load, result_info const& query)
{
    assert(load.min_capacity == load.min_capacity);
    assert(query.max_capacity == query.max_capacity);

    std::streamsize wn(5), wf(10);
    os << std::left << std::setfill(' ') << std::fixed << std::setprecision(6)
       << std::setw(wn) << load.max_capacity
       << std::setw(wn) << load.min_capacity 
       << std::setw(wf) << load.min
       << std::setw(wf) << query.min
       << std::endl;
    return os;
}

inline std::ostream& print_result_header(std::ostream& os, std::string const& lib)
{
    std::streamsize const wn(5), wf(10), vn(2);
    os << sibench::get_banner(lib) << ' ' << std::setw(wn * vn + wf * vn) << std::setfill('-') << ' ' << std::endl;
    os << std::left << std::setfill(' ')
       << std::setw(wn * vn) << "capacity" << std::setw(wf) << "load (s)" << std::setw(wf) << "query (s)"
       << std::endl;
    return os;
}

#if 0
// min and max
inline std::ostream& print_result(std::ostream& os, std::string const& /*lib*/, result_info const& load, result_info const& query)
{
    assert(load.min_capacity == load.min_capacity);
    assert(query.max_capacity == query.max_capacity);

    std::streamsize wn(5), wf(10);
    os << std::left << std::setfill(' ') << std::fixed << std::setprecision(6)
       << std::setw(wn) << load.max_capacity
       << std::setw(wn) << load.min_capacity 
       << std::setw(wf) << load.min
       << std::setw(wf) << load.max
       << std::setw(wf) << query.min
       << std::setw(wf) << query.max
       << std::endl;
    return os;
}
inline std::ostream& print_result_header(std::ostream& os, std::string const& lib)
{
    std::streamsize const wn(5), wf(10), vn(2);
    os << sibench::get_banner(lib) << ' ' << std::setw(wn * vn + wf * vn * vn) << std::setfill('-') << ' ' << std::endl;
    os << std::left << std::setfill(' ')
       << std::setw(wn * vn) << "capacity" << std::setw(wf * vn) << "load (s)" << std::setw(wf * vn) << "query (s)"
       << std::endl;
    os << std::left << std::setfill(' ')
       << std::setw(wn) << "max" << std::setw(wn) << "min"
       << std::setw(wf) << "min" << std::setw(wf) << "max"
       << std::setw(wf) << "min" << std::setw(wf) << "max"
       << std::endl;
    return os;
}
#endif

inline std::ostream& print_query_count(std::ostream& os, std::string const& lib, size_t i)
{
    os << sibench::get_banner(lib) << " stats: found=" << i << std::endl;
    return os;
}

template <typename Container, typename Operation>
inline result_info benchmark(char const* step, std::size_t iterations,
    Container const& objects, Operation op)
{
    result_info r(step);
    r.iterations = iterations;
    {
        util::high_resolution_timer t;
        op(objects, iterations);
        r.set_mark(t);
    }
    return std::move(r);
}

} // namespace sibench

#endif
