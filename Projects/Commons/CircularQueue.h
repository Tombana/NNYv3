template<class T> class CircularQueue {
private:
	volatile unsigned int m_read;
	volatile unsigned int m_write;
	const unsigned int m_size;
	volatile T *m_data;

public:
	CircularQueue(unsigned int size) : m_size(size) {
		m_read = 0;
		m_write = 0;
		m_data = new T[size];
	}
	
	~CircularQueue() {
		delete[] m_data;
	}
	
	//push a new element when not a reference
	bool tryPushRaw(T element) {
		return tryPush(element);
	}
	
	//push a new element in the circular queue
	bool tryPush(T &element) {
		int nextElement = (m_write + 1) % m_size;
		if(nextElement != m_read) {
			m_data[m_write] = element;
			m_write = nextElement;
			return true;
		} else {
			return false;
		}
	}

	//remove the next element from the circular queue
	bool tryPop(T &element) {
		if(m_read == m_write)
			return false;
		int nextElement = (m_read + 1) % m_size;
		element = m_data[m_read];
		m_read = nextElement;
		return true;
	}
};