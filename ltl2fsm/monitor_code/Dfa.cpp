/**
 * @file ltl2fsm/monitor_code/Dfa.cpp
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


#include <ltl2fsm/monitor_code/Dfa.hpp>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


Dfa::Dfa(Nfa * const nfa)
    : m_nfa(nfa),
      m_dstate(m_nfa->initial_states())
{
}

Dfa::~Dfa()
{
}

void Dfa::step(Bit_Vector_t const & bitvector)
{
    m_dstate = m_nfa->step(m_dstate, bitvector);
}

Dfa::Dstate_t const & Dfa::state()
{
    return m_dstate;
}

void Dfa::reset()
{
    m_dstate = m_nfa->initial_states();
}

bool Dfa::accepting(Dstate_t const & dstate) const
{
    return m_nfa->accepting(dstate);
}


LTL2FSM__END__NAMESPACE__LTL2FSM;
