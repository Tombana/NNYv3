#ifndef H_CONFIG_H
#define H_CONFIG_H

//Main settings
#define CONFIG_FILENAME                             "world.conf"
#define CONFIG_DATABASE_SUPPORTED                   4

//Acceptor class
#define CONFIG_ACCEPTOR_MAX_PENDING_CONNECTION      10

//ThreadRealmConnector class
#define CONFIG_REALMCONNECTOR_MAX_RETRY             1  //Retry 1 time before taking a little nap (sleep delay)
#define CONFIG_REALMCONNECTOR_RETRY_SLEEP           5  //Sleep for 5 seconds, then retry again

#endif
