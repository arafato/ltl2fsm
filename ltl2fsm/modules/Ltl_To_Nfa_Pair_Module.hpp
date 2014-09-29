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


#ifndef LTL2FSM__MODULES__LTL_TO_NFA_PAIR_MODULE__HPP
#define LTL2FSM__MODULES__LTL_TO_NFA_PAIR_MODULE__HPP


#include <ltl2fsm/base/config.hpp>
#include <ltl2fsm/modules/Module.hpp>

#include <iostream>
#include <string>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM; 


template <typename REP> class Nondet_Finite_Automaton;


/**
 * @class Ltl_To_Nfa_Pair_Module
 *
 * @brief This module offers an interface to directly convert a ltl
 * formula 'p' into the two according NFA's for 'p' and '!p'.
 *
 * It wraps the modules @ref Ltl_To_Nba_Module and @ref
 * Nfa_Construction_Module.
 *
 * @nosubgrouping
 */

template<typename REP>
class Ltl_To_Nfa_Pair_Module
    : public Module
{
    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Construction / Destruction
     * @{
     */
public:
    Ltl_To_Nfa_Pair_Module();
    virtual ~Ltl_To_Nfa_Pair_Module();
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Types
     * @{
     */
public:
    typedef REP Representation_t;
    typedef Nondet_Finite_Automaton<Representation_t> NFA_t;

    /**
     * @brief A Nfa_Pair_t consists of two NFA's. The first one
     * corresponds to the ltl formula 'p', the second one to '!p'.
     */ 
    typedef ::std::pair<NFA_t *, NFA_t *> Nfa_Pair_t;
    
private:
    typedef Ltl_To_Nfa_Pair_Module<REP> Self;
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Conversion
     * @{
     */
public:
    /**
     * @brief Returns a @ref Nfa_Pair_t consisting of two NFA's, the
     * first one corresponding to the ltl formula @a ltl_formula, the
     * second one to @a !ltl_formula.
     */ 
    Nfa_Pair_t execute(::std::string const & ltl_formula) const;
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Internal Helpers
     * @{
     */
private:
    NFA_t * p_execute(::std::string const & ltl_formula) const;
    // @}


#if LTL2FSM_DEBUG__LEVEL__ > 1
public:
    void m_class_invariance() const;
#endif
};


LTL2FSM__END__NAMESPACE__LTL2FSM;


#endif
