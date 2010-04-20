//General configuration
#define CONFIG_SERVER_PORT                  6132

//Realm configuration
#define CONFIG_REALM_IP                     "127.0.0.1"
#define CONFIG_REALM_PORT                   6131
#define CONFIG_SERVER_ID                    1
#define CONFIG_SERVER_KEY                   "SDFXqjs7nX"
#define CONFIG_REALMCONNECTOR_RETRY         3 //Retry 3 times before setting a sleep delay
#define CONFIG_REALMCONNECTOR_RETRY_SLEEP   10 //Sleep 10 seconds, then retry again; (looping)

//#define CONFIG_DATABASE_SUPPORTED         3
#define CONFIG_VERBOSE                      1
#define CONFIG_VERBOSE_NOTHING              0
#define CONFIG_VERBOSE_IMPORTANT            1
#define CONFIG_VERBOSE_DEBUGGING            2

//ThreadPool configuration
#define CONFIG_THREADPOOL_DEFAULT_WORKER    10 //Default first thread workers
#define CONFIG_THREADPOOL_ADD_WORKER        20 //Amount of threads to create when we are running out
#define CONFIG_THREADPOOL_MAX_WORKER        500 //Maximum workers/clients allowed to be created
//TODO (NitriX#): The following thing is to be implemented, otherwise, threads never get destroyed even if no more needed
//#define CONFIG_THREADPOOL_THRESHOLD       10 //Keep only <threshold value> spare threads in cache, kill all others

//ZSocket configuration
#define CONFIG_ZSOCKET_MAX_PENDING_CONNECTION  10
#define CONFIG_ZSOCKET_READ_BUFFER_SIZE        1024 //1024 bytes

//MySQL informations
//#define CONFIG_MYSQL_SERVER               "localhost"
//#define CONFIG_MYSQL_USERNAME             "root"
//#define CONFIG_MYSQL_PASSWORD             "*********"
//#define CONFIG_MYSQL_DATABASE             "nnyv3_v3"

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
