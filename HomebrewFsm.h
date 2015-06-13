#pragma once

#include <memory>

namespace MyFsm {

    class FSM;

    class State {
    public:
        virtual void Work(MyFsm::FSM &fsm) = 0;
        virtual std::wstring ToString() const = 0;
    };

    typedef std::shared_ptr<State> StatePtr;

    class FSM {
    public:
        FSM(MyFsm::StatePtr state);
        void Run();
        void TransitTo(StatePtr newState);
        void Stop();
    private:
        StatePtr state;
        bool running;
        bool Running();
    };
}