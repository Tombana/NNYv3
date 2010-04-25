#ifndef H_GLOBAL_MODELS
#define H_GLOBAL_MODELS

//HERE'S THE TYPEDEFS AND STRUCTURES WE USES A BIT
//EVERYWHERE FOR THE GLOBAL SHARED VARIABLES. I GOT
//PISSED OFF AND CREATED A FILE. HERE WE GO!
//~NitriX

//=============================
//     EASIERS TYPEDEFS
//=============================
typedef unsigned char  BYTE;  //1 byte
typedef unsigned short WORD;  //2 bytes
typedef unsigned long  DWORD; //4 bytes

//=============================
//      MAP GRID SYSTEM
//=============================
struct s_mapGrid_coord {
  unsigned int x;
  unsigned int y;
  s_mapGrid_coord(const unsigned int A,const unsigned int B) :
    x(A),y(B) {}
};

class MapGridcompare  {
    public:
    bool operator()
    (const s_mapGrid_coord& A, const s_mapGrid_coord& B)
    const { return A.x<B.x; }
};

typedef std::map<s_mapGrid_coord,unsigned int,MapGridcompare> MapGrid;

#endif
