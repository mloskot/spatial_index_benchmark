#include "spatial_index_benchmark.hpp"
#include <spatialindex/SpatialIndex.h>
using namespace std;
namespace si = SpatialIndex;

int main()
{
    try
    {
        auto n = 1000U;
        auto const boxes = sibench::generate_boxes(n);

        uint32_t index_capacity = 20;
        uint32_t leaf_capacity = index_capacity;
        uint32_t dimension = 2;
        si::id_type index_id;
        std::unique_ptr<si::IStorageManager> sm(si::StorageManager::createNewMemoryStorageManager());
        std::unique_ptr<si::ISpatialIndex> si(si::RTree::createNewRTree(*sm, 0.7, index_capacity,
            leaf_capacity, dimension, si::RTree::RV_RSTAR, index_id));

        typedef std::array<double, 2> coord_array_t;
        
        auto const s = boxes.size();
        for (std::size_t i = 0; i < s; ++i)
        {
            auto const& box = boxes[i];
            coord_array_t p1 = { std::get<0>(box), std::get<1>(box) };
            coord_array_t p2 = { std::get<2>(box), std::get<3>(box) };

            si::id_type item_id(i);
            si::Region region(si::Point(p1.data(), p1.size()), si::Point(p2.data(), p2.size()));
            si->insertData(0, nullptr, region, item_id);
        }
        
        return EXIT_SUCCESS;
    }
    catch ( ... )
    {
    }
    return EXIT_FAILURE;
}
