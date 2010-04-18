//General configuration
#define CONFIG_SERVER_PORT                  6132
#define CONFIG_DATABASE_SUPPORTED           3
#define CONFIG_VERBOSE                      1 //[0]Nothing, [1]Important stuff, [2]Debugging

//ThreadPool configuration
#define CONFIG_THREADPOOL_DEFAULT_WORKER    5 //Default first thread workers
#define CONFIG_THREADPOOL_ADD_WORKER        20 //Amount of threads to create when we are running out
#define CONFIG_THREADPOOL_MAX_WORKER        500 //Maximum workers/clients allowed to be created
//TODO (NitriX#): The following thing is to be implemented, otherwise, threads never get destroyed even if no more needed
//#define CONFIG_THREADPOOL_THRESHOLD       10 //Keep only <threshold value> spare threads in cache, kill all others

//ZSocket configuration
#define MAX_PENDING_CONNECTION              10

//MySQL informations
#define CONFIG_MYSQL_SERVER                 "localhost"
#define CONFIG_MYSQL_USERNAME               "root"
#define CONFIG_MYSQL_PASSWORD               "d9klmav5hrya0"
#define CONFIG_MYSQL_DATABASE               "nnyv3_v3"

//Easier typedefs
typedef unsigned char  BYTE;  //1 byte
typedef unsigned short WORD;  //2 bytes
typedef unsigned long  DWORD; //4 bytes

//Debugging purpose, use: sleep(x)
#ifdef WIN32
    #include <windows.h>
    #define sleep(x) Sleep(x * 1000)
#else
    #include <unistd.h>
#endif
