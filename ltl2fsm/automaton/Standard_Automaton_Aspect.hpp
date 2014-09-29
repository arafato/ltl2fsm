/**
 * @file ltl2fsm/automaton/Standard_Automaton_Aspect.hpp
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


#ifndef LTL2FSM__AUTOMATON__STANDARD_AUTOMATON_ASPECT__HPP
#define LTL2FSM__AUTOMATON__STANDARD_AUTOMATON_ASPECT__HPP


#include <ltl2fsm/base/config.hpp>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


/**
 * @class Standard_Automaton_Aspect
 *
 * @brief This aspect provides the implementation for methods that are
 * common amongst all automaton classes.
 *
 * @nosubgrouping
 */
template<typename BASE>
class Standard_Automaton_Aspect
    : public BASE
{
    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @Name Types
     * @{
     */
private:
    typedef Standard_Automaton_Aspect Self;

public:
    typedef typename BASE::Representation_t Representation_t;

    typedef typename Representation_t::Out_Edge_Iterator_t Out_Edge_Iterator_t;
    typedef typename Representation_t::Edge_Iterator_t Edge_Iterator_t;
    typedef typename Representation_t::Vertex_Iterator_t Vertex_Iterator_t;

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
     * @brief Holds all vertex properties of Representation_t. 
     */ 
    typedef typename Representation_t::Property_Vertex_Map_t Property_Vertex_Map_t;
    
    /**
     * @brief Holds all edge properties of Representation_t. 
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
    // @}
    
    
    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Construction / Destruction
     * @{
     */
protected:
    Standard_Automaton_Aspect();
    explicit Standard_Automaton_Aspect(Representation_t * const representation);
    Standard_Automaton_Aspect(Self const & other);
    Standard_Automaton_Aspect & operator=(Self const & other);
public:
    virtual ~Standard_Automaton_Aspect();
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Aspect field access
     * @{
     */
public:
    /**
     * @brief Returns a pointer to the representation.
     *
     * @note This method is inlined.
     */ 
    Representation_t * p_representation();
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Representation field access
     * @{
     */
public:
    virtual Vertex_Iterator_Pair_t  vertex_pair();
    virtual Property_Vertex_Map_t const & property_vertex_map();
    virtual Property_Edge_Map_t const & property_edge_map();
    virtual Edge_Iterator_Pair_t edge_pair();
    virtual Out_Edge_Iterator_Pair_t out_edges(Vertex_Descriptor_t const & e);
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
    virtual bool accepting(Vertex_Descriptor_t const & v);    
	
    /**
     * @brief Returns the label of the vertex @a v.
     */ 
    virtual ::std::string const & vertex_label(Vertex_Descriptor_t const & v);

    /**
     * @brief Returns the label of the edge @a e.
     */ 
    virtual ::std::string const & edge_label(Edge_Descriptor_t const & e);

    /**
     * @brief Returns the vertex that is the target of @a e.
     */ 
    virtual Vertex_Descriptor_t target(Edge_Descriptor_t const & e);
    // @}
        

    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name State
     * @{
     */
private:
    /**
     * @brief The actual representation of the class that wraps all
     * relevant datastructures of an automaton wrp to the whole
     * conversion process of ltl2fsm. It can be of any kind that is
     * compatible to the Boost BGL.
     */ 
    Representation_t * m_representation;
    // @}


#if LTL2FSM_DEBUG__LEVEL__ > 1
public:
    void m_class_invariance() const;
#endif
};


template<typename BASE>
inline typename Standard_Automaton_Aspect<BASE>::Representation_t * Standard_Automaton_Aspect<BASE>::p_representation()
{
    return m_representation;
}


LTL2FSM__END__NAMESPACE__LTL2FSM;


#endif
