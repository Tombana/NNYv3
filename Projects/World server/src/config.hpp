#ifndef H_CONFIG
#define H_CONFIG

//Settings
#define CONFIG_FILENAME             "world.conf"
#define CONFIG_DATABASE_SUPPORTED   1
//=================================================
//   DO NOT EDIT BELOW THIS LINE UNLESS YOU KNOW
//              WHAT YOU ARE DOING
//=================================================

//Version configuration (generated from main())
//#define BUILD_TIME                                  g_CONFIG.read<std::string>   ("BUILD_TIME")
//#define BUILD_NUMBER                                g_CONFIG.read<unsigned int>  ("BUILD_NUMBER")
//#define GIT_COMMIT_HASH                             g_CONFIG.read<std::string>   ("GIT_COMMIT_HASH")
//#define GIT_COMMIT_NUMBER                           g_CONFIG.read<unsigned int>  ("GIT_COMMIT_NUMBER")
//Server configuration
#define CONFIG_SERVER_ID                            g_CONFIG.read<unsigned int>  ("SERVER_ID")
#define CONFIG_SERVER_PORT                          g_CONFIG.read<unsigned int>  ("SERVER_PORT")
#define CONFIG_SERVER_KEY                           g_CONFIG.read<std::string>   ("SERVER_KEY")
//Realm configuration
#define CONFIG_REALM_IP                             g_CONFIG.read<std::string>   ("REALM_IP")
#define CONFIG_REALM_PORT                           g_CONFIG.read<unsigned int>  ("REALM_PORT")
#define CONFIG_REALMCONNECTOR_RETRY                 g_CONFIG.read<unsigned int>  ("REALMCONNECTOR_RETRY")
#define CONFIG_REALMCONNECTOR_RETRY_SLEEP           g_CONFIG.read<unsigned int>  ("REALMCONNECTOR_RETRY_SLEEP")
//MySQL informations
#define CONFIG_MYSQL_SERVER                         g_CONFIG.read<std::string>  ("MYSQL_SERVER").c_str()
#define CONFIG_MYSQL_USERNAME                       g_CONFIG.read<std::string>  ("MYSQL_USERNAME").c_str()
#define CONFIG_MYSQL_PASSWORD                       g_CONFIG.read<std::string>  ("MYSQL_PASSWORD").c_str()
#define CONFIG_MYSQL_DATABASE                       g_CONFIG.read<std::string>  ("MYSQL_DATABASE").c_str()
//Console error mode (static)
#define CONFIG_VERBOSE_NOTHING                      0
#define CONFIG_VERBOSE_IMPORTANT                    1
#define CONFIG_VERBOSE_DEBUGGING                    2
//ThreadPool configuration
#define CONFIG_THREADPOOL_DEFAULT_WORKER            g_CONFIG.read<unsigned int>  ("THREADPOOL_DEFAULT_WORKER")
#define CONFIG_THREADPOOL_ADD_WORKER                g_CONFIG.read<unsigned int>  ("THREADPOOL_ADD_WORKER")
#define CONFIG_THREADPOOL_MAX_WORKER                g_CONFIG.read<unsigned int>  ("THREADPOOL_MAX_WORKER")

//Loading configuration
#include "ConfigFile.h"
extern ConfigFile g_CONFIG;

#endif
