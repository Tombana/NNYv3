#ifndef H_BYTEARRAY
#define H_BYTEARRAY

#include <iostream>
#include <string.h>
#include <math.h>
#include "protocol.hpp"
#include "config.hpp"

#define BYTEARRAY_VERSION "1.1"

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
    void addString(std::string &str);
    void addString(const char *str);
    //Reading packets
    BYTE readByte();
    bool readBool();
    WORD readWord();
    DWORD readDword();
    std::string readString();
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
