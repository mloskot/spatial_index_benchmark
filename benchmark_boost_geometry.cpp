#define BOOST_GEOMETRY_INDEX_DETAIL_ENABLE_DEBUG_INTERFACE
#include "spatial_index_benchmark.hpp"
#include <boost/geometry/index/rtree.hpp>
#include <boost/geometry/index/detail/rtree/visitors/print.hpp>
#include <boost/geometry/index/detail/rtree/visitors/statistics.hpp>
using namespace std;
namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

int main()
{
    try
    {
        typedef bg::model::point<double, 2, bg::cs::cartesian> point_t;
        typedef bg::model::box<point_t> box_t;
        typedef bgi::rtree<box_t, bgi::rstar<100>> rtree_t;
        
        auto print_status = [](sibench::result_info const& r)
        {
            std::cout << "boost.geometry: " << r;
        };

        auto print_statistics = [](rtree_t const& i)
        {            
            auto const stats = bgi::statistics(i);
            std::cout << "boost.geometry statistics: levels=" << get<0>(stats)
                    << ", nodes=" << get<1>(stats)
                    << ", leaves=" << get<2>(stats)
                    << ", values=" << get<3>(stats)
                    << ", values_min=" << get<4>(stats)
                    << ", values_max=" << get<5>(stats)
                    << std::endl;
        };
        
        // Generate random objects for indexing
        auto const boxes = sibench::generate_boxes(sibench::max_objects);

        // Set up index
        // TODO
        
        rtree_t rtree;

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
            print_status(marks);
        }
        
        //std::cout <<  rtree;
        
        print_statistics(rtree);

        // Benchmark: query
        {
            // TODO: benchmark() should forward query parameter, not boxes
            auto const marks = sibench::benchmark("insert", sibench::max_iterations, boxes,
                [&rtree] (sibench::boxes2d_t const& /*boxes*/) {
                    ; // TODO
            });
            print_status(marks);
        }


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
