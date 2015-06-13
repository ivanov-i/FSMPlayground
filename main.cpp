//#include "BoostFSM.h"
#include "HomebrewFsm.h"
#include <string>

class Disconnected :public MyFsm::State
{
public:
    virtual std::wstring ToString() const override
    {
        return std::wstring(L"Disconnected");
    };
    virtual void Work(MyFsm::FSM &fsm) override;
};

class SocketConnected :public MyFsm::State
{
public:
    virtual std::wstring ToString() const override
    {
        return std::wstring(L"SocketConnected");
    }

    virtual void Work(MyFsm::FSM &fsm) override;
};

int main() {
//    BoostFsm::EventLoop();
    auto fsm = MyFsm::FSM(std::make_shared<Disconnected>());
    fsm.Run();
};

void Disconnected::Work(MyFsm::FSM &fsm) {
    fsm.TransitTo(std::make_shared<SocketConnected>());
}

void SocketConnected::Work(MyFsm::FSM &fsm) {
    fsm.Stop();
}