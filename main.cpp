#include <boost/msm/front/euml/euml.hpp>
#include <boost/msm/front/euml/state_grammar.hpp>
#include <boost/msm/back/state_machine.hpp>
#include <iostream>

namespace msm = boost::msm;
using namespace boost::msm::front::euml;

BOOST_MSM_EUML_ACTION(state_entry)
{
    template <class Event, class Fsm, class State>
    void operator()(const Event &ev, Fsm &fsm, State &state) const
    {
        std::cout << "Entering" << std::endl;
    }
};

BOOST_MSM_EUML_ACTION(state_exit)
{
    template <class Event, class Fsm, class State>
    void operator()(const Event &ev, Fsm &fsm, State &state) const
    {
        std::cout << "Exiting\n";
    }
};

BOOST_MSM_EUML_STATE((state_entry, state_exit), SocketDisconnected)
BOOST_MSM_EUML_STATE((state_entry, state_exit), SocketConnected)
BOOST_MSM_EUML_EVENT(Connected)
BOOST_MSM_EUML_EVENT(Disconnected)

BOOST_MSM_EUML_DECLARE_ATTRIBUTE(std::function<void(void)>, nextAction);

template <class Fsm, class Event>
void transitTo(Fsm& fsm, const Event &event)
{
    fsm.get_attribute(nextAction) = [&fsm, &event]{fsm.process_event(event);};
}

BOOST_MSM_EUML_ACTION(ConnectSocket)
{
    template <class Evt,class Fsm,class SourceState, class TargetState>
    void operator()(Evt const& ,Fsm& fsm, SourceState&, TargetState& )
    {
        transitTo(fsm, Disconnected);
    }
};

BOOST_MSM_EUML_ACTION(DisconnectSocket)
{
    template <class Evt,class Fsm,class SourceState, class TargetState>
    void operator()(Evt const& ,Fsm& fsm, SourceState&, TargetState& )
    {
        transitTo(fsm, Connected);
    }
};

BOOST_MSM_EUML_TRANSITION_TABLE((
    SocketDisconnected + Connected / ConnectSocket == SocketConnected,
    SocketConnected + Disconnected / DisconnectSocket == SocketDisconnected
), http_transition_table)

BOOST_MSM_EUML_DECLARE_STATE_MACHINE(
(http_transition_table, init_ << SocketDisconnected, no_action, no_action, attributes_ << nextAction, configure_ << no_configure_),
http_state_machine)

int main() {
    auto fsm = msm::back::state_machine<http_state_machine>();
    auto firstAction = [&fsm]{
        fsm.process_event(Connected);
    };
    fsm.get_attribute(nextAction) = firstAction;
    while(true) {
        std::cout << *fsm.current_state() << std::endl;
        auto next = fsm.get_attribute(nextAction);
        next();
    }
};