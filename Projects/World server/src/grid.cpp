#include "grid.h"

extern grid::WorldMaps g_worldMaps;

namespace grid {
    //===================================
    //           CREATE MAP
    //===================================
    //WARNING: This function isn't thread-safe, uses it only before starting threads!
    void createMap(unsigned int mapID) {
        //Create an empty map structure
        s_map emptyMap;
        //Initialise the mutex on our map
        emptyMap.mutex = PTHREAD_MUTEX_INITIALIZER;
        //Insert our map to the WorldMaps.
        //Btw, no need mutexes here because maps are created before threads are started
        g_worldMaps.insert(WorldMaps::value_type(mapID,emptyMap));
    }

    //===================================
    //      ADD LEAF (TO GRID TO MAP)
    //===================================
    void addLeaf(unsigned int mapID, unsigned int grid_x, unsigned int grid_y, s_thread_data *data) {
        //Acquire map mutex
        pthread_mutex_lock(&g_worldMaps[mapID].mutex);

        //Create box structure filled with coordinates
        s_gridBox box(grid_x, grid_y);

        //STD doc: if <box> does not match the key of any element in the container,
        //the function inserts a new element <s_gridLeaf> with that key and
        //returns a reference to its mapped value.
        //-----
        //So here we create an empty leaf if there's none yet
        g_worldMaps[mapID].grid[box];

        //[...] more code here

        //Release map mutex
        pthread_mutex_unlock(&g_worldMaps[mapID].mutex);

        //---------

        //grid.insert(MapGrid::value_type(nano1,101));
        //g_worldMaps[1].insert(MapGrid::value_type(nano2,102));
        //TODO (NitriX#): We are testing mapGrid here
        //s_mapGrid_coord nano_test(21,782268);
        //g_mapGrid.insert(MapGrid::value_type(nano_test,666));
        //std::cerr << "Wohoo!: " << g_mapGrid[nano_test] << std::endl;
    }
}
