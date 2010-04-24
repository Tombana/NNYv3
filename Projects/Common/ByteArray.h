#ifndef H_BYTEARRAY
#define H_BYTEARRAY

#include <iostream>
#include <string.h>
#include <math.h>
#include "protocol.hpp"

//Specific typedefs
typedef unsigned char  BYTE;  // 1byte
typedef unsigned short WORD;  // 2bytes
typedef unsigned long  DWORD; //4bytes

long double _pow(int base, int exponent);

class ByteArray {
public:
    //Constructors/destructors
    ByteArray();
    ByteArray(const ByteArray &obj);
    ByteArray(char *array, int &size);
    ~ByteArray();
    //std::string wrapper
    DWORD size();
    void clear();
    BYTE operator[](int i);
    void erase(int pos, int nb);
    //Creating packets
    void addCmd(WORD hex);
    void addByte(BYTE hex);
    void addBool(bool cst);
    void addWord(WORD hex);
    void addDword(DWORD hex);
    void addString(std::string str);
    void addString(const char *str);
    //Reading packets
    BYTE readByte();
    bool readBool();
    WORD readWord();
    DWORD readDword(); //not working
    std::string readString();

	//Tombana: Use this like: packet.read<int>(); and packet.read<DWORD>();
		template <typename T>
		T read(){
			if( (m_seek + sizeof(T)) > m_buffer.size() ) return T(); //This is the same as return 0; but it will also work for when T is a class
			T val = *reinterpret_cast<T*>((BYTE*)m_buffer.c_str()+m_seek);
			m_seek += sizeof(T);
			return val;
		}
		//template specialisation: packet.read<std::string>()
		template <>
		std::string read(){ return readString(); }

    void setSeek(unsigned int newSeek);
    void modSeek(int newSeek);
    //Processing packets
    std::string genPacket();
    std::string getRaw();
    void append(ByteArray &obj);
    void append(std::string &obj);
    void printHex();
    bool eof();

private:
    void init();

    std::string m_buffer;
    DWORD m_seek;
    DWORD m_nbCmd;
};

#endif
