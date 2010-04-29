#ifndef H_HASHTABLE
#define H_HASHTABLE

#include <iostream>
#include <cstdlib> //NULL is defined here

//This hash table uses at the moment: "separate chaining"
//----------
// Another solution I had implemented (then removed) is "coalesced hashing/chaining".
// The major probrem with this solution is it have a limited size.
// For the moment I would rather chose "separate chaining" over "coalesced hashing/chaining" or  "open addressing".
//----
// Coalesced chaining is a lesser known strategy that forms a hybrid of
// "separate chaining" and "open addressing". In a separately chained hash table,
// items that hash to the same index are placed on a list at that index (linked list).
// This can result in a great deal of wasted memory because the table itself
// must be large enough to maintain a load factor that performs well.
//----------

//Keys are unsigned int's only
//Also it cannot have two entry with the same key.

//Amount of buckets in the table, must be a prime number
//See Documentation/1000 first primes.txt
#define HASHTABLE_PRIME 5

//Note: HASHTABLE_PRIME cannot be greather than 65535 (short)(maximum size of an array)

template <class T>
class HashTable {
    struct s_node{
        unsigned int  key;
        T             value;
        s_node       *next;
    };

	public:
		 HashTable();
		~HashTable();

        void            insert(unsigned int key, T& value);
        void            remove(unsigned int key);
        float           loadFactor();
        void            flush();
        bool            exists(unsigned int key);
        T&              operator[](unsigned int key);

	protected:
        unsigned int hash(unsigned int key);

	private:
        s_node       *m_table[HASHTABLE_PRIME];
        unsigned int  m_entries; //nb of entries in total
};

template <class T>
HashTable<T>::HashTable() {
    for (unsigned int i=0; i<HASHTABLE_PRIME; i++) {
        m_table[i] = NULL;
    }
    m_entries = 0;
}

template <class T>
HashTable<T>::~HashTable() {
    flush();
}

template <class T>
void HashTable<T>::flush() {
    for (unsigned int index=0; index<HASHTABLE_PRIME; index++) {
        if (m_table[index] != NULL) {
            s_node *ptr = m_table[index];
            s_node *ptr_last = NULL;
            while (ptr->next != NULL) {
                ptr_last = ptr;
                ptr = ptr->next;
                delete ptr_last;
                m_entries--;
            }
            delete ptr;
            m_entries--;
            m_table[index] = NULL;
        }
    }
}

template <class T>
bool HashTable<T>::exists(unsigned int key) {
    unsigned int index = hash(key);
    s_node *ptr = m_table[index];
    if (ptr != NULL) {
        while (ptr->key != key) {
            ptr = ptr->next;
            if (ptr == NULL) return false;
        }
        if (ptr->key == key) return true;
    }
    return false;
}

template <class T>
T& HashTable<T>::operator[](unsigned int key) {
    unsigned int index = hash(key);
    s_node *ptr = m_table[index];
    if (ptr != NULL) {
        while (ptr->key != key) {
            ptr = ptr->next;
            if (ptr == NULL) break;
        }
    }
    if (ptr == NULL) {
        std::cerr << "[HashTable] @ERROR: Crash! This key " << key << " doesn't exist in the array!" << std::endl;
    } else {
        return ptr->value;
    }
}

template <class T>
float HashTable<T>::loadFactor() {
    return (float)m_entries/HASHTABLE_PRIME;
}

template <class T>
void HashTable<T>::remove(unsigned int key) {
    unsigned int index = hash(key);
    s_node *ptr = m_table[index];
    s_node *ptr_last = NULL;
    while (ptr->key != key) {
        ptr_last = ptr;
        ptr = ptr->next;
    }
    if (ptr_last == NULL) { //first in list
        m_table[index] = ptr->next;
        delete ptr;
    } else { //in list
        if (ptr->next == NULL) { //last in last
            ptr_last->next = NULL;
            delete ptr;
        } else { //in the middle
            ptr_last->next = ptr->next;
            delete ptr;
        }
    }
    m_entries--;
}

template <class T>
void HashTable<T>::insert(unsigned int key, T& value) {
    //Use hash function to generate table index
    unsigned int index = hash(key);

    //Check if this bucket is empty
    if (m_table[index] == NULL) {
        //no more work needed, create the node
        m_table[index]        = new s_node;
        m_table[index]->key   = key;
        m_table[index]->value = value;
        m_table[index]->next  = NULL;
    } else {
        //This bucket already is busy; insert the node between root & 1st element
        s_node *old           = m_table[index];
        m_table[index]        = new s_node;
        m_table[index]->key   = key;
        m_table[index]->value = value;
        m_table[index]->next  = old;
    }

    m_entries++;
}

template <class T>
unsigned int HashTable<T>::hash(unsigned int key) {
     return key % HASHTABLE_PRIME;
}

#endif
