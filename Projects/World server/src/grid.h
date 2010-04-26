#ifndef H_GRID
#define H_GRID

#include <map>
#include "threadHandler.h"

namespace grid {
    void createMap(unsigned int mapID);
    void addLeaf(unsigned int mapID, unsigned int grid_x, unsigned int grid_y, s_thread_data *data);

    struct s_gridBox {
      unsigned int x;
      unsigned int y;
      s_gridBox(const unsigned int A,const unsigned int B) :
        x(A),y(B) {}
    };

    struct s_gridLeaf {
      s_thread_data    *data;
      s_gridLeaf       *next;
    };

    class GridCompare  {
        public:
        bool operator()
        (const s_gridBox& A, const s_gridBox& B)
        const { return A.x<B.x; }
    };

    typedef std::map<s_gridBox,s_gridLeaf,GridCompare> Grid;

    struct s_map {
        pthread_mutex_t   mutex;
        Grid              grid;
    };

    typedef std::map<unsigned int,s_map> WorldMaps;

}
#endif
