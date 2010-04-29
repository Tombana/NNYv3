while (true) { //a loop to parse all CMDs in the capsule
    switch (capsule.read<WORD>()) {
        #include "capsuleHdl_default.hpp"       //Include debugs and default cases
    }
    if (capsule.eof()) break; //break the loop, no more CMDs
}
