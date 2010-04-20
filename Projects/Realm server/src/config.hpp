#ifndef H_CONFIG
#define H_CONFIG

#define CONFIG_DATABASE_SUPPORTED 3

//MySQL informations
#define CONFIG_MYSQL_SERVER                         g_CONFIG.read<std::string>  ("MYSQL_SERVER").c_str()
#define CONFIG_MYSQL_USERNAME                       g_CONFIG.read<std::string>  ("MYSQL_USERNAME").c_str()
#define CONFIG_MYSQL_PASSWORD                       g_CONFIG.read<std::string>  ("MYSQL_PASSWORD").c_str()
#define CONFIG_MYSQL_DATABASE                       g_CONFIG.read<std::string>  ("MYSQL_DATABASE").c_str()
//Server configuration
#define CONFIG_SERVER_PORT                          g_CONFIG.read<unsigned int>  ("SERVER_PORT")
//ZSocket configuration
#define CONFIG_ZSOCKET_MAX_PENDING_CONNECTION       g_CONFIG.read<unsigned int>  ("ZSOCKET_MAX_PENDING_CONNECTION")
#define CONFIG_ZSOCKET_READ_BUFFER_SIZE             1024

//Loading configuration
#include "ConfigFile.h"
extern ConfigFile g_CONFIG;

//Specific typedefs
typedef unsigned char  BYTE;  // 1byte
typedef unsigned short WORD;  // 2bytes
typedef unsigned long  DWORD; //4bytes

#endif
