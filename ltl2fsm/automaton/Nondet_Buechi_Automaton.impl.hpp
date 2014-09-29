/**
 * @file ltl2fsm/automaton/Nondet_Buechi_Automaton.impl.hpp
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


#ifndef LTL2FSM__AUTOMATON__NONDET_BUECHI_AUTOMATON_IMPL_HPP
#define LTL2FSM__AUTOMATON__NONDET_BUECHI_AUTOMATON_IMPL_HPP


#include <ltl2fsm/automaton/Nondet_Buechi_Automaton.hpp>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


#if LTL2FSM_DEBUG__LEVEL__ > 1
template<typename REP>
void Nondet_Buechi_Automaton<REP>::m_class_invariance() const
{
}
#endif


template<typename REP>
Nondet_Buechi_Automaton<REP>::Nondet_Buechi_Automaton(Representation_t * const representation)
    : Super(representation)
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT;
}

template<typename REP>
Nondet_Buechi_Automaton<REP>::Nondet_Buechi_Automaton(Self const & other)
    : Super(other)
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT;
}

template<typename REP>
Nondet_Buechi_Automaton<REP>::~Nondet_Buechi_Automaton()
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT;
}

template<typename REP>
void Nondet_Buechi_Automaton<REP>::strong_components(Strong_Components_t & component)
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    Super::p_representation()->strong_components(component);
}

template<typename REP>
void Nondet_Buechi_Automaton<REP>::set_accepting(Vertex_Descriptor_t const & v)
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    Super::p_representation()->set_accepting(v);
}


LTL2FSM__END__NAMESPACE__LTL2FSM;


#endif
