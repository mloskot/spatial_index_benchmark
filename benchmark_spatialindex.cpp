#include <cstdint>
#include <cstdlib>
#include <spatialindex/SpatialIndex.h>
using namespace std;
using namespace SpatialIndex;

int main()
{
    try
    {
        id_type identifier;
        uint32_t index_capacity = 20;
        uint32_t leaf_capacity = index_capacity;
        uint32_t dimension = 2;
        IStorageManager* sm = StorageManager::createNewMemoryStorageManager();
        ISpatialIndex* si = RTree::createNewRTree(*sm, 0.7, index_capacity, leaf_capacity, dimension, SpatialIndex::RTree::RV_RSTAR, identifier);
    
        delete si;
        delete sm;
        
        return EXIT_SUCCESS;
    }
    catch ( ... )
    {
    }
    return EXIT_FAILURE;
}
