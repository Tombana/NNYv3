#ifndef H_CONFIG
#define H_CONFIG

//Settings
#define CONFIG_VERBOSE   2
#define CONFIG_FILENAME  "world.conf"

//=================================================
//   DO NOT EDIT BELOW THIS LINE UNLESS YOU KNOW
//              WHAT YOU ARE DOING
//=================================================

//Server configuration
#define CONFIG_SERVER_ID                            g_CONFIG.read<unsigned int>  ("SERVER_ID")
#define CONFIG_SERVER_PORT                          g_CONFIG.read<unsigned int>  ("SERVER_PORT")
#define CONFIG_SERVER_KEY                           g_CONFIG.read<std::string>   ("SERVER_KEY")
#define CONFIG_ZSOCKET_MAX_PENDING_CONNECTION       g_CONFIG.read<unsigned int>  ("ZSOCKET_MAX_PENDING_CONNECTION")
//Realm configuration
#define CONFIG_REALM_IP                             g_CONFIG.read<std::string>   ("REALM_IP")
#define CONFIG_REALM_PORT                           g_CONFIG.read<unsigned int>  ("REALM_PORT")
#define CONFIG_REALMCONNECTOR_RETRY                 g_CONFIG.read<unsigned int>  ("REALMCONNECTOR_RETRY")
#define CONFIG_REALMCONNECTOR_RETRY_SLEEP           g_CONFIG.read<unsigned int>  ("REALMCONNECTOR_RETRY_SLEEP")
//Console error mode
#define CONFIG_VERBOSE_NOTHING                      0
#define CONFIG_VERBOSE_IMPORTANT                    1
#define CONFIG_VERBOSE_DEBUGGING                    2
//ThreadPool configuration
#define CONFIG_THREADPOOL_DEFAULT_WORKER            g_CONFIG.read<unsigned int>  ("THREADPOOL_DEFAULT_WORKER")
#define CONFIG_THREADPOOL_ADD_WORKER                g_CONFIG.read<unsigned int>  ("THREADPOOL_ADD_WORKER")
#define CONFIG_THREADPOOL_MAX_WORKER                g_CONFIG.read<unsigned int>  ("THREADPOOL_MAX_WORKER")
//ZSocket configuration
#define CONFIG_ZSOCKET_MAX_PENDING_CONNECTION       g_CONFIG.read<unsigned int>  ("ZSOCKET_MAX_PENDING_CONNECTION")

//Loading configuration
#include "ConfigFile.h"
extern ConfigFile g_CONFIG;

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

#endif
