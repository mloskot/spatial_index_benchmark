//#include <cstdint>
#include <cstdlib>
#include <SpatialIndex/SpatialIndex.h>
#include "spatial_index_benchmark.hpp"
using namespace std;
namespace si = SpatialIndex;

int main()
{
    try
    {
        auto n = 10U;
        //auto const coords = sibench::generate_coordinates(n);
        auto const boxes = sibench::generate_boxes(n);

        uint32_t index_capacity = 20;
        uint32_t leaf_capacity = index_capacity;
        uint32_t dimension = 2;
        si::id_type index_id;
        si::IStorageManager* sm = si::StorageManager::createNewMemoryStorageManager();
        si::ISpatialIndex* si = si::RTree::createNewRTree(*sm, 0.7, index_capacity,
            leaf_capacity, dimension, si::RTree::RV_RSTAR, index_id);

        auto const s = boxes.size();
        for (std::size_t i = 0; i < s; ++i)
        {
            auto const& box = boxes[0];
            typedef std::array<double, 2> point_t;
            point_t p1 = { std::get<0>(box), std::get<1>(box) };
            point_t p2 = { std::get<2>(box), std::get<3>(box) };

            si::id_type item_id(i);
            si::Region region(si::Point(p1.data(), p1.size()), si::Point(p2.data(), p2.size()));
            si->insertData(0, nullptr, region, item_id);
        }
        delete si;
        delete sm;
        
        return EXIT_SUCCESS;
    }
    catch ( ... )
    {
    }
    return EXIT_FAILURE;
}
