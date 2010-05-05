//=====================================
// PCKT_C_AUTH
//=====================================
case PCKT_C_AUTH:
    std::cout << "[capsuleHandler] -- PCKT_C_AUTH --" << std::endl;
    std::cout << "[capsuleHandler] Username: " << capsule.readString() << std::endl;
    std::cout << "[capsuleHandler] Password: " << capsule.readString() << std::endl;
    break;
