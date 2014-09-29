/**
 * @file ltl2fsm/automaton/Automaton.hpp
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


#ifndef LTL2FSM__AUTOMATON__AUTOMATON__HPP
#define LTL2FSM__AUTOMATON__AUTOMATON__HPP


#include <ltl2fsm/base/config.hpp>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


/**
 * @class Automaton
 *
 * @brief Abstract template base class. Its interface defines the most
 * common methods amongst all automaton types. The interface is implemented by 
 * an aspect, such as @ref Standard_Automaton_Aspect.
 *
 * @nosubgrouping
 */
template<typename REP>
class Automaton
{
    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Destruction
     * @{
     */
public:
    virtual ~Automaton();
    // @}

    
    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Types
     * @{
     */
public:
    typedef REP Representation_t;
    typedef Automaton Self;
        
    typedef typename Representation_t::Out_Edge_Iterator_t Out_Edge_Iterator_t;
    
    /**
     * @brief A vertex descriptor. It can be used to access the
     * according properties stored in Property_Vertex_Map_t.
     */ 
    typedef typename Representation_t::Vertex_Descriptor_t Vertex_Descriptor_t;

    /**
     * @brief An edge descriptor. It can be used to access the
     * according properties stored in Property_Edge_Map_t.
     */     
    typedef typename Representation_t::Edge_Descriptor_t Edge_Descriptor_t;
    
    /**
     * @brief Holds all vertex labels of Representation_t. 
     */ 
    typedef typename Representation_t::Property_Vertex_Map_t Property_Vertex_Map_t;
  
    /**
     * @brief Holds all edge labels of Representation_t. 
     */ 
    typedef typename Representation_t::Property_Edge_Map_t Property_Edge_Map_t;

    /**
     * @brief A ::std::pair of two edge iterators. 
     */ 
    typedef typename Representation_t::Edge_Iterator_Pair_t Edge_Iterator_Pair_t;

    /**
     * @brief A ::std::pair of two vertex iterators. 
     */
    typedef typename Representation_t::Vertex_Iterator_Pair_t Vertex_Iterator_Pair_t;

    /**
     * @brief A ::std::pair of two out edge iterators.
     */ 
    typedef typename Representation_t::Out_Edge_Iterator_Pair_t Out_Edge_Iterator_Pair_t;
    
    typedef typename Representation_t::Edge_Iterator_t Edge_Iterator_t;
    typedef typename Representation_t::Vertex_Iterator_t Vertex_Iterator_t;

    /**
     * @brief Strong_Components_t is a map, that stores the according
     * component of a vertex.
     */ 
    typedef typename Representation_t::Strong_Components_t Strong_Components_t;
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Representation field access
     * @{
     */
public:
    /**
     * @brief Returns a ::std::pair of Vertex_Iterator_t.
     */ 
    virtual Vertex_Iterator_Pair_t vertex_pair() = 0;

    /**
     * @brief Returns the Property_Vertex_Map_t of the according automaton.
     */ 
    virtual Property_Vertex_Map_t const & property_vertex_map() = 0;

    /**
     * @brief Returns the Property_Edge_Map_t of the according automaton.
     */ 
    virtual Property_Edge_Map_t const & property_edge_map() = 0;
    
    /**
     * @brief Returns a ::std::pair of Edge_Iterator_t.
     */ 
    virtual Edge_Iterator_Pair_t edge_pair() = 0;

    /**
     * @brief Returns all outgoing edges of a vertex @a v.
     */ 
    virtual Out_Edge_Iterator_Pair_t out_edges(Vertex_Descriptor_t const & v) = 0;
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Automaton properties
     * @{
     */
public:
    /**
     * @brief Returns true if vertex @a v is an accepting state, false
     * if it is a non accepting state.
     */ 
    virtual bool accepting(Vertex_Descriptor_t const & v) = 0;
    
    /**
     * @brief Returns the label of the vertex @a v.
     */ 
    virtual ::std::string const & vertex_label(Vertex_Descriptor_t const & v) = 0;
    
    /**
     * @brief Returns the label of the edge @a e.
     */ 
    virtual ::std::string const & edge_label(Edge_Descriptor_t const & e) = 0;

    /**
     * @brief Returns the vertex that is the target of @a e.
     */ 
    virtual Vertex_Descriptor_t target(Edge_Descriptor_t const & e) = 0;
    // @}


#if LTL2FSM_DEBUG__LEVEL__ > 1
public:
    void m_class_invariance() const;
#endif
};


LTL2FSM__END__NAMESPACE__LTL2FSM;


#endif
