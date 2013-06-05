//
// Copyright (C) 2013 Mateusz Loskot <mateusz@loskot.net>
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy 
// at http://www.boost.org/LICENSE_1_0.txt)
//
#include "spatial_index_benchmark.hpp"
#include <spatialindex/SpatialIndex.h>
using namespace std;
namespace si = SpatialIndex;

namespace {
void print_statistics(std::ostream& os, std::string const& lib, si::ISpatialIndex const& i)
{
    si::IStatistics* pstat = nullptr;
    i.getStatistics(&pstat);
    std::unique_ptr<si::IStatistics> stat(pstat);

    os << sibench::get_banner(lib)
       << " stats: reads=" << stat->getReads()
       << ", writes=" << stat->getWrites()
       << ", nodes=" << stat->getNumberOfNodes()
       << ", ndata=" << stat->getNumberOfData()
       << std::endl;
}

si::RTree::RTreeVariant get_variant()
{
    auto const rv = sibench::get_rtree_variant().first;
    switch(rv)
    {
    case sibench::rtree_variant::linear:
        return si::RTree::RV_LINEAR;
    case sibench::rtree_variant::quadratic:
        return si::RTree::RV_QUADRATIC;
    case sibench::rtree_variant::rstar:
        return si::RTree::RV_RSTAR;
    default:
        throw std::runtime_error("unknown rtree variant");
    };
}
}

int main()
{
    try
    {
        std::string const lib("lsi");
        
        // Generate random objects for indexing
        auto const boxes = sibench::generate_boxes(sibench::max_objects);

        // Set up index
        si::RTree::RTreeVariant const variant = get_variant();
        uint32_t const index_capacity = 100; // default: 100
        uint32_t const leaf_capacity = 100; // default: 100
        double const fill_factor = 0.5; // default: 0.7 // TODO: does it mean index_capacity * fill_factor?
        uint32_t const dimension = 2;
        si::id_type index_id;
        std::unique_ptr<si::IStorageManager> sm(si::StorageManager::createNewMemoryStorageManager());
        std::unique_ptr<si::ISpatialIndex> rtree(si::RTree::createNewRTree(*sm,
            fill_factor, index_capacity, leaf_capacity, dimension, variant, index_id));

        // Benchmark: insert
        {
            auto const marks = sibench::benchmark("insert", boxes,
                [&rtree] (sibench::boxes2d_t const& boxes) {

                    typedef std::array<double, 2> coord_array_t;
                    auto s = boxes.size();
                    for (decltype(s) i = 0; i < s; ++i)
                    {
                        auto const& box = boxes[i];
                        coord_array_t p1 = { std::get<0>(box), std::get<1>(box) };
                        coord_array_t p2 = { std::get<2>(box), std::get<3>(box) };

                        si::id_type item_id(i);
                        si::Region region(
                            si::Point(p1.data(), p1.size()),
                            si::Point(p2.data(), p2.size()));
                        rtree->insertData(0, nullptr, region, item_id);
                    }
            });
            sibench::print_result(std::cout, lib, marks);
        }
        
        print_statistics(std::cout, lib, *rtree);

        // Benchmark: query
        //{
        //    // TODO: benchmark() should forward query parameter, not boxes
        //    auto const marks = sibench::benchmark("insert", sibench::max_iterations, boxes,
        //        [&rtree] (sibench::boxes2d_t const& /*boxes*/) {
        //            ; // TODO
        //    });
        //    sibench::print_result(std::cout, lib, marks);
        //}
        
        return EXIT_SUCCESS;
    }
    catch (Tools::Exception& e)
    {
        std::cerr << e.what() << std::endl;
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
