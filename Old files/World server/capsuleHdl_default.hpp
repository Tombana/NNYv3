//=====================================
// PCKT_X_DEBUG
//=====================================
case PCKT_X_DEBUG:
    std::cout << "[capsuleHandler] -- PCKT_X_DEBUG --" << std::endl;
    std::cout << "[capsuleHandler] Debug text: " << capsule.readString() << std::endl;
    break;
//=====================================
// DEFAULT
//=====================================
default:
    capsule.modSeek(-2);
    std::cerr << "[capsuleHandler] Unknow packet cmdID " << capsule.read<WORD>() << "! STOP!" << std::endl;
    capsule.setSeek(capsule.size()); //Goto EOF so it breaks to loop
    break;
