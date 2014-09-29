/**
 * @file ltl2fsm/automaton/Nondet_Finite_Automaton.hpp
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


#ifndef LTL2FSM__AUTOMATON__NONDET_FINITE_AUTOMATON__HPP
#define LTL2FSM__AUTOMATON__NONDET_FINITE_AUTOMATON__HPP


#include <ltl2fsm/base/config.hpp>
#include <ltl2fsm/automaton/Standard_Automaton_Aspect.impl.hpp>
#include <ltl2fsm/automaton/Automaton.hpp>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


/**
 * @class Nondet_Finite_Automaton
 *
 * @brief A Nondet_Finite_Automaton is a placeholder for a concrete
 * representation such as @ref Graphviz_Representation that represents a NFA.
 *
 * Its interface offers functionality to access all the neccessary
 * datastructures that are needed when dealing with a NFA wrt the
 * whole conversion process of ltl2fsm.
 *
 * The most common methods defined by the interface of @ref Automaton
 * are implemented in @Standard_Automaton_Aspect.
 *
 * @nosubgrouping
 */
template<typename REP>
class Nondet_Finite_Automaton
    : public Standard_Automaton_Aspect<Automaton<REP> >
{
    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Types
     * @{
     */

private:
    typedef Standard_Automaton_Aspect<Automaton<REP> > Super;
    typedef Nondet_Finite_Automaton Self;
public:
    typedef typename Super::Representation_t Representation_t;

    typedef typename Representation_t::Vertex_Descriptor_t Vertex_Descriptor_t;
    typedef typename Representation_t::Edge_Descriptor_t Edge_Descriptor_t;
    typedef typename Representation_t::Out_Edge_Iterator_Pair_t Out_Edge_Iterator_Pair_t;
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Creation / Destruction
     * @{
     */
public:
    explicit Nondet_Finite_Automaton(Representation_t * const representation);
    virtual ~Nondet_Finite_Automaton();
    Nondet_Finite_Automaton(Self const & other);
    // @}


#if LTL2FSM_DEBUG__LEVEL__ > 1
public:
    void m_class_invariance() const;
#endif
};


LTL2FSM__END__NAMESPACE__LTL2FSM;


#endif
