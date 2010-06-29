#ifndef H_SHAREDMODELS
#define H_SHAREDMODELS

//=============================
//        EASY TYPEDEFS
//=============================
typedef unsigned char  BYTE;  //1 byte  : up to 0xFF
typedef unsigned short WORD;  //2 bytes : up to 0xFFFF
typedef unsigned long  DWORD; //4 bytes : up to 0xFFFFFFFF

//=============================
//    MORE COMPLEX TYPEDEFS
//=============================
typedef BYTE  ACK;      //Packet's acknowledgement codes
typedef WORD  CMD;	    //Packet's command codes
typedef WORD  PORT;		//A port will never be bigger than 65535

#endif
