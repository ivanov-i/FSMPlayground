#include <boost/msm/front/euml/euml.hpp>
#include <boost/msm/front/euml/state_grammar.hpp>
#include <boost/msm/back/state_machine.hpp>
#include <iostream>

namespace msm = boost::msm;
using namespace boost::msm::front::euml;

template <class Fsm, class State>
void HandleStateEnter(Fsm &fsm, State &state);

BOOST_MSM_EUML_ACTION(state_entry)
{
    template <class Event, class Fsm, class State>
    void operator()(const Event &ev, Fsm &fsm, State &state) const
    {
        std::cout << "entering" << std::endl;
        HandleStateEnter(fsm, state);
    }
};

BOOST_MSM_EUML_STATE((state_entry), SocketDisconnected)
BOOST_MSM_EUML_STATE((state_entry), SocketConnected)
BOOST_MSM_EUML_EVENT(Connected)
BOOST_MSM_EUML_EVENT(Disconnected)

BOOST_MSM_EUML_DECLARE_ATTRIBUTE(std::function<void(void)>, nextAction);
BOOST_MSM_EUML_DECLARE_ATTRIBUTE(bool, stopped);

template <class Fsm, class Event>
void sendMessage(Fsm& fsm, const Event &event)
{
    fsm.get_attribute(nextAction) = [&fsm, &event]{fsm.process_event(event);};
}

template <class Fsm> void Stop(Fsm& fsm)
{
    fsm.get_attribute(stopped) = true;
}

BOOST_MSM_EUML_TRANSITION_TABLE((
    SocketDisconnected + Connected == SocketConnected,
    SocketConnected + Disconnected == SocketDisconnected
), http_transition_table)

BOOST_MSM_EUML_DECLARE_STATE_MACHINE(
(http_transition_table, init_ << SocketDisconnected, no_action, no_action, attributes_ << nextAction << stopped, configure_ << no_configure_),
http_state_machine)

void EventLoop() {
    auto fsm = msm::back::state_machine<http_state_machine>();
    auto firstAction = [&fsm]{
        fsm.process_event(Connected);
    };
    fsm.get_attribute(nextAction) = firstAction;
    while(!fsm.get_attribute(stopped)) {
        std::__1::cout << *fsm.current_state() << std::__1::endl;
        auto next = fsm.get_attribute(nextAction);
        next();
    }
}

int main() {
    EventLoop();
};

template <class Fsm>
static void HandleStateEnter(Fsm &fsm, decltype(SocketConnected) &state)
{
    sendMessage(fsm, Disconnected);
}

template <class Fsm>
static void HandleStateEnter(Fsm &fsm, decltype(SocketDisconnected) &state)
{
    sendMessage(fsm, Connected);
    Stop(fsm);
}
