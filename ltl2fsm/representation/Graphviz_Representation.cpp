/**
 * @file ltl2fsm/representation/Graphviz_Representation.cpp
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


#include <ltl2fsm/representation/Graphviz_Representation.hpp>
#include <ltl2fsm/base/exception/Invalid_Vertex_Name.hpp>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


#if LTL2FSM_DEBUG__LEVEL__ > 1
void Graphviz_Representation::m_class_invariance() const
{
}
#endif

Graphviz_Representation::Graphviz_Representation(::boost::GraphvizDigraph const & graph)
    : m_graph(graph),
      m_vertex_map(::boost::get(::boost::vertex_attribute, m_graph)),
      m_edge_map(::boost::get(::boost::edge_attribute, m_graph)),
      m_vertex_iterator_pair(::boost::vertices(m_graph)),
      m_edge_iterator_pair(::boost::edges(m_graph))
    
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT;
}

Graphviz_Representation::Graphviz_Representation(Self const & other)
    : m_graph(other.m_graph),
      m_vertex_map(::boost::get(::boost::vertex_attribute, m_graph)),
      m_edge_map(::boost::get(::boost::edge_attribute, m_graph)),
      m_vertex_iterator_pair(::boost::vertices(m_graph)),
      m_edge_iterator_pair(::boost::edges(m_graph))
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT;
}

Graphviz_Representation & Graphviz_Representation::operator=(Self const & other)
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    
    if(this == &other)
      return *this;

    m_graph = other.m_graph;
    m_vertex_map = ::boost::get(::boost::vertex_attribute, m_graph);
    m_edge_map = ::boost::get(::boost::edge_attribute, m_graph);
    m_vertex_iterator_pair = ::boost::vertices(m_graph);
    m_edge_iterator_pair = ::boost::edges(m_graph);

    LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT;

    return *this;
}

Graphviz_Representation::~Graphviz_Representation()
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT;
}

Graphviz_Representation::Vertex_Iterator_Pair_t Graphviz_Representation::vertex_pair()
{
    LTL2FSM_METHOD_GUARD("");
    return m_vertex_iterator_pair;
}

Graphviz_Representation::Edge_Iterator_Pair_t Graphviz_Representation::edge_pair()
{
    LTL2FSM_METHOD_GUARD("");
    return m_edge_iterator_pair;
}

Graphviz_Representation::Property_Vertex_Map_t const & Graphviz_Representation::property_vertex_map()
{
    LTL2FSM_METHOD_GUARD("");
    return m_vertex_map;
}

Graphviz_Representation::Property_Edge_Map_t const & Graphviz_Representation::property_edge_map()
{
    LTL2FSM_METHOD_GUARD("");
    return m_edge_map;
}

Graphviz_Representation::Vertex_Descriptor_t Graphviz_Representation::add_vertex()
{
    LTL2FSM_METHOD_GUARD("");

    Vertex_Descriptor_t vd = ::boost::add_vertex(m_graph);
    // Updating...
    m_vertex_map = ::boost::get(::boost::vertex_attribute, m_graph);
    m_vertex_iterator_pair = ::boost::vertices(m_graph);

    m_vertex_map[vd]["label"] = "unnamed";
    m_vertex_map[vd]["peripheries"] = "1";
    // Inserting specific labels so that the graph description is
    // suited for dot output.
    m_vertex_map[vd]["shape"] = "ellipse";
    m_vertex_map[vd]["fontname"] = "\"TimesNewRoman\"";
        
    return vd;
}

Graphviz_Representation::Edge_Descriptor_t Graphviz_Representation::add_edge(Vertex_Descriptor_t const & source,
									     Vertex_Descriptor_t const & target)
{
    LTL2FSM_METHOD_GUARD("");

    typedef ::std::pair<Edge_Descriptor_t, bool> Pair_t;
    Pair_t ret = ::boost::add_edge(source, target, m_graph);
    // Updating...
    m_edge_map = ::boost::get(::boost::edge_attribute, m_graph);
    m_edge_iterator_pair = ::boost::edges(m_graph);

    m_edge_map[ret.first]["label"] = "unnamed";
    // Specific label for dot output.
    m_edge_map[ret.first]["fontname"] = "\"TimesNewRoman\"";
        
    // ret.second is always true here
    return ret.first;
}

void Graphviz_Representation::set_label(Vertex_Descriptor_t const & v,
					::std::string const & name)
{
   LTL2FSM_METHOD_GUARD("");
   
   m_vertex_map[v]["label"] = name;
}

void Graphviz_Representation::set_label(Edge_Descriptor_t const & e,
					::std::string const & name)
{
    LTL2FSM_METHOD_GUARD("");

    m_edge_map[e]["label"] = name;
}

void Graphviz_Representation::set_accepting(Vertex_Descriptor_t const & v)
{
    LTL2FSM_METHOD_GUARD("");

    (*(m_vertex_map[v].find("peripheries"))).second = "2";
}

void Graphviz_Representation::set_top(Vertex_Descriptor_t const & v)
{
    LTL2FSM_METHOD_GUARD("");

    (*(m_vertex_map[v].find("peripheries"))).second = "3";
}

void Graphviz_Representation::set_bottom(Vertex_Descriptor_t const & v)
{
    LTL2FSM_METHOD_GUARD("");

    (*(m_vertex_map[v].find("peripheries"))).second = "4";
}

void Graphviz_Representation::set_inconclusive(Vertex_Descriptor_t const & v)
{
    LTL2FSM_METHOD_GUARD("");

    (*(m_vertex_map[v].find("peripheries"))).second = "5";
}

Graphviz_Representation::Out_Edge_Iterator_Pair_t Graphviz_Representation::out_edges(Vertex_Descriptor_t const & v)
{
    LTL2FSM_METHOD_GUARD("");
    return ::boost::out_edges(v, m_graph);
}

Graphviz_Representation::Vertex_Descriptor_t const Graphviz_Representation::target(Edge_Descriptor_t const & e)
{
    LTL2FSM_METHOD_GUARD("");
    return ::boost::target(e, m_graph);
}

void Graphviz_Representation::strong_components(Strong_Components_t & component)
{
    LTL2FSM_METHOD_GUARD("");

    ::boost::strong_components(m_graph, ::boost::make_assoc_property_map(component));
}

::std::string const & Graphviz_Representation::vertex_label(Vertex_Descriptor_t const & v)
{
    LTL2FSM_METHOD_GUARD("");

    return (*(m_vertex_map[v].find("label"))).second;
}

Graphviz_Representation::Vertex_Descriptor_t const & Graphviz_Representation::vertex_descriptor(::std::string const & name)
{
    LTL2FSM_METHOD_GUARD("");

    for(Vertex_Iterator_Pair_t vp = m_vertex_iterator_pair; vp.first != vp.second; ++vp.first){
	if(vertex_label(*vp.first) == name)
	    return *vp.first;
    }
    // No match...
    throw Invalid_Vertex_Name(LTL2FSM__SOURCE_LOCATION,
			      ::std::string("Vertex name does not exist in graph: '" + name + "'"));
}

bool Graphviz_Representation::accepting(Vertex_Descriptor_t const & v)
{
    LTL2FSM_METHOD_GUARD("");

    return (*(m_vertex_map[v].find("peripheries"))).second == "2" ? true : false;
}

bool Graphviz_Representation::top(Vertex_Descriptor_t const & v)
{
    LTL2FSM_METHOD_GUARD("");

    return (*(m_vertex_map[v].find("peripheries"))).second == "3" ? true : false;
}

bool Graphviz_Representation::bottom(Vertex_Descriptor_t const & v)
{
    LTL2FSM_METHOD_GUARD("");

    return (*(m_vertex_map[v].find("peripheries"))).second == "4" ? true : false;
}

bool Graphviz_Representation::inconclusive(Vertex_Descriptor_t const & v)
{
    LTL2FSM_METHOD_GUARD("");

    return (*(m_vertex_map[v].find("peripheries"))).second == "5" ? true : false;
}

::std::string const & Graphviz_Representation::edge_label(Edge_Descriptor_t const & e)
{
    LTL2FSM_METHOD_GUARD("");

    return (*(m_edge_map[e].find("label"))).second;
}


LTL2FSM__END__NAMESPACE__LTL2FSM;
