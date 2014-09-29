/**
 * @file ltl2fsm/automaton/Det_Finite_Automaton.impl.hpp
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


#ifndef LTL2FSM__AUTOMATON__DET_FINITE_AUTOMATON_IMPL_HPP
#define LTL2FSM__AUTOMATON__DET_FINITE_AUTOMATON_IMPL_HPP


#include <ltl2fsm/automaton/Det_Finite_Automaton.hpp>

#include <ltl2fsm/base/exception/Edge_Already_Added.hpp>
#include <ltl2fsm/base/exception/Invalid_Vertex_Name.hpp>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


#if LTL2FSM_DEBUG__LEVEL__ > 1
template<typename REP>
void Det_Finite_Automaton<REP>::m_class_invariance() const
{
}
#endif


template<typename REP>
Det_Finite_Automaton<REP>::Det_Finite_Automaton()
    : Super()
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT;
}

template<typename REP>
Det_Finite_Automaton<REP>::Det_Finite_Automaton(Representation_t * const representation)
    : Super(representation)
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT;
}

template<typename REP>
Det_Finite_Automaton<REP>::Det_Finite_Automaton(Self const & other)
    : Super(other)
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT;
}

template<typename REP>
Det_Finite_Automaton<REP>::~Det_Finite_Automaton()
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT;
}

template<typename REP>
typename Det_Finite_Automaton<REP>::Vertex_Descriptor_t Det_Finite_Automaton<REP>::add_vertex()
{
    LTL2FSM_METHOD_GUARD("");
    return Super::p_representation()->add_vertex();
}

template<typename REP>
typename Det_Finite_Automaton<REP>::Edge_Descriptor_t Det_Finite_Automaton<REP>::add_edge(Vertex_Descriptor_t const & source,
											  Vertex_Descriptor_t const & target)
{
    LTL2FSM_METHOD_GUARD("");

    Edge_Descriptor_t tmp;
    try{
	tmp = Super::p_representation()->add_edge(source, target);
    }
    catch(Edge_Already_Added const & e){
	throw;
    }
    
    return tmp;
}

template<typename REP>
void Det_Finite_Automaton<REP>::set_label(Vertex_Descriptor_t const & v,
					  ::std::string const & name)
{
    LTL2FSM_METHOD_GUARD("");
    Super::p_representation()->set_label(v, name);
}

template<typename REP>
void Det_Finite_Automaton<REP>::set_label(Edge_Descriptor_t const & e,
					  ::std::string const & name)
{
    LTL2FSM_METHOD_GUARD("");
    Super::p_representation()->set_label(e, name);
}

template<typename REP>
void Det_Finite_Automaton<REP>::set_accepting(Vertex_Descriptor_t const & v)
{
    LTL2FSM_METHOD_GUARD("");
    Super::p_representation()->set_accepting(v);
}

template<typename REP>
typename Det_Finite_Automaton<REP>::Vertex_Descriptor_t const & Det_Finite_Automaton<REP>::vertex_descriptor(::std::string const & name)
{
    LTL2FSM_METHOD_GUARD("");

    try{
	return Super::p_representation()->vertex_descriptor(name);
    }
    catch(Invalid_Vertex_Name const & e){
	throw;
    }
}

template<typename REP>
void Det_Finite_Automaton<REP>::set_top(Vertex_Descriptor_t const & v)
{
    LTL2FSM_METHOD_GUARD("");

    Super::p_representation()->set_top(v);
}

template<typename REP>
void Det_Finite_Automaton<REP>::set_bottom(Vertex_Descriptor_t const & v)
{
    LTL2FSM_METHOD_GUARD("");

    Super::p_representation()->set_bottom(v);
}

template<typename REP>
void Det_Finite_Automaton<REP>::set_inconclusive(Vertex_Descriptor_t const & v)
{
    LTL2FSM_METHOD_GUARD("");

    Super::p_representation()->set_inconclusive(v);
}

template<typename REP>
bool Det_Finite_Automaton<REP>::top(Vertex_Descriptor_t const & v)
{
    LTL2FSM_METHOD_GUARD("");

    return Super::p_representation()->top(v);
}

template<typename REP>
bool Det_Finite_Automaton<REP>::bottom(Vertex_Descriptor_t const & v)
{
    LTL2FSM_METHOD_GUARD("");

    return Super::p_representation()->bottom(v);
}

template<typename REP>
bool Det_Finite_Automaton<REP>::inconclusive(Vertex_Descriptor_t const & v)
{
    LTL2FSM_METHOD_GUARD("");

    return Super::p_representation()->inconclusive(v);
}


LTL2FSM__END__NAMESPACE__LTL2FSM;


#endif
