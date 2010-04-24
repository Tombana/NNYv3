#include "threadHandler.h"

void *threadHandler (void *ptr) {
    //----------- FIXING THREAD DATA ---------------
    std::cout << "[threadHandler] New thread created!" << std::endl;
    //For what is comming next, 'ptr' is void type and must be casted to a s_thread_data pointer
    //Since it would be really A LOT confusing in the next lines, i decided to make a quick #define
    #define VIRTUAL_THREAD_DATA_PTR ((s_thread_data *) ptr)
    //Getting the threadData back to its normal type (void->s_tread_data)
    //And copying the datas to local variables so now we dont mind
    //if the threadData get destroyed, we work with real objects
    //not somewhere in the main() memory ;D
    s_thread_data threadData = *VIRTUAL_THREAD_DATA_PTR; //copying what the pointer refers to into a local structure
    //Send a signal to main() so it can continue processing and destroy the real (old) structure.
    VIRTUAL_THREAD_DATA_PTR->memory_flag_done_copying = true;  //Change the condition flag to true because we are done
    pthread_cond_signal(&VIRTUAL_THREAD_DATA_PTR->memory_protect_signal); //Now wake up main thread by sending a signal
    //*** PAST THIS POINT VIRTUAL_THREAD_DATA_PTR has been DESTROYED! Dont even think to use it! ***

    //Creating new 'client' socket :D
    ZSocket socket;
    socket <= threadData.socketID;
    //Defining some default values
    threadData.is_a_worldserver = false;
    threadData.worldserverID = 0;

    //----------- WELCOME PACKET ---------------
    // IMPORTANT: If the connection is dropped before
    // this part, socket_write wont work and lead to a
    // memory leak from the OS, dont ask me why.
    // --
    // So here is this fix, we check the socket status
    // --
    // It also prevent trying to send data on
    // a closed socket because in case of spam-flood
    // from a evil hacker ;D
    //----------------------------------------
    //Seems like it still doesnt work, whatever
    //valgrind says 'POSSIBLY LOST' wich I know isnt the case.
    //if (socket.isAlive()) { //easy-fix-of-doom (edit: still doesnt works)
        //Creating packet buffer
        ByteArray packetToSend;
        //Welcome packet
        packetToSend.addCmd(PCKT_R_WELCOME);
        //Debug packet
        packetToSend.addCmd(PCKT_X_DEBUG);
        packetToSend.addString("Yay~! Awsum-server-of-doom says hi :)");
        //Writing
        socket << packetToSend;
        packetToSend.clear();
    //}

    ByteArray buffer;
    while (true) { //thread main loop
        //===== Here the famous packet parser!
        //+++ so all the code following must be put in the section that receive/process incoming packets +++
        //+++ this is usually placed whiting a while(true) loop with a socketRead blocking just before +++

        //here we have read all bytes from the socket and put it into 'input'
        ByteArray input;
        socket >> input;
        //'buffer' is a special ByteArray class to read/write a byte array.
        //This object is unique to all threads and is never destroyed.
        //(well its actually destroyed when the socket disconnect but yeah)
        buffer.append(input); //so we append the new input to the end of the buffer

        //!!!!!!!!!!!!!!!!!!
        if (!socket.isDataReceived()) break; //socket disconnected
        //!!!!!!!!!!!!!!!!!!

        //a while() here:
        //yeah sometimes there's many capsules in a packet so we have to
        //do this loop more than once, but otherwise (and usually) it will simply break().
        while (true) {
        //----------------------------------
            //Set the buffer seek to the begening so we can perform our checks without errors.
            //By errors I mean this is making sure we arnt lost somewhere else in the buffer
            //like... you guessed it... doing this loop twice.
            //That could totally result in a massive crash if we dont reset the seek pointer.
            buffer.setSeek(0); //Here, problem solved, whats next :D

            //Now the seek is placed to the begining, so we are supposed to
            //read a "~" (0x7E) byte firstly.
            if (buffer.read<BYTE>() == 0x7E) { //[byte] Packet begining signature
                DWORD length = buffer.read<DWORD>(); //we will need this later on so we know when we are done accumulating data
                BYTE nbCmds = buffer.read<BYTE>(); //might be usefull for debugging even if its useless now

                //Is the length too big? We never know, some hackers could send a fake
                //packet to force the server to put 1Go of trash in memory.
                //Lets fix a limit of 1MB, i doubt we will somewhen have a packet that long
                //but whatever. 1MB is way enough.
                if (length > 1048576) { //1024 Bytes * 1024 KiloBytes = (1 MB) 1048576 Bytes
                    packetToSend.addCmd(PCKT_R_HACKING_ATTEMPT);
                    socket << packetToSend;
                    socket.socket_close();
                    std::cerr << "[threadHandler] @ERROR: Length refused!" << std::endl;
                    break;
                }

                //Okay now the interesting part
                //is the [buffer data] (full size - packet header)
                //greater OR equal the [capsule data] length we have been told earlier?
                if (buffer.size()-6 >= length) {
                    //Awesome we have enough bytes now :D
                    //Lets continue the "so waited" code part
                    //Copy the 6+length first bytes from the buffer to 'capsule'.
                    ByteArray capsule;
                    std::string substring = buffer.getRaw().substr(6, length);
                    capsule.append(substring); //position to 6 and read length bytes
                    //PROCESS THE CAPSULE (switch and stuff)
                    //-----------------------------------------------
                    #include "capsuleHandler.hpp"
                    //-----------------------------------------------
                    //Delete the 6+length first bytes from the buffer
                    buffer.erase(0, 6+length); //position to 0 and delete 6+length character
                    //We already processed it, we no need anymore. Anyway keeping it
                    //would totally screw up the code because the Seek is always set
                    //to the begening remember? So trust me, delete it >:D

                    //Oh well, whats next, the buffer might have move capsules in it
                    //(except the one we just processed and deleted)
                    //so, easy solution, lets do the loop one more time :P
                    //But before looping, lets check if I was right
                    if (buffer.size() == 0) {
                        //We have read all the capsules in this packet
                        //Eheh, oh well we dont need to loop :)
                        break; //break the processing loop to wait for another packet from the socket
                    } else {
                        //Eheh, i was right, there is really is another capsule in this packet :D
                    }
                } else {
                    //The buffer doesnt have enough bytes yet,
                    //we still need to append new inputs to the buffer :(
                    //So lets break for now to read another packet from the socket
                    //and... well... continue accumulating
                    //until we have enough bytes to process that damn packet :D
                    break; //break the processing loop to wait for another packet from the socket
                }

            } else {
                //At this point, i hope we arent expecting a new capsule
                //because if its the case, that capsule doesnt starts with 0x7E.
                //Something is definitivelly going wrong.
                std::cerr << "[threadHandler] @ERROR: Packets are corrupted!" << std::endl;
                std::cerr << "[threadHandler] @ERROR: There is another capsule in this packet, not starting with 0x7E!" << std::endl;
                //TODO (NitriX#): threadHandler, packets corrupted! Drop the guy & log
            }
        }

    }

    std::cout << "[threadHandler] Client logged out!" << std::endl;
    socket.socket_close();

    if (threadData.is_a_worldserver) threadData.dispatcher->setOffline(threadData.worldserverID);

	return NULL;
}
