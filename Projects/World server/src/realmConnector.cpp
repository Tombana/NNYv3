#include "realmConnector.h"

extern pthread_mutex_t g_realmConnector_mutex; //MUTEX! :)
extern pthread_cond_t  g_realmConnector_cond; //Protected with g_realmConnector_mutex
extern bool            g_realmConnector_authorized; //Protected with g_realmConnector_mutex

void *realmConnector (void *ptr) {
    int error = 0;

    while (true) {
        ZSocket realm;
        realm.socket_create();

        if (realm.socket_connect(CONFIG_REALM_IP, CONFIG_REALM_PORT)) {
            //---
            ByteArray packetToSend;
            packetToSend.addCmd(PCKT_W_SYNC_KEY);
            packetToSend.add<DWORD>(CONFIG_SERVER_ID);
            packetToSend.addString(CONFIG_SERVER_KEY);
            //---
            realm << packetToSend;
        }

        while (true) {
            ByteArray input;
            realm >> input;
            if (!realm.isConnected()) break;

            //input.readByte(); //Header
            //input.readDword(); //Size
            //input.readByte(); //Cmd_nbs
            input.modSeek(6); //Optimization

            while (true) { //a loop to parse all CMDs in the capsule
                switch (input.read<WORD>()) {
                //=====================================
                case PCKT_X_DEBUG:
                    input.readString();
                    break;
                case PCKT_R_WELCOME:
                    break;
                case PCKT_R_SYNC_KEY_ACK:
                    g_realmConnector_authorized = input.readBool();
                    pthread_cond_signal(&g_realmConnector_cond);
                    break;
                default:
                    //std::cerr << "[realmConnector] Unknow packet! STOP!" << std::endl;
                    input.setSeek(input.size()); //Goto EOF so it breaks to loop
                    break;
                //=====================================
                }

                if (input.eof()) break; //break the loop, no more CMDs
            }
        }

        //std::cout << "[realmConnector] We have been disconnected! Reconnecting..." << std::endl;
        if (error == CONFIG_REALMCONNECTOR_RETRY) {
            sleep(CONFIG_REALMCONNECTOR_RETRY_SLEEP);
        } else {
            error++;
        }
    }
}
