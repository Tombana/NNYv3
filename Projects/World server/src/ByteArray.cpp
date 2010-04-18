#include "ByteArray.h"

ByteArray::ByteArray() {
    init();
}

void ByteArray::init() {
    m_seek = 0;
    m_nbCmd = 0;
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
    m_nbCmd = obj.m_nbCmd;
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

//! This function append a BYTE to the byte array
void ByteArray::addByte(BYTE hex) {
    char buffer[1];
    buffer[0] = (char)(hex %256);
    m_buffer.append(buffer, 1);
}

//! This function append a DWORD and a const char* to the byte array
void ByteArray::addString(const char *str) {
    std::string buffer = str;
    addDword((int)buffer.size());
    m_buffer.append(buffer);
}

//! This function append a DWORD and std::string to the byte array
void ByteArray::addString(std::string &str) {
    addDword((int)str.size());
    m_buffer.append(str);
}

//! Clear the byte array entirely
void ByteArray::clear() {
    m_buffer.clear();
    m_nbCmd = 0;
    m_seek = 0;
}

bool ByteArray::readBool() {
    if (readByte() == 1) {
        return true;
    } else {
        return false;
    }
}

void ByteArray::addBool(bool cst) {
    if (cst) {
        addByte(1); //true=1
    } else {
        addByte(0); //false=0
    }
}

//! Return the size of the byte array
DWORD ByteArray::size() {
    return m_buffer.size();
}

//! Wrapper to use the byte array is it really was an array @return BYTE
BYTE ByteArray::operator[](int i) {
    return m_buffer[i];
}

//! This function append a \e WORD to the byte array
/**
* It basically performs a call to addWord() but \b also increment \e m_nbCmd. \n
* This is usefull later on when we will generate a packet with our ByteArray
* because we need to know how many \e commands has been appended.
*/
void ByteArray::addCmd(WORD hex) {
    m_nbCmd++;
    addWord(hex);
}

//! This function append a WORD to the byte array
void ByteArray::addWord(WORD hex) {
    char buffer[2];
    buffer[0] = (char)(hex %256);
    buffer[1] = (char)((int)(hex / pow(256,1)) %256);
    m_buffer.append(buffer,2);
}

//! This function append a DWORD to the byte array
void ByteArray::addDword(DWORD hex) {
    char buffer[4];
    buffer[0] = (char)(hex %256);
    buffer[1] = (char)((int)(hex / pow(256,1)) %256);
    buffer[2] = (char)((int)(hex / pow(256,2)) %256);
    buffer[3] = (char)((int)(hex / pow(256,3)) %256);
    m_buffer.append(buffer, 4);
}

//! [Harmful] This function append a raw std::string to the byte array
void ByteArray::append(std::string &obj) {
    m_buffer.append(obj);
}

//! [Harmful] This function append another ByteArray to the byte array
void ByteArray::append(ByteArray &obj) {
    m_buffer.append(obj.getBuffer());
}

//! Returns the byte array buffer @return std::string byte array buffer
std::string ByteArray::getBuffer() {
    return m_buffer;
}

//! Generate a packet @return a ByteArray of a generated packet header + the byte array buffer
std::string ByteArray::getPacket() {
    //Packet begining [Byte]
    std::string output = "~";

    //Packet size [DWORD]
    int bufferSize = m_buffer.size();
    char buffer[4];
    buffer[0] = (char)((int)bufferSize %256);
    buffer[1] = (char)((int)(bufferSize / pow(256,1)) %256);
    buffer[2] = (char)((int)(bufferSize / pow(256,2)) %256);
    buffer[3] = (char)((int)(bufferSize / pow(256,3)) %256);
    output.append(buffer,4); //packet size

    //Packet nbCmd [Byte]
    output += (char)((int)m_nbCmd %256);

    //Packet content
    output.append(m_buffer);

    return output;
}

//! Read 1 byte from the byte array @return BYTE
BYTE ByteArray::readByte() {
    //This is REALLY optimized, i know its confusing a bit but trust me
    m_seek+=1;
    return (BYTE)m_buffer[m_seek-1];
}

//! Read 2 bytes from the byte array @return WORD
WORD ByteArray::readWord() {
    WORD val=0;
    val+=(DWORD)pow(256, 1) * (BYTE)m_buffer[m_seek+1];
    val+=(BYTE)m_buffer[m_seek];
    m_seek+=2;
    return val;
}

//! Read 4 bytes from the byte array @return DWORD
DWORD ByteArray::readDword() {
    DWORD val=0;
    val+=(DWORD)pow(256, 3) * (BYTE)m_buffer[m_seek+3];
    val+=(DWORD)pow(256, 2) * (BYTE)m_buffer[m_seek+2];
    val+=(DWORD)pow(256, 1) * (BYTE)m_buffer[m_seek+1];
    val+=(BYTE)m_buffer[m_seek];
    m_seek+=4;
    return val;
}

//! Read a 'string' from the byte array
/** Actually it reads a \e _size DWORD, then reads \e _size amount of chars and put them into a std:string
@return std::string
*/
std::string ByteArray::readString() {
    std::string output;
    DWORD length = readDword();
    output.append(m_buffer.substr(m_seek, length));
    m_seek+=length;
    return output;
}

//! Position the seek to somewhere else in the byte array
void ByteArray::setSeek(DWORD newSeek) {
    m_seek = newSeek;
}

//! Modify sightly the position of the seek in the byte array
void ByteArray::modSeek(int newSeek) {
    m_seek += newSeek;
	}

//! [Debugging] Prints (human-readable) the byte array
void ByteArray::printHex() {
    std::cout << "[PrintHex]: (";
    int size = m_buffer.size();
    for (int i=0; i<size; i++) {
        printf("%2.2X ", m_buffer.at(i));
    }
    std::cout << ")" << std::endl;
}
