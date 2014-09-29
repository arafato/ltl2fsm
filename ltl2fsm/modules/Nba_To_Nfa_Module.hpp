/**
 * @file ltl2fsm/modules/Nba_To_Nfa_Module.hpp
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
 * @todo Documentation
 */


#ifndef LTL2FSM__MODULES__NBA_TO_NFA_MODULE__HPP
#define LTL2FSM__MODULES__NBA_TO_NFA_MODULE__HPP


#include <ltl2fsm/base/config.hpp>
#include <ltl2fsm/modules/Module.hpp>

#include <set>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


template <typename REP> class Nondet_Buechi_Automaton;
template <typename REP> class Nondet_Finite_Automaton;


/**
 * @class Nba_To_Nfa_Module
 *
 * @brief This module offers an interface to convert a @ref
 * Nondet_Buechi_Automaton to a @ref Nondet_Finite_Automaton.
 *
 * @nosubgrouping
 */
template<typename REP>
class Nba_To_Nfa_Module
    : public Module
{
    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Types
     * @{
     */
public:
    typedef REP Representation_t;
    typedef Nondet_Finite_Automaton<Representation_t> NFA_t;
    typedef Nondet_Buechi_Automaton<Representation_t> NBA_t;
    
    typedef typename Representation_t::Strong_Components_t Strong_Components_t;
        
private:
    typedef Nba_To_Nfa_Module<REP> Self;
    typedef typename Representation_t::Vertex_Descriptor_t Vertex_Descriptor_t;
    typedef typename Representation_t::Vertex_Iterator_Pair_t Vertex_Iterator_Pair_t;

    /**
     * @brief Stores all component ids that have an accepting vertex.
     */ 
    typedef ::std::set<int> Accepting_Components_t;
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Construction / Destruction
     * @{
     */
public:
    Nba_To_Nfa_Module();
    virtual ~Nba_To_Nfa_Module();
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Conversion
     * @{
     */
public:
    /**
     * @brief Converts a @a NBA_t to a @a NFA_t.
     */ 
    NFA_t * const execute(NBA_t & nba);


#if LTL2FSM_DEBUG__LEVEL__ > 1
public:
	void m_class_invariance() const;
#endif
};


LTL2FSM__END__NAMESPACE__LTL2FSM;


#endif
