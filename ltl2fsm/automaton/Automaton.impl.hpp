/**
 * @file ltl2fsm/automaton/Automaton.impl.hpp
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


#ifndef LTL2FSM__AUTOMATON__AUTOMATON_IMPL_HPP
#define LTL2FSM__AUTOMATON__AUTOMATON_IMPL_HPP


#include <ltl2fsm/automaton/Automaton.hpp>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


#if LTL2FSM_DEBUG__LEVEL__ > 1
template<typename REP>
void Automaton<REP>::m_class_invariance() const
{
}
#endif


template<typename REP>
Automaton<REP>::~Automaton()
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT;
}


LTL2FSM__END__NAMESPACE__LTL2FSM;


#endif
