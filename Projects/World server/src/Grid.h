#ifndef H_GRID
#define H_GRID

#include "HashTable.hpp"
#include "threadHandler.h"

//Maps ids are unsigned ints
//Grid x,y's are unsigned shorts

//Grid size; 100x100 grid. Note, it wont take more or less memory if you resize it.
//Just make sure you don't go over 65535. 65535x65535 equals the size of an iteger
#define GRID_SIZE 100
#define GRID_CELL_SIZE 200 //200x200 pixel each cells of the grid

//This class is thread-safe
class Grid {
    public:
    struct s_node {
        s_thread_data      *data;
        s_node             *next;
    };

    struct s_cell {
        pthread_mutex_t     mutex; //For when we add data to the linked list
        s_node             *head; //The linked list
    };

    struct s_handle {
        s_cell             *cell;
        s_thread_data      *owner;
        s_handle(s_thread_data *p_owner) : owner(p_owner) {}
    };

    typedef s_handle       *Handle;

    private:
    struct s_map {
        pthread_mutex_t     mutex; //For when we add or remove grid in the map
        HashTable<s_cell>   grid; //Our grid (A hash table of s_cell)
    };

	public:
		Grid();
       ~Grid();
        void                createMap(unsigned int mapID);
        Handle              createHandle(s_thread_data &data);
        Handle              subscribe(Handle &handle, unsigned int mapID, unsigned short grid_x, unsigned short grid_y);
        void                unsubscribe(Handle &handle, unsigned int mapID, unsigned short grid_x, unsigned short grid_y);
        void                send(Handle &handle, ByteArray &packet);
        unsigned short      getGridFromCoord(unsigned int xy);
	protected:
        //All protected functions aren't thread-safe
        void                _createGrid(unsigned int mapID, unsigned int gridID);
	private:
        HashTable<s_map>    m_maps;   //hash table of maps
        pthread_mutex_t     m_x_maps; //mutex for the hash table
};

#endif
