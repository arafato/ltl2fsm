/**
 * @file ltl2fsm/automaton/Det_Finite_Automaton.hpp
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


#ifndef LTL2FSM__AUTOMATON__DET_FINITE_AUTOMATON__HPP
#define LTL2FSM__AUTOMATON__DET_FINITE_AUTOMATON__HPP


#include <ltl2fsm/base/config.hpp>
#include <ltl2fsm/automaton/Standard_Automaton_Aspect.impl.hpp>
#include <ltl2fsm/automaton/Automaton.impl.hpp>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


/**
 * @class Det_Finite_Automaton
 *
 * @brief A Det_Finite_Automaton is a placeholder for a concrete
 * representation. such as @ref Graphviz_Representation that
 * represents a DFA.
 *
 * Its interface offers functionality to access all the neccessary
 * datastructures that are needed when dealing with a DFA wrt the
 * whole conversion process of ltl2fsm.
 *
 * The most common methods defined by the interface of @ref Automaton
 * are implemented in @Standard_Automaton_Aspect.
 *
 * @nosubgrouping
 */
template<typename REP>
class Det_Finite_Automaton
    : public Standard_Automaton_Aspect<Automaton<REP> >
{
    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Types
     * @{
     */

private:
    typedef Standard_Automaton_Aspect<Automaton<REP> > Super;
    typedef Det_Finite_Automaton Self;
public:
    typedef typename Super::Representation_t Representation_t;
    
    typedef typename Representation_t::Vertex_Descriptor_t Vertex_Descriptor_t;
    typedef typename Representation_t::Edge_Descriptor_t Edge_Descriptor_t;
    typedef typename Representation_t::Out_Edge_Iterator_Pair_t Out_Edge_Iterator_Pair_t;
    
    /**
     * @brief Needed for unittesting
     */ 
    typedef typename Representation_t::Property_Vertex_Map_t Property_Vertex_Map_t;
    
    /**
     * @brief Needed for unittesting
     */ 
    typedef typename Representation_t::Property_Edge_Map_t Property_Edge_Map_t;
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Creation / Destruction
     * @{
     */
public:
    Det_Finite_Automaton();
    explicit Det_Finite_Automaton(Representation_t * const representation);
    virtual ~Det_Finite_Automaton();
    Det_Finite_Automaton(Self const & other);
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Automaton construction
     * @{
     */
public:
    /**
     * @brief Returns a vertex descriptor for the new vertex.
     *
     * Property_Vertex_Map_t is initialized with all neccessary
     * attributes that are needed during the conversion process and
     * set to a default value. See the documentation of the according
     * representation for more details.
     */ 
    Vertex_Descriptor_t add_vertex();

    /**
     * @brief Returns an edge descriptor for the new edge with the
     * source vertex @a source and the target vertex @a target.
     * 
     * Property_Edge_Map_t is initialized with all neccessary
     * attributes that are needed during the conversion process and
     * set to a default value. See the documentation of the according
     * representation for more details.
     *
     * Some graphtypes, i.e. representations, will not insert an edge
     * if an edge with the same source and target is already in the
     * graph. In that case an exception will be thrown.
     *
     * @throws Edge_Already_Added
     */ 
    Edge_Descriptor_t add_edge(Vertex_Descriptor_t const & source,
			       Vertex_Descriptor_t const & target);

    /**
     * @brief Sets the attribute 'label' of vertex @a v to @a name.
     */ 
    void set_label(Vertex_Descriptor_t const & v,
		   ::std::string const & name);
    
    /**
     * @brief Sets the attribute 'label' of edge @a e to @a name.
     */ 
    void set_label(Edge_Descriptor_t const & e,
		   ::std::string const & name);
    
    /**
     * @brief Sets vertex @a v to an accepting state.
     */ 
    void set_accepting(Vertex_Descriptor_t const & v);

    /**
     * @brief Returns the vertex descriptor of the vertex with name @a name.
     *
     * @note It is assumed, that every name of a vertex is unique,
     * i.e. there do not exist two vertices within the same graph with
     * the same name.
     *
     * @throws Invalid_Vertex_Name
     */ 
    Vertex_Descriptor_t const & vertex_descriptor(::std::string const & name);
    // @}

    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Three-valued logic
     * @{
     */
public:
    /**
     * @brief Marks the state @v as top (peripheries=3).
     */ 
    void set_top(Vertex_Descriptor_t const & v);
    
    /**
     * @brief Marks the state @v as bottom (peripheries=4).
     */ 
    void set_bottom(Vertex_Descriptor_t const & v);

    /**
     * @brief Marks the state @v as inconclusive (peripheries=5).
     */ 
    void set_inconclusive(Vertex_Descriptor_t const & v);
        
    /**
     * @brief Returns true if @a v is not element of the set of
     * accepting states of the DFA that corresponds to some negated
     * ltl formula p.
     */     
    bool top(Vertex_Descriptor_t const & v);
    
    /**
     * @brief Returns true if @a v is not element of the set of
     * accepting states of the DFA that corresponds the some ltl
     * formula p.
     */ 
    bool bottom(Vertex_Descriptor_t const & v);

    /**
     * @brief Returns true if @a v is both element of the set of
     * accepting states of a DFA that corresponds to some ltl formula
     * 'p' and of a DFA that corresponds to '!p'.
     */ 
    bool inconclusive(Vertex_Descriptor_t const & v);
    // @}


#if LTL2FSM_DEBUG__LEVEL__ > 1
public:
    void m_class_invariance() const;
#endif
};


LTL2FSM__END__NAMESPACE__LTL2FSM;


#endif
