/**
 * @file ltl2fsm/automaton/Det_Finite_Automaton.t.cpp
 *
 * $Id$
 *
 * @author Oliver Arafat
 *
 * @brief [LEVEL: alpha] testing Det_Finite_Automaton.impl.hpp
 *
 * @note DOCUMENTED
 *
 * @test
 *
 * @todo
 */


#include <diagnostics/unittest.hpp>

#include <ltl2fsm/automaton/Nondet_Finite_Automaton.impl.hpp>
#include <ltl2fsm/representation/Graphviz_Representation.hpp>

#include <sstream>


#define TEST_COMPONENT_NAME Nondet_Finite_Automaton
#define TEST_COMPONENT_NAMESPACE ltl2fsm


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;
TEST_NAMESPACE_BEGIN;
TEST_COMPONENT_TEST_NAMESPACE_BEGIN;
using namespace ::diagnostics::unittest;


/**
 * @brief A DFA with a graphviz representation
 */ 
typedef Nondet_Finite_Automaton<Graphviz_Representation> NFA_t;

/**
 * @brief Testinput for this testsuite. This dot file is a transformed
 * version of buechi_orig.dot.  It is extended with those labels that
 * are added in Ltl_To_Nba_Module, such as peripheries and epsilon
 * labels.
 */ 
::std::string const & _buechi_transformed = "test/input/buechi_transformed.dot";


void test__Nondet_Finite_Automaton_Constructor(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_buechi_transformed, graph);
    
    NFA_t nfa(new Graphviz_Representation(graph));
    NFA_t::Property_Vertex_Map_t vmap = nfa.property_vertex_map();
    NFA_t::Property_Edge_Map_t emap = nfa.property_edge_map();
    NFA_t::Vertex_Iterator_Pair_t vp = nfa.vertex_pair();
    NFA_t::Edge_Iterator_Pair_t ep = nfa.edge_pair();
    
    ////////////////////
    // TESTING FIELDS

    // Vertex iterator pair test
    TEST_ASSERT(vp.first!=vp.second);
    ++vp.first; TEST_ASSERT(vp.first!=vp.second);
    ++vp.first; TEST_ASSERT(vp.first!=vp.second);
    ++vp.first; TEST_ASSERT(vp.first==vp.second);    

    // Edge iterator pair test
    TEST_ASSERT(ep.first != ep.second);
    ++ep.first; TEST_ASSERT(ep.first != ep.second);
    ++ep.first; TEST_ASSERT(ep.first != ep.second);
    ++ep.first; TEST_ASSERT(ep.first != ep.second);
    ++ep.first; TEST_ASSERT(ep.first == ep.second);
    

}

void test__Nondet_Finite_Automaton_Copy_Constructor(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_buechi_transformed, graph);

    NFA_t nfa(new Graphviz_Representation(graph));
    NFA_t nfa_copy(nfa);
    	
    NFA_t::Vertex_Iterator_Pair_t vp = nfa_copy.vertex_pair();
    NFA_t::Edge_Iterator_Pair_t ep = nfa_copy.edge_pair();
    
    ////////////////////
    // TESTING FIELDS
    
    // Vertex iterator pair test
    TEST_ASSERT(vp.first!=vp.second);
    ++vp.first; TEST_ASSERT(vp.first!=vp.second);
    ++vp.first; TEST_ASSERT(vp.first!=vp.second);
    ++vp.first; TEST_ASSERT(vp.first==vp.second);    
    
    // Edge iterator pair test
    TEST_ASSERT(ep.first != ep.second);
    ++ep.first; TEST_ASSERT(ep.first != ep.second);
    ++ep.first; TEST_ASSERT(ep.first != ep.second);
    ++ep.first; TEST_ASSERT(ep.first != ep.second);
    ++ep.first; TEST_ASSERT(ep.first == ep.second);
}

void test__out_edges(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_buechi_transformed, graph);
    
    NFA_t nfa(new Graphviz_Representation(graph));    
    NFA_t::Vertex_Iterator_Pair_t vp = nfa.vertex_pair();
    NFA_t::Out_Edge_Iterator_Pair_t oep;
    
    // Init vertex (#0)
    TEST_ASSERT(vp.first != vp.second);
    oep = nfa.out_edges(*vp.first);
    TEST_ASSERT(oep.first != oep.second);
    TEST_ASSERT(nfa.edge_label(*oep.first) == "epsilon");
    ++oep.first; TEST_ASSERT(oep.first == oep.second);
    
    // Vertex #1
    ++vp.first; TEST_ASSERT(vp.first != vp.second);
    oep = nfa.out_edges(*vp.first);
    TEST_ASSERT(oep.first != oep.second);
    TEST_ASSERT(nfa.edge_label(*oep.first) == "a");
    ++oep.first; TEST_ASSERT(nfa.edge_label(*oep.first) == "b");
    ++oep.first; TEST_ASSERT(oep.first == oep.second);
    
    // Vertex #2
    ++vp.first; TEST_ASSERT(vp.first != vp.second);
    oep = nfa.out_edges(*vp.first);
    TEST_ASSERT(oep.first != oep.second);
    TEST_ASSERT(nfa.edge_label(*oep.first) == "b");
    ++oep.first; TEST_ASSERT(oep.first == oep.second);

    ++vp.first; TEST_ASSERT(vp.first == vp.second);
}

void test__target(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_buechi_transformed, graph);
    
    NFA_t nfa(new Graphviz_Representation(graph));    
    NFA_t::Vertex_Iterator_Pair_t vp = nfa.vertex_pair();
    NFA_t::Out_Edge_Iterator_Pair_t oep;

    // Init vertex (#0)
    TEST_ASSERT(vp.first != vp.second);
    oep = nfa.out_edges(*vp.first);
    TEST_ASSERT(oep.first != oep.second);
    TEST_ASSERT(nfa.vertex_label(nfa.target(*oep.first)) == "({(a U Gb)},\\n {(a U Gb)})");
    ++oep.first; TEST_ASSERT(oep.first == oep.second);
    
    // Vertex #1
    ++vp.first; TEST_ASSERT(vp.first != vp.second);
    oep = nfa.out_edges(*vp.first);
    TEST_ASSERT(oep.first != oep.second);
    TEST_ASSERT(nfa.vertex_label(nfa.target(*oep.first)) == "({(a U Gb)},\\n {(a U Gb)})");
    ++oep.first; TEST_ASSERT(nfa.vertex_label(nfa.target(*oep.first)) == "({Gb}, ø)");
    
    // Vertex #2
    ++vp.first; TEST_ASSERT(vp.first != vp.second);
    oep = nfa.out_edges(*vp.first);
    TEST_ASSERT(oep.first != oep.second);
    TEST_ASSERT(nfa.vertex_label(nfa.target(*oep.first)) == "({Gb}, ø)");
    ++oep.first; TEST_ASSERT(oep.first == oep.second);

    ++vp.first; TEST_ASSERT(vp.first == vp.second);
}



TEST_COMPONENT_TEST_NAMESPACE_END;
TEST_NAMESPACE_END;
LTL2FSM__END__NAMESPACE__LTL2FSM;


TEST_SUITE_BEGIN;
TEST_NORMAL_CASE(&test__Nondet_Finite_Automaton_Constructor, LEVEL_PROD);
TEST_NORMAL_CASE(&test__Nondet_Finite_Automaton_Copy_Constructor, LEVEL_PROD);
TEST_NORMAL_CASE(&test__out_edges, LEVEL_PROD);
TEST_NORMAL_CASE(&test__target, LEVEL_PROD);
TEST_SUITE_END;


STREAM_TEST_SYSTEM_MAIN;
