#ifndef H_CONFIG
#define H_CONFIG

//Database version supported
#define CONFIG_DATABASE_VERSION_REALM 1 //Version supported by the Realm server
#define CONFIG_DATABASE_VERSION_WORLD 1 //Version supported by the World server

//Realm connector configuration
#define CONFIG_REALMCONNECTOR_MAX_RETRY			0  //Maximum attempts to reconnect until taking a break.
#define CONFIG_REALMCONNECTOR_SLEEPTIME			10 //How long that break will be

//World configuration
#define CONFIG_ALLOW_MULTIPLE_LOGIN	  0 //0 = false | 1 = true

#endif