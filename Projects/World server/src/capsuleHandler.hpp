while (true) { //a loop to parse all CMDs in the capsule
    switch (capsule.read<WORD>()) {
        #include "capsuleHdl_auth.hpp"			//In-game authentification
        #include "capsuleHdl_default.hpp"       //Debugs and default cases
    }
    if (capsule.eof()) break; //break the loop, no more CMDs
}
