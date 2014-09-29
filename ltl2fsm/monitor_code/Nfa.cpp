/**
 * @file ltl2fsm/monitor_code/Nfa.cpp
 *
 * $Id$
 *
 * @author Oliver Arafat
 *
 * @brief @ref
 *
 * @note DOCUMENTED
 *
 * @test
 *
 * @todo
 */


#include <ltl2fsm/monitor_code/Nfa.hpp>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


Nfa::Nfa()
{
}

Nfa::~Nfa()
{
}

bool Nfa::accepting(Dstate_t const & dstate) const
{
    Dstate_t::const_iterator it(dstate.begin());
    Dstate_t::const_iterator const end(dstate.end());
    
    for(; it != end; ++it){
	Accepting_Set_t::const_iterator acc(::std::find(m_accepting_set.begin(),
							m_accepting_set.end(),
							*it));
	if(acc != m_accepting_set.end())
	    return true;
    }
    
    return false;
}

Nfa::Dstate_t & Nfa::initial_states()
{
    return m_initial_states;
}

Nfa::Accepting_Set_t & Nfa::accepting_set()
{
    return m_accepting_set;
}


LTL2FSM__END__NAMESPACE__LTL2FSM;
