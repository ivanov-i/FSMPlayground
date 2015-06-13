//#include "BoostFSM.h"
#include "HomebrewFsm.h"

int main() {
//    BoostFsm::EventLoop();
    auto fsm = MyFsm::FSM();
    fsm.Run();
};

