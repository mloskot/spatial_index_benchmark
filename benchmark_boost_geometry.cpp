//
// Copyright (C) 2013 Mateusz Loskot <mateusz@loskot.net>
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy 
// at http://www.boost.org/LICENSE_1_0.txt)
//
#define BOOST_GEOMETRY_INDEX_DETAIL_ENABLE_DEBUG_INTERFACE
#include "spatial_index_benchmark.hpp"
#include <boost/geometry/index/rtree.hpp>
#ifdef BOOST_GEOMETRY_INDEX_DETAIL_ENABLE_DEBUG_INTERFACE
#include <boost/geometry/index/detail/rtree/visitors/statistics.hpp>
#include <boost/geometry/index/detail/rtree/visitors/print.hpp>
#endif
using namespace std;
namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

template <typename T>
void print_statistics(std::ostream& os, std::string const& lib, T const& i)
{
    auto const stats = bgi::statistics(i);
    os << sibench::get_banner(lib)
#ifdef BOOST_GEOMETRY_INDEX_DETAIL_ENABLE_DEBUG_INTERFACE
       << " stats: levels=" << std::get<0>(stats)
       << ", nodes=" << get<1>(stats)
       << ", leaves=" << get<2>(stats)
       << ", values=" << get<3>(stats)
       << ", values_min=" << get<4>(stats)
       << ", values_max=" << get<5>(stats)
#else
       << "no statistics"
#endif
       << std::endl;
}

int main()
{
    try
    {
#ifdef BGI_DYNAMIC_RTREE
        std::string const lib("bgi_rt");
#else
        std::string const lib("bgi");
#endif
        
        // Generate random objects for indexing
        auto const boxes = sibench::generate_boxes(sibench::max_objects);

        // Set up index
        std::size_t const max_capacity = 100;
        std::size_t const min_capacity =  50; // default: max * 0.3

        typedef bg::model::point<double, 2, bg::cs::cartesian> point_t;
        typedef bg::model::box<point_t> box_t;
#ifdef BGI_DYNAMIC_RTREE
    #ifdef RTREE_VARIANT_LINEAR
        typedef bgi::dynamic_linear rtree_parameters_t;
    #elif RTREE_VARIANT_QUADRATIC
        typedef bgi::dynamic_quadratic rtree_parameters_t;
    #else
        typedef bgi::dynamic_rstar rtree_parameters_t;
    #endif
    typedef bgi::rtree<box_t, rtree_parameters_t> rtree_t;
    rtree_t rtree(rtree_parameters_t(max_capacity, min_capacity));
#else
    #ifdef RTREE_VARIANT_LINEAR
        typedef bgi::rtree<box_t, bgi::linear<max_capacity, min_capacity>> rtree_t;
    #elif RTREE_VARIANT_QUADRATIC
        typedef bgi::rtree<box_t, bgi::quadratic<max_capacity, min_capacity>> rtree_t;
    #else
        typedef bgi::rtree<box_t, bgi::rstar<max_capacity, min_capacity>> rtree_t;
    #endif    
    rtree_t rtree;
#endif

        // Benchmark: insert
        {
            auto const marks = sibench::benchmark("insert", boxes,
                [&rtree] (sibench::boxes2d_t const& boxes) {

                    auto s = boxes.size();
                    for (decltype(s) i = 0; i < s; ++i)
                    {
                        auto const& box = boxes[i];
                        point_t p1(std::get<0>(box), std::get<1>(box));
                        point_t p2(std::get<2>(box), std::get<3>(box));
                        box_t region(p1, p2);
                        rtree.insert(region);
                    }
            });
            sibench::print_result(std::cout, lib, marks);
        }

        //std::cout <<  rtree;
        print_statistics(std::cout, lib, rtree);

        //// Benchmark: query
        //{
        //    // TODO: benchmark() should forward query parameter, not boxes
        //    auto const marks = sibench::benchmark("insert", sibench::max_iterations, boxes,
        //        [&rtree] (sibench::boxes2d_t const& /*boxes*/) {
        //            ; // TODO
        //    });
        //    print_status(marks);
        //}


        return EXIT_SUCCESS;
    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "unknown error" << std::endl;
    }
    return EXIT_FAILURE;
}
