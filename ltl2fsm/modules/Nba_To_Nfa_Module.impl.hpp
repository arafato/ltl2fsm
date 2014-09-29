/**
 * @file ltl2fsm/modules/Nba_To_Nfa_Module.impl.hpp
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


#ifndef LTL2FSM__MODULES__NBA_TO_NFA_MODULE_IMPL__HPP
#define LTL2FSM__MODULES__NBA_TO_NFA_MODULE_IMPL__HPP


#include <ltl2fsm/modules/Nba_To_Nfa_Module.hpp>
#include <ltl2fsm/automaton/Nondet_Buechi_Automaton.impl.hpp>
#include <ltl2fsm/automaton/Nondet_Finite_Automaton.impl.hpp>

#include <stack>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;
LTL2FSM__BEGIN__NAMESPACE__INTERNAL;


/**
 * @class Nba_To_Nfa_Module_Assistant
 *
 * @brief This class offers all neccessary components,
 * i.e. datastructures and convenience methods, to @ref
 * Nba_To_Nfa_Module to transform a NBA to a NFA.
 *
 * @nosubgrouping
 */
template<typename REP>
class Nba_To_Nfa_Module_Assistant
{
    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Types
     * @{
     */
private:
    typedef REP Representation_t;
    typedef Nba_To_Nfa_Module_Assistant<REP> Self;

    typedef typename Representation_t::Vertex_Descriptor_t Vertex_Descriptor_t;
    typedef typename Representation_t::Vertex_Iterator_Pair_t Vertex_Iterator_Pair_t;
    typedef typename Representation_t::Out_Edge_Iterator_Pair_t Out_Edge_Iterator_Pair_t;

public:
    typedef Nondet_Buechi_Automaton<Representation_t> NBA_t;

    /**
     * @brief DFS stack.
     */ 
    typedef ::std::stack<Vertex_Descriptor_t> Dfs_Stack_t;    
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Construction / Destruction
     * @{
     */
public:
    explicit Nba_To_Nfa_Module_Assistant(NBA_t & nba);
    virtual ~Nba_To_Nfa_Module_Assistant();
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Convenience methods
     * @{
     */
public:
    /**
     * @brief Dfs-search from vertex @a v. This method returns true if
     * there is a path from vertex @a v to an accepting state.
     */ 
    bool dfs(Vertex_Descriptor_t const & v) const;
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name State
     * @{
     */
private:
    NBA_t * m_nba;
    // @}


#if LTL2FSM_DEBUG__LEVEL__ > 1
public:
	void m_class_invariance() const;
#endif
};

#if LTL2FSM_DEBUG__LEVEL__ > 1
template<typename REP>
void Nba_To_Nfa_Module_Assistant<REP>::m_class_invariance() const
{
}
#endif

template<typename REP>
Nba_To_Nfa_Module_Assistant<REP>::Nba_To_Nfa_Module_Assistant(NBA_t & nba)
    : m_nba(&nba)
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT;
}

template<typename REP>
Nba_To_Nfa_Module_Assistant<REP>::~Nba_To_Nfa_Module_Assistant()
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT;
}

template<typename REP>
bool Nba_To_Nfa_Module_Assistant<REP>::dfs(Vertex_Descriptor_t const & v) const
{
    LTL2FSM_METHOD_GUARD("");
    Dfs_Stack_t dfs_stack;
    dfs_stack.push(v);
    
    while(!dfs_stack.empty()){
	Vertex_Descriptor_t v = dfs_stack.top();
	dfs_stack.pop();

	if(m_nba->accepting(v))
	    return true;
	
	Out_Edge_Iterator_Pair_t oep = m_nba->out_edges(v);
	for(; oep.first != oep.second; ++oep.first){
	    if(m_nba->target(*oep.first) != v)
		dfs_stack.push(m_nba->target(*oep.first));
	}
	
    }
        
    return false;
}


LTL2FSM__END__NAMESPACE__INTERNAL;


//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////


#if LTL2FSM_DEBUG__LEVEL__ > 1
template<typename REP>
void Nba_To_Nfa_Module<REP>::m_class_invariance() const
{
}
#endif

template<typename REP>
Nba_To_Nfa_Module<REP>::Nba_To_Nfa_Module()
    : Module()
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT;
}

template<typename REP>
Nba_To_Nfa_Module<REP>::~Nba_To_Nfa_Module()
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT;
}

template<typename REP>
typename Nba_To_Nfa_Module<REP>::NFA_t * const Nba_To_Nfa_Module<REP>::execute(NBA_t & nba)
{
    LTL2FSM_METHOD_GUARD("");

    typedef ::ltl2fsm::internal::Nba_To_Nfa_Module_Assistant<Representation_t> Module_Assistant_t;
    Module_Assistant_t assistant(nba);
    typename Module_Assistant_t::Dfs_Stack_t vstack;

    Accepting_Components_t acc_comp;
    Vertex_Iterator_Pair_t vp = nba.vertex_pair();
    Strong_Components_t component;

    nba.strong_components(component);
    
    for(; vp.first != vp.second; ++vp.first){
	if(nba.accepting(*vp.first))
	    acc_comp.insert(component[*vp.first]);	
    }

    for(vp = nba.vertex_pair(); vp.first != vp.second; ++vp.first){
	Accepting_Components_t::const_iterator iter(acc_comp.find(component[*vp.first]));
	if(iter != acc_comp.end() && !nba.accepting(*vp.first))
 	    nba.set_accepting(*vp.first);
	else if(iter == acc_comp.end() && nba.vertex_label(*vp.first) != "")
	    vstack.push(*vp.first);
    }

    while(!vstack.empty()){

	Vertex_Descriptor_t v = vstack.top();
	vstack.pop();

	if(assistant.dfs(v))
	    nba.set_accepting(v);
    }

    return new NFA_t(nba.p_representation());
}


LTL2FSM__END__NAMESPACE__LTL2FSM;


#endif
