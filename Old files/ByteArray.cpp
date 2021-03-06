#include "ByteArray.h"

ByteArray::ByteArray() {
    init();
}

void ByteArray::init() {
    m_seek = 0;
    m_buffer = "";
}

//! Erase bytes from the byte array
/**
* @param pos position yourself somewhere in the buffer
* @param nb the amount of bytes to delete from that position
*/
void ByteArray::erase(int pos, int nb) {
    m_buffer.erase(pos, nb);
}

//! Special overloaded constructor
/** Is only created/used/returned in/by (ZSocket::)socket_read()
* @param array are the bytes read from the socket
* @param size are the amount of bytes read from the socket
*/
ByteArray::ByteArray(char *array, int &size) {
    init();
    if (size > 0) { //dont perform actions if size is negative or equals to zero
        const unsigned char *temp_array;
        temp_array = reinterpret_cast<const unsigned char*>(array);
        m_buffer.append(reinterpret_cast<const char*>(temp_array),size);
    }
}

//! Copy constructor, nothing surnatural here.
ByteArray::ByteArray(const ByteArray &obj) {
    m_buffer = obj.m_buffer;
    m_seek = obj.m_seek;
}

ByteArray::~ByteArray() {
    //destructor
}

//! Check if the seek is as the end of the byte array
/**
* @return
* TRUE if we are at the EndOfFile and there's nothing else to read \n
* FALSE if we are not at the EndOfFile and there is still something to read \n
*/
bool ByteArray::eof() {
    if (m_seek == size()) {
        return true; //the pointer is at the end
    } else {
        return false; //the pointer isnt at the end
    }
}

//! This function append a DWORD and a const char* to the byte array
void ByteArray::addString(const char *str) {
    std::string buffer = str;
    add<DWORD>((int)buffer.size());
    m_buffer.append(buffer);
}

//! This function append a DWORD and std::string to the byte array
void ByteArray::addString(std::string str) {
    add<DWORD>((int)str.size());
    m_buffer.append(str);
}

//! Clear the byte array entirely
void ByteArray::clear() {
    m_buffer.clear();
    m_seek = 0;
}

bool ByteArray::readBool() {
    if (read<BYTE>() == 1) {
        return true;
    } else {
        return false;
    }
}

void ByteArray::addBool(bool cst) {
    if (cst) {
        add<BYTE>(1); //true=1
    } else {
        add<BYTE>(0); //false=0
    }
}

//! Return the size of the byte array
DWORD ByteArray::size() {
    return (DWORD)m_buffer.size();
}

//! Wrapper to use the byte array is it really was an array @return BYTE
BYTE ByteArray::operator[](int i) {
    return (BYTE)m_buffer[i];
}

//! [Harmful] This function append a raw std::string to the byte array
void ByteArray::append(std::string &obj) {
    m_buffer.append(obj);
}

//! [Harmful] This function append another ByteArray to the byte array
void ByteArray::append(ByteArray &obj) {
    m_buffer.append(obj.getRaw());
}

//! Returns the byte array buffer @return std::string byte array buffer
std::string ByteArray::getRaw() {
    return m_buffer;
}

//! Generate a packet @return a ByteArray of a generated packet header + the byte array buffer
std::string ByteArray::genPacket() {
    //Packet begining [Byte]
    std::string output = "~";

    //Packet size [DWORD]
    DWORD size = m_buffer.size();
    output.append(reinterpret_cast<char*>(&size), sizeof(DWORD));

    //Packet content
    output.append(m_buffer);

    return output;
}

// Tombana: Use the templated read function!!!!
//Or use the right operators instead of the _pow stuff.
//DWORD ByteArray::readDword() {
//	DWORD val=0;
//	val |= ( (BYTE)m_buffer[m_seek+3] & 0xff ) << 24;
//	val |= ( (BYTE)m_buffer[m_seek+2] & 0xff ) << 16;
//	val |= ( (BYTE)m_buffer[m_seek+1] & 0xff ) << 8;
//	val |= ( (BYTE)m_buffer[m_seek] & 0xff );
//	m_seek+=4;
//	return val;
//}

//! Read a 'string' from the byte array
/** Actually it reads a \e _size DWORD, then reads \e _size amount of chars and put them into a std:string
@return std::string
*/
std::string ByteArray::readString() {
    std::string output;
    unsigned int length = read<DWORD>();
    output.append(m_buffer.substr(m_seek, length));
    m_seek+=length;
    return output;
}

//! Position the seek to somewhere else in the byte array
void ByteArray::setSeek(unsigned int newSeek) {
    m_seek = newSeek;
}

//! Modify sightly the position of the seek in the byte array
//! Can be negative
void ByteArray::modSeek(int newSeek) {
    m_seek += newSeek;
}

//! [Debugging] Prints (human-readable) the byte array
void ByteArray::printHex() {
    std::cerr << "[PrintHex]: (";
    unsigned int size = m_buffer.size();
    for (unsigned int i=0; i<size; i++) {
        if (i == size-1) {
            printf("%02X", (BYTE) m_buffer.at(i));
        } else {
            printf("%02X ", (BYTE) m_buffer.at(i));
        }
    }
    std::cerr << ")" << std::endl;
}
