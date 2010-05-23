#ifndef H_CONFIG_HPP
#define H_CONFIG_HPP

#include "config_static.h"

//=================================================
//   DO NOT EDIT BELOW THIS LINE UNLESS YOU KNOW
//              WHAT YOU ARE DOING
//=================================================

//Server configuration
#define CONFIG_SERVER_ID                            g_CONFIG.read<unsigned int>  ("SERVER_ID")
#define CONFIG_SERVER_PORT                          g_CONFIG.read<unsigned int>  ("SERVER_PORT")
#define CONFIG_SERVER_KEY                           g_CONFIG.read<std::string>   ("SERVER_KEY")
#define CONFIG_MULTIPLE_LOGGING_ALLOWED             g_CONFIG.read<bool>          ("MULTIPLE_LOGGING_ALLOWED")
//Realm configuration
#define CONFIG_REALM_IP                             g_CONFIG.read<std::string>   ("REALM_IP")
#define CONFIG_REALM_PORT                           g_CONFIG.read<unsigned int>  ("REALM_PORT")
//MySQL informations
#define CONFIG_MYSQL_SERVER                         g_CONFIG.read<std::string>  ("MYSQL_SERVER").c_str()
#define CONFIG_MYSQL_USERNAME                       g_CONFIG.read<std::string>  ("MYSQL_USERNAME").c_str()
#define CONFIG_MYSQL_PASSWORD                       g_CONFIG.read<std::string>  ("MYSQL_PASSWORD").c_str()
#define CONFIG_MYSQL_DATABASE                       g_CONFIG.read<std::string>  ("MYSQL_DATABASE").c_str()
//ThreadPool configuration
#define CONFIG_THREADPOOL_DEFAULT_WORKER            g_CONFIG.read<unsigned int>  ("THREADPOOL_DEFAULT_WORKER")
#define CONFIG_THREADPOOL_ADD_WORKER                g_CONFIG.read<unsigned int>  ("THREADPOOL_ADD_WORKER")
#define CONFIG_THREADPOOL_MAX_WORKER                g_CONFIG.read<unsigned int>  ("THREADPOOL_MAX_WORKER")

//Loading configuration
#include "ConfigFile.h"
extern ConfigFile g_CONFIG;

#endif
