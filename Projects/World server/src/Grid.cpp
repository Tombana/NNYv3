#include "Grid.h"

//===================================
//           CONSTRUCTOR
//===================================
Grid::Grid() {
    m_x_maps = PTHREAD_MUTEX_INITIALIZER;
}

//===================================
//           DESTRUCTOR
//===================================
Grid::~Grid() {
}

//===================================
//           CREATE MAP
// Public and thread-safe
//===================================
void Grid::createMap(unsigned int mapID) {
    //Create an empty map structure
    s_map map;
    map.mutex = PTHREAD_MUTEX_INITIALIZER;
    //Lock the whole big table because we are adding a map
    pthread_mutex_lock(&m_x_maps);
        //Insert the map we created to the table
        m_maps.insert(mapID, map);
    //Unlock mutex, we are done
    pthread_mutex_unlock(&m_x_maps);
}

//===================================
//       CREATE GRID [protected]
// This function is not thread-safe
//===================================
void Grid::_createGrid(unsigned int mapID, unsigned int gridID) {
    //Create a cell (that holds the linkedlist and a mutex)
    s_cell cell;
    cell.mutex = PTHREAD_MUTEX_INITIALIZER; //is the mutex
    cell.head  = NULL; //is the linked list head
    m_maps[mapID].grid.insert(gridID, cell);
}

//===================================
//            SUBSCRIBE
// Public and thread-safe
//===================================
void Grid::subscribe(unsigned int mapID, unsigned short grid_x, unsigned short grid_y, s_thread_data *data) {
    //Compute coordinates
    unsigned int gridID = grid_y*GRID_SIZE + grid_x;

    //Create a grid if it doesn't exists yet
    pthread_mutex_lock(&m_maps[mapID].mutex);
        if (!m_maps[mapID].grid.exists(gridID)) {
            _createGrid(mapID, gridID);
        }
    pthread_mutex_unlock(&m_maps[mapID].mutex);

    //Append our data to the linked list in this cell
    pthread_mutex_lock(&m_maps[mapID].grid[gridID].mutex);
    if (m_maps[mapID].grid[gridID].head == NULL) {
        //first entry; create and set the node
        m_maps[mapID].grid[gridID].head       = new s_node;
        m_maps[mapID].grid[gridID].head->data = data;
        m_maps[mapID].grid[gridID].head->next = NULL;
    } else {
        //not the first entry
        s_node *old = m_maps[mapID].grid[gridID].head;
        //create and set the node
        m_maps[mapID].grid[gridID].head       = new s_node;
        m_maps[mapID].grid[gridID].head->data = data;
        m_maps[mapID].grid[gridID].head->next = old;
    }
    pthread_mutex_unlock(&m_maps[mapID].grid[gridID].mutex);
}

//===================================
//            SUBSCRIBE
// Public and NOT thread-safe
// TODO: thread-safety
//===================================
void Grid::unsubscribe(unsigned int mapID, unsigned short grid_x, unsigned short grid_y, s_thread_data *data) {
    //Compute coordinates
    unsigned int gridID = grid_y*GRID_SIZE + grid_x;

    s_node *ptr = m_maps[mapID].grid[gridID].head;
    s_node *ptr_last = NULL;

    while (ptr->data != data) {
        ptr_last = ptr;
        ptr = ptr->next;
        if (ptr == NULL) break;
    }

    if (ptr != NULL && ptr->data == data) {
        if (ptr_last == NULL) { //we are the first in the list
            if (ptr->next != NULL) { //with other people behind us
                m_maps[mapID].grid[gridID].head = ptr->next;
            } else { //with nobody behind us
                m_maps[mapID].grid.remove(gridID);
            }
        } else { //we arnt the first in the list
            if (ptr->next == NULL) { //but we are the last one in the list
                ptr_last->next = NULL;
            } else { //with other people behind us
                ptr_last->next = ptr->next;
            }
        }

        delete ptr;
    } else {
        std::cerr << "[Grid] @ERROR: Unable to unsubscribe; entry not found!" << std::endl;
    }
}
