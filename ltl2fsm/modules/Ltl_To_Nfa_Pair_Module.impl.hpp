/**
 * @file ltl2fsm/modules/Ltl_To_Nfa_Pair_Module.hpp
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


#ifndef LTL2FSM__MODULES__LTL_TO_NFA_PAIR_MODULE_IMPL__HPP
#define LTL2FSM__MODULES__LTL_TO_NFA_PAIR_MODULE_IMPL__HPP


#include <ltl2fsm/modules/Ltl_To_Nfa_Pair_Module.hpp>

#include <ltl2fsm/modules/Ltl_To_Nba_Module.hpp>
#include <ltl2fsm/modules/Nba_To_Nfa_Module.impl.hpp>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM; 


#if LTL2FSM_DEBUG__LEVEL__ > 1
template<typename REP>
void Ltl_To_Nfa_Pair_Module<REP>::m_class_invariance() const
{
}
#endif

template<typename REP>
Ltl_To_Nfa_Pair_Module<REP>::Ltl_To_Nfa_Pair_Module()
    : Module()
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT;
}

template<typename REP>
Ltl_To_Nfa_Pair_Module<REP>::~Ltl_To_Nfa_Pair_Module()
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT;
}

template<typename REP>
typename Ltl_To_Nfa_Pair_Module<REP>::NFA_t * Ltl_To_Nfa_Pair_Module<REP>::p_execute(::std::string const & formula) const
{
    LTL2FSM_METHOD_GUARD("");

    typedef Nondet_Finite_Automaton<Representation_t> NFA_t;
    typedef Nondet_Buechi_Automaton<Representation_t> NBA_t;

    Ltl_To_Nba_Module nba_module;
    Nba_To_Nfa_Module<Representation_t> nfa_module;
    
    NBA_t * const nba(nba_module.execute(formula));
    NFA_t * const nfa(nfa_module.execute(*nba));
    delete nba;
        
    return nfa;
}


template<typename REP>
typename Ltl_To_Nfa_Pair_Module<REP>::Nfa_Pair_t Ltl_To_Nfa_Pair_Module<REP>::execute(::std::string const & pos_formula) const
{
    LTL2FSM_METHOD_GUARD("");
    Nfa_Pair_t const nfa_pair(p_execute(pos_formula),
			      p_execute("!("+pos_formula+")"));
    return nfa_pair;
}


LTL2FSM__END__NAMESPACE__LTL2FSM;


#endif
