#include "ThreadRealmConnector.h"

ThreadRealmConnector::ThreadRealmConnector() {
    m_errorCounter = 0;
    m_socket.socket_create();
}

/** Obviously the whole code is in a loop so it reconnects to
* the server if the connection (\a m_socket) has been dropped.
*/
void ThreadRealmConnector::main() {
    //========================
    //  Main infinite loop
    //========================
    while (true) {
        //--------------------------
        // Connect to realm server
        //--------------------------
        if (m_socket.socket_connect(CONFIG_REALM_IP, CONFIG_REALM_PORT)) {
            ByteArray packetToSend;
            packetToSend.add<CMD>(PCKT_W_SYNC_KEY);
            packetToSend.add<DWORD>(CONFIG_SERVER_ID);
            packetToSend.addString(CONFIG_SERVER_KEY);
            m_socket << packetToSend;
        }

        //-----------------------------
        // Loop for processing inputs
        //-----------------------------
        while (true) {
            ByteArray input;
            m_socket >> input;
            if (!m_socket.isConnected()) break;

            //input.readByte(); //Header
            //input.readDword(); //Size
            input.modSeek(5); //Optimization (1 byte + 4 bytes)

            //------------------------------------------------
            // Another loop to parse all CMDs in the capsule
            //------------------------------------------------
            while (true) {
                //Note: here we are using a switch because there's
                //      not enough possible replies from the server
                //===================================== (Switch start)
                switch (input.read<WORD>()) {
                    case PCKT_R_WELCOME:
                        //Wohoo! The server welcome us :)
                        break;
                    case PCKT_X_DEBUG:
                        //Read string if any but don't print it, its useless for this thread
                        input.readString();
                        break;
                    case PCKT_R_SYNC_KEY_ACK:
                        //Possible ACKs: ACK_SUCCESS or ACK_FAILURE
                        if (input.read<ACK>() == ACK_SUCCESS) {
                            std::cout << "[ThreadRealmConnector] We are now online!" << std::endl;
                        } else {
                            std::cout << "[ThreadRealmConnector] The realm explicitly refused our server!" << std::endl;
                        }
                        break;
                    default:
                        std::cout << "[ThreadRealmConnector] Unknow packet CMD!" << std::endl;
                        input.setSeek(input.size()); //Goto EOF so it breaks to loop below (5 lines down)
                        break;
                }
                //===================================== (Switch end)
                if (input.eof()) break; //break the loop, no more CMDs
            }
        }

        //Just in case; it wont hurt anything anyways
        m_socket.socket_close();

        //Is the error counter to the maximum retries possible
        if (m_errorCounter >= CONFIG_REALMCONNECTOR_MAX_RETRY) {
            //If so, stop asking the realm and take a little nap
            //Note: Unit are in seconds so it must be multiplied by 1000.
            sleep(CONFIG_REALMCONNECTOR_RETRY_SLEEP*1000); //Sleep `x` second of 1000ms
            //Reset the counter so it retry again multiple times before taking another nap
            m_errorCounter=0;
        } else {
            m_errorCounter++;
        }
    }
}
