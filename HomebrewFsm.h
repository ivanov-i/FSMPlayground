#pragma once

#include <memory>

namespace MyFsm {

    class State {
    };

    class FSM {
    public:
        void Run();
    private:
        std::shared_ptr <State> state;
    };

}