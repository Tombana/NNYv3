#ifndef H_PACKET
#define H_PACKET

#include <iostream> //TODO: temp printHex()
#include <string>
#include "../Resources/resTypedef.h"

//TODO: We should inline some of theses functions.
//A assembly 'CALL' for theses small functions seems a little too much.
//I'm pretty sure Visual C++ will optimise this class but I'm not sure about all compilers.
//Hence in programming, coders are the rulers, I will make sure this class is inlined everytime :)
//I'm just too lazy to do it right now, ahah :]
class Packet {
public:
    //Constructors/destructors
    Packet(); //default constructor
    Packet(const Packet &obj); //copy constructor
    Packet(BYTE *new_array, unsigned short size); //Socket constructor (with content)
    ~Packet();
    //std::string wrapper
    DWORD size();
    void clear();
    BYTE operator[](int i);
    void erase(int pos, int nb);
    //Creating packets
    void addBool(bool cst);
    void addString(std::string str);
    void addString(const char *str);
    //Reading packets
    bool readBool();
    std::string readString();

    //Tombana: Use this like: packet.read<int>(); and packet.read<DWORD>();
    template <typename T>
    T read(){
        if((m_seek + sizeof(T)) > m_buffer.size()) return T(); //This is the same as return 0; but it will also work for when T is a class
        T val = *reinterpret_cast<T*>((BYTE*)m_buffer.c_str()+m_seek);
        m_seek += sizeof(T);
        return val;
    }

    template <typename T>
    void add(T hex) {
        m_buffer.append(reinterpret_cast<char*>(&hex), sizeof(T));
    }

    void printHex();
    bool eof();
	//Processing packets
	std::string genStringPacket();
	//Advanced commands:
	//TODO: I don't recommend using any of the following unless you know what you are doing
	void appendPacket(Packet &obj);
    std::string& getRefBuffer(); //[Harmful]
    void setSeek(unsigned int newSeek); //[Harmful]
    void backSeek(unsigned int modSeek); //[Harmful]
    
private:
    std::string m_buffer;
    unsigned int m_seek;
};

#endif
