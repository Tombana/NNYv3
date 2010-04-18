#include "main.h"

//--------------- GLOBAL SECTION -----------------
//nothing yet o.o

int main() {
    //DWORD* wohoo[0xFFFF];

    /*
    std::string str;
    std::ifstream configFile("config.ini");
    while (!configFile.eof()) {
        str.clear();
        configFile>>str;
        if (str.size() > 0) {
            std::cout << "--> " << str << std::endl;
        }
    }
    configFile.close();
    */

    //TODO (NitriX#): read config from config.ini
    //TODO (NitriX#): Implement a Timer so it d/c servers when they are ping timed out

    //signal(SIGINT,leaving); signal(SIGTERM,leaving);
    //signal(SIGILL,leaving); signal(SIGABRT,leaving);

    //TODO (NitriX#): I added too much DWORD everywhere... somes could be 'unsigned int' as well, sorry :/

    //TODO (NitriX#): We needs a function that reloads the worldserver list from DB

    //TODO (NitriX#1#): Omg log system! :<

    //TODO (NitriX#0#): What with packets hash? CRC32 please...
    //CCRC32 ccrc32;
    //std::string test = "Omfg we rules :D";
    //unsigned long hash = ccrc32.FullCRCString(test);
    //std::cout << "Hash CRC32: " << hash << std::endl;

    //HEADER MESSAGE
    std::cout <<
    "/===============================================\\" << std::endl <<
    "|         ~ NNYv3 Realm Server ~" << "\t\t|" << std::endl <<
    "|\t\t\t\t\t\t|" << std::endl <<
    "| Date: " << BUILD_DATE << "    \t\t|" << std::endl <<
    "| Version: " << BUILD_VERSION << " [Build " << BUILD_NUMBER << "]\t\t\t|" << std::endl <<
    "| Database supported: v" << DATABASE_SUPPORTED << "\t\t\t|" << std::endl <<
    "|===============================================|" << std::endl <<
    "| Recent modifications: \t\t\t|" << std::endl <<
    "| + Realm Server is now fully working\t\t|" << std::endl <<
    "| + capsuleHdl_sync is now done\t\t\t|" << std::endl <<
    "| + capsuleHdl_updater is now done\t\t|" << std::endl <<
    "| + A much cleaner capsuleHandler\t\t|" << std::endl <<
    //"| + updater:: has been removed from sources\t|" << std::endl <<
    //"| + Optimization; BYTE, WORD & DWORD added\t|" << std::endl <<
    //"| + Fixed threadHandler(); has been re-coded\t|" << std::endl <<
    //"| + Fixed threadData; memory protection added\t|" << std::endl <<
    //"| + TODOs added a bit everywhere in sources\t|" << std::endl <<
    //"| + Web panel performance issue; disabled\t|" << std::endl <<
    //"| + Web administration panel implemented\t|" << std::endl <<
    //"| + Worldservers dispatcher implemented\t\t|" << std::endl <<
    //"| + Fixed pthread_create(); s_thread_data added\t|" << std::endl <<
    //"| + Added MySQL library support \t\t|" << std::endl <<
    //"| + Added autoversioning support \t\t|" << std::endl <<
    //"| + Added byte arrays support \t\t\t|" << std::endl <<
    //"| + Added multi-threading support \t\t|" << std::endl <<
    //"| + Added ZSocket library \t\t\t|" << std::endl <<
    "\\===============================================/" << std::endl;
    //--------- Initiate database ------------
    Database database;
    database.connect();
    checkDatabase(database);

    //--------- Initiate dispatcher ------------
    Dispatcher dispatcher(database);
    //dispatcher.printServers();

    //--------- Starting HTTP Panel ------------
    //TODO (NitriX#): A HTTP Panel would be nice, dont you think ^^
    //Disabled for now, slow without a threadpool implementation
    /*
    std::cout << "Starting Administration Panel...";
    pthread_t pthread_httpHandler;
    int rcx;
    rcx = pthread_create(&pthread_httpHandler, NULL, httpHandler, NULL);
    rcx = pthread_detach(pthread_httpHandler);
    if (rcx) {
        std::cout << "@ERROR: pthread: pthread_create() failed!" << std::endl;
    }
    std::cout << "OK!" << std::endl;
    */

    //--------- Listening server ------------
    std::cout << "[main] Listening on port 6131... ";
    ZSocket socket; //Creating main server socket
    if (socket.socket_bind(6131)) {
        std::cout << "OK!" << std::endl << std::endl;
    } else {
        std::cout << "Failed!" << std::endl << "@ERROR: Unable to bind socket on port 6131" << std::endl;
        exit(EXIT_FAILURE);
    }

    //--------- STARTING SERVER ------------
    std::cout << "Realm Server started!" << std::endl;
    std::cout << "Waiting for connections..." << std::endl;
    //TODO (NitriX#): What about a ThreadPool?
    while (true) {
        pthread_t pthread; //New thread
        //======== THREAD DATA ==========
        //Waiting for clients, accept client from main socket and copy socket id into threadData.
        s_thread_data threadData; //Create a data structure that will be sent to the thread when its created
        threadData.socketID                 =  socket.socket_acceptClient(); //Oh yeah, the thread will need the client's socket
        threadData.database                 = &database;
        threadData.dispatcher               = &dispatcher;
        threadData.memory_protect_mutex     =  PTHREAD_MUTEX_INITIALIZER;    //Also a memory protection...
        threadData.memory_protect_signal    =  PTHREAD_COND_INITIALIZER;     //Another memory protection...
        threadData.memory_flag_done_copying =  false;                        //And one more memory protection...
        //===============================
        //Launching thread and send the threadData structure memory address as parameters
        int rc;
        rc = pthread_create(&pthread, NULL, threadHandler, &threadData); //here we send the s_thread_data address to the thread
        rc = pthread_detach(pthread); //Detach thread so it works on its own
            //Now we waits for the thread to copy threadData to some local vars...
            //else if we continue processing and threadHandler isnt done copying,
            //we are gonna get REALLY mean errors! Lets just avoid that and wait a bit =]

            //-- Waiting for focus; usually acquire immediatly
            //just some sort of security I guess (recommended from pthread documentation)
            pthread_mutex_lock(&threadData.memory_protect_mutex);

            //-- Check if the condition flag is still false
            //I mean, are we done copying? If so, dont waste our time anymore and jump...
            while(!threadData.memory_flag_done_copying)
                //-- Nah we arnt done copying, pause the thread and wait for a signal
                pthread_cond_wait(&threadData.memory_protect_signal, &threadData.memory_protect_mutex);
                //-- We got THE signal and we are back to life; what now?
                //-- Oh is the flag changed :D !? Loop and test the flag one more time.
                //-- (Normally, the flag has been sucessfully changed at this point... but we never know)

            //-- ... jump to here. Good. Release focus; normal processing from now :)
            pthread_mutex_unlock(&threadData.memory_protect_mutex);
        if (rc) {
            std::cout << "[main] @ERROR: pthread: pthread_create() failed!" << std::endl;
        }
    }

    return 0;
}

void checkDatabase(Database &database) {
    //----- Execute the query
    DB_RESULT result = database.query(DB_USE_RESULT, "SELECT db_version FROM config");
    DB_ROW    rows   = database.fetch_row(result);
    //----- Do stuff we wants with rows[]
    unsigned int db_version = atoi(rows[0]);
    //----- Free ressources as soon as possible
    database.queryDone();
    //----- Process the new data :)
    std::cout << "[main] Checking database version... ";
    if (db_version == DATABASE_SUPPORTED) {
        std::cout << "OK!" << std::endl;
    } else {
        std::cout << "Failed!" << std::endl;
        std::cout << "@ERROR: Your current database version is not supported!" << std::endl;
        std::cout << "@ERROR: Please install a clean 'fulldb' or run 'updates' from SQL folder." << std::endl;
        exit(EXIT_FAILURE);
    }
}

/*
void leaving(int sig) {
    std::cout << "Leaving!! Signal #" << sig << "! OMFG!" << std::endl;
    exit(EXIT_SUCCESS);
}
*/
