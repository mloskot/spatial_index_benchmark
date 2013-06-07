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

#define SIBENCH_RTREE_LOAD_BULK

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

struct query_visitor : public si::IVisitor
{
   query_visitor() : m_io_index(0), m_io_leaf(0), m_io_found(0) {}

    void visitNode(si::INode const& n)
    {
        n.isLeaf() ? ++m_io_leaf : ++m_io_index;
    }

    void visitData(si::IData const& d)
    {
        si::IShape* ps = nullptr;
        d.getShape(&ps);
        std::unique_ptr<si::IShape> shape(ps);
        ; // use shape

        // Region is represented as array of characters
        uint8_t* pd = 0;
        uint32_t size = 0;
        d.getData(size, &pd);
        //std::unique_ptr<uint8_t[]> data(pd);
        // use data
        //std::string str(reinterpret_cast<char*>(pd));

        //cout << d.getIdentifier() << endl; // ID is query answer
        ++m_io_found;
    }

    void visitData(std::vector<si::IData const*>& v)
    {
        // TODO
        assert(!v.empty()); (void)v;
        //cout << v[0]->getIdentifier() << " " << v[1]->getIdentifier() << endl;
    }

    size_t m_io_index;
    size_t m_io_leaf;
    size_t m_io_found;
};

#ifdef SIBENCH_RTREE_LOAD_BULK
struct data_stream : public si::IDataStream
{
    data_stream(sibench::boxes2d_t const& boxes)
        : boxes(boxes), next(0), pdnext(nullptr)
    {
        get_next();
    }
    ~data_stream()
    {
        delete pdnext;
    }
    
    si::IData* getNext()
    {
        if (!pdnext) return 0;
        si::RTree::Data* pcurrent = pdnext;
        get_next();
        return pcurrent;
    }

    bool hasNext()
    {
        return pdnext != nullptr;
    }

    uint32_t size()
    {
        return boxes.size();
    }

    void rewind()
    {
        if (pdnext)
        {
            delete pdnext;
            pdnext = nullptr;
        }
        next = 0;
    }

    void get_next()
    {
        pdnext = nullptr;
        if (next < boxes.size())
        {            
            auto const& box = boxes[next];
            double low[2] = { std::get<0>(box), std::get<1>(box) };
            double high[2] =  { std::get<2>(box), std::get<3>(box) };
            si::Region region(low, high, 2);
            si::id_type id(next);
            pdnext = new si::RTree::Data(sizeof(double), reinterpret_cast<byte*>(low), region, id);
            ++next;
        }

    }

    sibench::boxes2d_t const& boxes;
    sibench::boxes2d_t::size_type next;
    si::RTree::Data* pdnext;

private:
    data_stream(data_stream const&); /*=delete*/
    data_stream& operator=(data_stream const&); /*=delete*/
};
}
#endif // SIBENCH_RTREE_LOAD_BULK

int main()
{
    try
    {
        std::string const lib("lsi");
        
        // Generate random objects for indexing
        auto const boxes = sibench::generate_boxes(sibench::max_insertions);       

        // Set up index
        typedef std::array<double, 2> coord_array_t;
        si::RTree::RTreeVariant const variant = get_variant();
        uint32_t const index_capacity = 100; // default: 100
        uint32_t const leaf_capacity = 100; // default: 100
        double const fill_factor = 0.5; // default: 0.7 // TODO: does it mean index_capacity * fill_factor?
        uint32_t const dimension = 2;
        si::id_type index_id;
        std::unique_ptr<si::IStorageManager> sm(si::StorageManager::createNewMemoryStorageManager());
#ifdef SIBENCH_RTREE_LOAD_BULK
        std::unique_ptr<si::ISpatialIndex> rtree;
        // Benchmark: insert
        {
            si::IStorageManager* psm = sm.get();
            auto const marks = sibench::benchmark("insert", boxes.size(), boxes,
                [&rtree, &psm, fill_factor, index_capacity, leaf_capacity, dimension, variant, &index_id] (sibench::boxes2d_t const& boxes, std::size_t /*iterations*/)
            {
                data_stream dstream(boxes);
                std::unique_ptr<si::ISpatialIndex> rtree_tmp(si::RTree::createAndBulkLoadNewRTree(si::RTree::BLM_STR,
                    dstream, *psm, fill_factor, index_capacity, leaf_capacity, dimension, variant, index_id));
                rtree = std::move(rtree_tmp);
            });
            sibench::print_result(std::cout, lib, marks);

            print_statistics(std::cout, lib, *rtree);
        }
#else
        std::unique_ptr<si::ISpatialIndex> rtree(si::RTree::createNewRTree(*sm,
            fill_factor, index_capacity, leaf_capacity, dimension, variant, index_id));

        // Benchmark: insert
        {
            auto const marks = sibench::benchmark("insert", boxes.size(), boxes,
                [&rtree] (sibench::boxes2d_t const& boxes, std::size_t iterations)
            {
                auto const s = iterations < boxes.size() ? iterations : boxes.size();
                for (size_t i = 0; i < s; ++i)
                {
                    auto const& box = boxes[i];
                    coord_array_t const p1 = { std::get<0>(box), std::get<1>(box) };
                    coord_array_t const p2 = { std::get<2>(box), std::get<3>(box) };
                    si::Region region(
                        si::Point(p1.data(), p1.size()),
                        si::Point(p2.data(), p2.size()));
                    si::id_type item_id(i);
                    rtree->insertData(0, nullptr, region, item_id);
                }
            });
            sibench::print_result(std::cout, lib, marks);

            print_statistics(std::cout, lib, *rtree);
        }
#endif
        // Benchmark: query
        {
            size_t query_found = 0;

            auto const marks = sibench::benchmark("query", sibench::max_queries, boxes,
                [&rtree, &query_found] (sibench::boxes2d_t const& boxes, std::size_t iterations)
            {
                for (size_t i = 0; i < iterations; ++i)
                {
                    auto const& box = boxes[i];
                    coord_array_t const p1 = { std::get<0>(box) - 10, std::get<1>(box) - 10 };
                    coord_array_t const p2 = { std::get<2>(box) + 10, std::get<3>(box) + 10 };
                    si::Region region(
                        si::Point(p1.data(), p1.size()),
                        si::Point(p2.data(), p2.size()));
                    query_visitor qvisitor;
                    rtree->intersectsWithQuery(region, qvisitor);

                    query_found += qvisitor.m_io_found;
                }
            });
            sibench::print_result(std::cout, lib, marks);
            sibench::print_query_count(std::cout, lib, query_found);
        }
        
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
