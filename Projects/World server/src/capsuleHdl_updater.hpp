//=====================================
// PCKT_C_REVISION
//=====================================
case PCKT_C_REVISION:
    { //brakets here relates to "jumping over the initialization" (ISO C++ '03 6.7/3)
    DWORD clientRevision = capsule.readDword();
    std::cout << "[capsuleHandler] -- PCKT_C_REVISION --" << std::endl;
    std::cout << "[capsuleHandler] Client revision: " << clientRevision << std::endl;
    //---------------------------------

    //---------------------------------
    }
    break;
