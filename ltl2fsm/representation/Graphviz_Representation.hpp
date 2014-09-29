/**
 * @file ltl2fsm/representation/Graphviz_Representation.hpp
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


#ifndef LTL2FSM__REPRESENTATION__GRAPHVIZ_REPRESENTATION__HPP
#define LTL2FSM__REPRESENTATION__GRAPHVIZ_REPRESENTATION__HPP


#include <ltl2fsm/base/config.hpp>

#include <iostream>

// Boost graph library
#include <boost/graph/graphviz.hpp>
#include <boost/graph/strong_components.hpp>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


/**
 * @class Graphviz_Representation
 *
 * @brief A Graphviz_Representation holds all relevant datastructures
 * of @ref ::boost::GraphvizDigraph. It is used to model automata that
 * are based on a graphviz description.
 *
 * A Graphviz_Representation can be used as a concrete representation
 * of @ref Automaton.
 *
 * @nosubgrouping
 */
class Graphviz_Representation
{
    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Creation / Destruction
     * @{
     */
private:
    typedef Graphviz_Representation Self;

public:
    explicit Graphviz_Representation(::boost::GraphvizDigraph const & graph);
    Graphviz_Representation(Self const & other);
    Graphviz_Representation & operator=(Self const & other);
    ~Graphviz_Representation();
    // @}
  

    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Types
     * @{
     */
public:
    typedef ::boost::graph_traits< ::boost::GraphvizDigraph>::out_edge_iterator Out_Edge_Iterator_t;
    typedef ::boost::graph_traits< ::boost::GraphvizDigraph>::edge_iterator Edge_Iterator_t;
    typedef ::boost::graph_traits< ::boost::GraphvizDigraph>::vertex_iterator Vertex_Iterator_t;
    typedef ::boost::graph_traits< ::boost::GraphvizDigraph>::vertex_descriptor Vertex_Descriptor_t;
    typedef ::boost::graph_traits< ::boost::GraphvizDigraph>::edge_descriptor Edge_Descriptor_t;
    typedef ::boost::property_map< ::boost::GraphvizDigraph, ::boost::vertex_attribute_t>::type Property_Vertex_Map_t;
    typedef ::boost::property_map< ::boost::GraphvizDigraph, ::boost::edge_attribute_t>::type Property_Edge_Map_t;
    typedef ::std::pair<Out_Edge_Iterator_t, Out_Edge_Iterator_t> Out_Edge_Iterator_Pair_t;
    typedef ::std::pair<Vertex_Iterator_t, Vertex_Iterator_t> Vertex_Iterator_Pair_t;
    typedef ::std::pair<Edge_Iterator_t, Edge_Iterator_t> Edge_Iterator_Pair_t;
    typedef ::std::map<Vertex_Descriptor_t, int> Strong_Components_t;
    typedef ::boost::GraphvizDigraph Graph_t;
    // @}

  
    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Field access
     * @{
     */
public:
    Vertex_Iterator_Pair_t vertex_pair();
    Edge_Iterator_Pair_t edge_pair();
    Property_Vertex_Map_t const & property_vertex_map();
    Property_Edge_Map_t const & property_edge_map();
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
     * The Property_Vertex_Map_t is initialized with all neccessary
     * attributes that are needed during the conversion process.
     * By default, "label" is set to "unnamed", "peripheries" is set to "1".
     *
     * Additionally, specific labels, such as "shape" and "fontname"
     * are added.  They are not directly needed for the conversion
     * process but for a correct dot-format output.
     */ 
    Vertex_Descriptor_t add_vertex();

    /**
     * @brief Returns an edge descriptor for the new edge. 
     *
     * The Property_Edge_Map_t is initialized with all neccessary
     * attributes that are needed during the conversion process. By
     * default, "label" is set to "unnamed".
     *
     * Additionally, specific labels, such as "fontname" are
     * added. They are not directly needed for the conversion process
     * but for a correct dot-format output.
     *
     * The graphviz representation allows it to add multiple edges with
     * the same source and target vertex.
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
     * @brief Returns a ::std::pair of Out_Edge_Iterator_t, i.e. all
     * outgoing edges from vertex @a v.
     */ 
    Out_Edge_Iterator_Pair_t out_edges(Vertex_Descriptor_t const & v);

    /**
     * @brief Returns the vertex that is the target of @a e.
     */ 
    Vertex_Descriptor_t const target(Edge_Descriptor_t const & e);

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
    bool accepting(Vertex_Descriptor_t const & v);

    /**
     * @brief Returns true if @a v is not element of the set of
     * accepting states of the DFA that corresponds to some negated
     * ltl formula p.
     */     
    bool top(Vertex_Descriptor_t const & v);

    /**
     * @brief Returns true if @a v is not element of the set of
     * accepting states of the DFA that corresponds to some ltl
     * formula p.
     */ 
    bool bottom(Vertex_Descriptor_t const & v);

    /**
     * @brief Returns true if @a v is both element of the set of
     * accepting states of a DFA that corresponds to some ltl formula
     * 'p' and of a DFA that corresponds to '!p'.
     */ 
    bool inconclusive(Vertex_Descriptor_t const & v);
                
    /**
     * @brief Returns the label of the vertex @a v.
     */ 
    ::std::string const & vertex_label(Vertex_Descriptor_t const & v);

    /**
     * @brief Returns the label of the edge @a e.
     */ 
    ::std::string const & edge_label(Edge_Descriptor_t const & e);

    /**
     * @brief This method initializes @ component, i.e. it associates
     * to every vertex its strongly connected component id it belongs
     * to.
     */  
    void strong_components(Strong_Components_t & component);

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
     * @name State
     * @{
     */
private:
    /**
     * @brief The @ref ::boost::GraphvizDigraph object that stores the
     * automaton structure.
     */ 
    Graph_t m_graph;
  
    /**
     * @brief A Prop_Vertex_Map_t stores all vertex labels that
     * are defined in the graphviz description.
     */ 
    Property_Vertex_Map_t m_vertex_map;

    /**
     * @brief A Prop_Edge_Map_t stores all edge labels that
     * are defined in the graphviz description.
     */ 
    Property_Edge_Map_t m_edge_map;

    /**
     * @brief A ::std::pair of Vertex_Iterator_Pair_t. The first
     * iterator points to the first vertex in the sequence, the second
     * to the last one.
     */ 
    Vertex_Iterator_Pair_t m_vertex_iterator_pair;

    /**
     * @brief A ::std::pair of Edge_Iterator_Pair_t. The first
     * iterator points to the first edge in the sequence, the second
     * to the last one.
     */ 
    Edge_Iterator_Pair_t m_edge_iterator_pair;
    // @}


#if LTL2FSM_DEBUG__LEVEL__ > 1
public:
    void m_class_invariance() const;
#endif
};


LTL2FSM__END__NAMESPACE__LTL2FSM;


#endif
