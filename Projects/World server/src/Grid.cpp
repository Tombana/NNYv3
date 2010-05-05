#include "Grid.h"

//===================================
//           CONSTRUCTOR
//===================================
Grid::Grid() {
  pthread_mutex_init(&m_x_maps, NULL);
  //m_x_maps = PTHREAD_MUTEX_INITIALIZER;
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
    pthread_mutex_init(&map.mutex, NULL);
    //map.mutex = PTHREAD_MUTEX_INITIALIZER;
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
    pthread_mutex_init(&cell.mutex, NULL);
    //cell.mutex = PTHREAD_MUTEX_INITIALIZER; //is the mutex
    cell.head  = NULL; //is the linked list head
    m_maps[mapID].grid.insert(gridID, cell);
}

//===================================
//            SUBSCRIBE
// Public and thread-safe
//===================================
Grid::Handle Grid::subscribe(Handle &handle, unsigned int mapID, unsigned short grid_x, unsigned short grid_y) {
    //Compute coordinates
    unsigned int gridID = grid_y*GRID_SIZE + grid_x;

    //Create a grid if it doesn't exists yet
    pthread_mutex_lock(&m_maps[mapID].mutex); //mutex 1
        if (!m_maps[mapID].grid.exists(gridID)) {
            _createGrid(mapID, gridID);
        }

    //Append our data to the linked list in this cell
    pthread_mutex_lock(&m_maps[mapID].grid[gridID].mutex); //mutex 2
    pthread_mutex_unlock(&m_maps[mapID].mutex); //release mutex 1
    if (m_maps[mapID].grid[gridID].head == NULL) {
        //first entry; create and set the node
        m_maps[mapID].grid[gridID].head       = new s_node;
        m_maps[mapID].grid[gridID].head->data = handle->owner;
        m_maps[mapID].grid[gridID].head->next = NULL;
    } else {
        //not the first entry
        s_node *old = m_maps[mapID].grid[gridID].head;
        //create and set the node
        m_maps[mapID].grid[gridID].head       = new s_node;
        m_maps[mapID].grid[gridID].head->data = handle->owner;
        m_maps[mapID].grid[gridID].head->next = old;
    }
    pthread_mutex_unlock(&m_maps[mapID].grid[gridID].mutex); //release mutex 2

    s_handle *temp = new s_handle(handle->owner);
    temp->cell = &m_maps[mapID].grid[gridID];
    return temp;
}

//===================================
//           SUBSCRIBE
// Public and thread-safe
//===================================
void Grid::unsubscribe(Handle &handle, unsigned int mapID, unsigned short grid_x, unsigned short grid_y) {
    //Compute coordinates
    unsigned int gridID = grid_y*GRID_SIZE + grid_x;

    pthread_mutex_lock(&m_maps[mapID].mutex); //mutex 1
    pthread_mutex_lock(&m_maps[mapID].grid[gridID].mutex); //mutex 2
    pthread_mutex_unlock(&m_maps[mapID].mutex); //release mutex 1

    s_node *ptr = m_maps[mapID].grid[gridID].head;
    s_node *ptr_last = NULL;

    while (ptr->data != handle->owner) {
        ptr_last = ptr;
        ptr = ptr->next;
        if (ptr == NULL) break;
    }

    if (ptr != NULL && ptr->data == handle->owner) {
        if (ptr_last == NULL) { //we are the first in the list
            if (ptr->next != NULL) { //with other people behind us
                m_maps[mapID].grid[gridID].head = ptr->next;
            } else { //with nobody behind us
                pthread_mutex_unlock(&m_maps[mapID].grid[gridID].mutex); //release mutex 2 cuz its gonna be deleted
                m_maps[mapID].grid.remove(gridID);
                return;
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

    pthread_mutex_unlock(&m_maps[mapID].grid[gridID].mutex); //release mutex 2
}

//===================================
//           CREATE HANDLE
// No need thread-safety
//===================================
Grid::Handle Grid::createHandle(s_thread_data &data) {
    s_handle *handle = new s_handle(&data);
    return handle;
}

//===================================
//            SEND
// Public and thread-safe
//===================================
void Grid::send(Handle &handle, ByteArray &packet) {
    s_node *ptr = handle->cell->head;

    pthread_mutex_lock(&handle->cell->mutex);
    while (ptr != NULL) {
        if (ptr->data != handle->owner) ptr->data->socket << packet;
        ptr = ptr->next;
    }
    pthread_mutex_unlock(&handle->cell->mutex);
}
