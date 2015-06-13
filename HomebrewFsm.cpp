#include <iostream>
#include "HomebrewFsm.h"



void MyFsm::FSM::Run() {
    while(Running())
    {
        std::wcout << state->ToString() << std::endl;
        state->Work(*this);
    }
}

void MyFsm::FSM::TransitTo(MyFsm::StatePtr newState) {
    state = newState;
}

MyFsm::FSM::FSM(MyFsm::StatePtr state)
        : running(true), state(state) {
}

bool MyFsm::FSM::Running() {
    return running;
}

void MyFsm::FSM::Stop() {
    running = false;
}
