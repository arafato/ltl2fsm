/**
 * @file ltl2fsm/automaton/Nondet_Buechi_Automaton.hpp
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


#ifndef LTL2FSM__AUTOMATON__NONDET_BUECHI_AUTOMATON__HPP
#define LTL2FSM__AUTOMATON__NONDET_BUECHI_AUTOMATON__HPP


#include <ltl2fsm/base/config.hpp>
#include <ltl2fsm/automaton/Standard_Automaton_Aspect.impl.hpp>
#include <ltl2fsm/automaton/Automaton.impl.hpp>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


/**
 * @class Nondet_Buechi_Automaton
 *
 * @brief A Nondet_Buechi_Automaton is a placeholder for a concrete
 * representation such as @ref Graphviz_Representation that represents a NBA.
 *
 * Its interface offers functionality to access all the neccessary
 * datastructures that are needed when dealing with a NBA wrt the
 * whole conversion process of ltl2fsm.
 *
 * The most common methods defined by the interface of @ref Automaton
 * are implemented in @ref Standard_Automaton_Aspect.
 * 
 * @nosubgrouping
 */
template<typename REP>
class Nondet_Buechi_Automaton
    : public Standard_Automaton_Aspect<Automaton<REP> >
{
    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Types
     * @{
     */
private:
    typedef Standard_Automaton_Aspect<Automaton<REP> > Super;
    typedef Nondet_Buechi_Automaton Self;
public:
    typedef typename Super::Representation_t Representation_t;

    typedef typename Representation_t::Strong_Components_t Strong_Components_t;
    typedef typename Representation_t::Vertex_Descriptor_t Vertex_Descriptor_t;
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Creation / Destruction
     * @{
     */
public:
    explicit Nondet_Buechi_Automaton(Representation_t * const representation);
    virtual ~Nondet_Buechi_Automaton();
    Nondet_Buechi_Automaton(Self const & other);
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Automaton construction
     * @{
     */
public:
    /**
     * @brief Sets vertex @a v to an accepting state.
     */ 
    void set_accepting(Vertex_Descriptor_t const & v);
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Automaton properties
     * @{
     */
public:
    /**
     * @brief This method initializes @a component, i.e. it associates
     * to every vertex its strongly connected component id it belongs
     * to.
     */  
    void strong_components(Strong_Components_t & component);
    // @}


#if LTL2FSM_DEBUG__LEVEL__ > 1
public:
    void m_class_invariance() const;
#endif
};


LTL2FSM__END__NAMESPACE__LTL2FSM;


#endif
