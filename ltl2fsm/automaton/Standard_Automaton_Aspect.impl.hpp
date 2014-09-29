/**
 * @file ltl2fsm/automaton/Standard_Automaton_Aspect.impl.hpp
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


#ifndef LTL2FSM__AUTOMATON__STANDARD_AUTOMATON_ASPECT_IMPL__HPP
#define LTL2FSM__AUTOMATON__STANDARD_AUTOMATON_ASPECT_IMPL__HPP


#include <ltl2fsm/automaton/Standard_Automaton_Aspect.hpp>
#include <algorithm>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


#if LTL2FSM_DEBUG__LEVEL__ > 1
template<typename BASE>
void Standard_Automaton_Aspect<BASE>::m_class_invariance() const
{
}
#endif


template<typename BASE>
Standard_Automaton_Aspect<BASE>::Standard_Automaton_Aspect()
    : m_representation(new Representation_t())
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT;
}

template<typename BASE>
Standard_Automaton_Aspect<BASE>::Standard_Automaton_Aspect(Representation_t * const representation)
  : m_representation(representation)
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT;
}

template<typename BASE>
Standard_Automaton_Aspect<BASE>::Standard_Automaton_Aspect(Self const & other)
  : m_representation(new Representation_t(*other.m_representation))
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT;
}

template<typename BASE>
Standard_Automaton_Aspect<BASE>::operator=(Self const & other)
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT;

    using ::std::swap;
    
    if (this == &other)
      return *this;

    Standard_Automaton_Aspect<BASE> copy(other);
    swap(this->m_representation, copy.m_representation);

    return * this;
}

template<typename BASE>
Standard_Automaton_Aspect<BASE>::~Standard_Automaton_Aspect()
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT;
    delete m_representation;
}

template<typename BASE>
typename Standard_Automaton_Aspect<BASE>::Vertex_Iterator_Pair_t Standard_Automaton_Aspect<BASE>::vertex_pair()
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    return m_representation->vertex_pair();
}

template<typename BASE>
typename Standard_Automaton_Aspect<BASE>::Property_Vertex_Map_t const & Standard_Automaton_Aspect<BASE>::property_vertex_map()
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    return m_representation->property_vertex_map();
}

template<typename BASE>
typename Standard_Automaton_Aspect<BASE>::Property_Edge_Map_t const & Standard_Automaton_Aspect<BASE>::property_edge_map()
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    return m_representation->property_edge_map();
}

template<typename BASE>
typename Standard_Automaton_Aspect<BASE>::Edge_Iterator_Pair_t Standard_Automaton_Aspect<BASE>::edge_pair()
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    return m_representation->edge_pair();
}

template<typename BASE>
::std::string const & Standard_Automaton_Aspect<BASE>::vertex_label(Vertex_Descriptor_t const & v)
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    return m_representation->vertex_label(v);    
}

template<typename BASE>
bool Standard_Automaton_Aspect<BASE>::accepting(Vertex_Descriptor_t const & v)
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    return m_representation->accepting(v);
}

template<typename BASE>
::std::string const & Standard_Automaton_Aspect<BASE>::edge_label(Edge_Descriptor_t const & e)
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    return m_representation->edge_label(e);
}

template<typename BASE>
typename Standard_Automaton_Aspect<BASE>::Out_Edge_Iterator_Pair_t Standard_Automaton_Aspect<BASE>::out_edges(Vertex_Descriptor_t const & v)
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    return m_representation->out_edges(v);
}

template<typename BASE>
typename Standard_Automaton_Aspect<BASE>::Vertex_Descriptor_t Standard_Automaton_Aspect<BASE>::target(Edge_Descriptor_t const & e)
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    return m_representation->target(e);
}


LTL2FSM__END__NAMESPACE__LTL2FSM;


#endif
