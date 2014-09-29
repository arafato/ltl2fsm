/**
 * @file ltl2fsm/automaton/Nondet_Buechi_Automaton.t.cpp
 *
 * $Id$
 *
 * @author Oliver Arafat
 *
 * @brief [LEVEL: alpha] testing Nondet_Buechi_Automaton.impl.hpp
 *
 * @note DOCUMENTED
 *
 * @test
 *
 * @todo
 */


#include <diagnostics/unittest.hpp>

#include <ltl2fsm/automaton/Nondet_Buechi_Automaton.impl.hpp>
#include <ltl2fsm/representation/Graphviz_Representation.hpp>

#include <sstream>


#define TEST_COMPONENT_NAME Nondet_Buechi_Automaton
#define TEST_COMPONENT_NAMESPACE ltl2fsm


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;
TEST_NAMESPACE_BEGIN;
TEST_COMPONENT_TEST_NAMESPACE_BEGIN;
using namespace ::diagnostics::unittest;


typedef Nondet_Buechi_Automaton<Graphviz_Representation> NBA_t;

/**
 * @brief Testinput for this testsuite. This dot file is a transformed
 * version of buechi_orig.dot.  It is extended with those labels that
 * are added in Ltl_To_Nba_Module, such as peripheries and epsilon
 * labels.
 */ 
::std::string const & _buechi_transformed = "test/input/buechi_transformed.dot";

/**
 * @brief The number of vertices of the graph described by
 * buechi_transformed.dot.
 */ 
int const NUM_OF_VERTICES = 3;

/**
 * @brief The number of edges of the graph described by
 * buechi_transformed.dot.
 */ 
int const NUM_OF_EDGES = 4;


void test__Nondet_Buechi_Automaton_Constructor(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_buechi_transformed, graph);

    NBA_t * nba = new NBA_t(new Graphviz_Representation(graph));
        
    TEST_ASSERT(nba != 0);

    delete nba;
}

void test__Nondet_Buechi_Automaton_Copy_Constructor(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_buechi_transformed, graph);

    NBA_t * nba = new NBA_t(new Graphviz_Representation(graph));
    TEST_ASSERT(nba != 0);
    NBA_t * nba_copy = new NBA_t(new Graphviz_Representation(graph));
    TEST_ASSERT(nba_copy != 0);
    
    delete nba;
    delete nba_copy;
}

/**
 * @brief The dot-file used here as test-input describes a graph with three vertices
 * (including the init vertex).
 */ 
void test__vertex_pair(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_buechi_transformed, graph);

    NBA_t * nba = new NBA_t(new Graphviz_Representation(graph));
    NBA_t::Vertex_Iterator_Pair_t vp = nba->vertex_pair();
    
    int num_of_vertices = 0;
    for(; vp.first != vp.second; ++vp.first){
	++num_of_vertices;
    }

    TEST_ASSERT(num_of_vertices == NUM_OF_VERTICES);
    delete nba;
}

/**
 * @brief The dot-file used here as test-input describes a graph with
 * four edges (including epsilon transitions).
 */ 
void test__edge_pair(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_buechi_transformed, graph);

    NBA_t * nba = new NBA_t(new Graphviz_Representation(graph));
    NBA_t::Edge_Iterator_Pair_t ep = nba->edge_pair();
    
    int num_of_edges = 0;
    for(; ep.first != ep.second; ++ep.first){
	++num_of_edges;
    }

    TEST_ASSERT(num_of_edges == NUM_OF_EDGES);
    delete nba;
}

/**
 * @brief The third vertex is supposed to be accepting, the first and
 * second one are supposed to be non-accepting.
 */ 
void test__accepting(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_buechi_transformed, graph);
    
    NBA_t * nba = new NBA_t(new Graphviz_Representation(graph));
    NBA_t::Vertex_Iterator_Pair_t vp = nba->vertex_pair();


    for(int i = 0; vp.first != vp.second; ++vp.first, ++i){
	switch(i){
	    case 0: TEST_ASSERT(!nba->accepting(*vp.first));
		break;
	    case 1: TEST_ASSERT(!nba->accepting(*vp.first));
		break;
	    case 2: TEST_ASSERT(nba->accepting(*vp.first));
		break;
	}
    }

    delete nba;
}

/**
 * @brief Vertex:   Expected value:
 *           0      "" (empty string)
 *           1      ({(a U Gb)},\n {(a U Gb)})
 *           2      ({Gb}, ø)
 */ 
void test__vertex_label(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_buechi_transformed, graph);
    
    NBA_t * nba = new NBA_t(new Graphviz_Representation(graph));
    NBA_t::Vertex_Iterator_Pair_t vp = nba->vertex_pair();


    for(int i = 0; vp.first != vp.second; ++vp.first, ++i){
	switch(i){
	    case 0: TEST_ASSERT(nba->vertex_label(*vp.first) == "");
		break;
	    case 1: TEST_ASSERT(nba->vertex_label(*vp.first) == "({(a U Gb)},\\n {(a U Gb)})");
		break;
	    case 2: TEST_ASSERT(nba->vertex_label(*vp.first) == "({Gb}, ø)");
		break;
	}
    }

    delete nba;
}

/**
 * @brief Edge:     Expected value:
 *           0      epsilon
 *           1      a
 *           2      b
 *           3      b
 */ 
void test__edge_label(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_buechi_transformed, graph);
    
    NBA_t * nba = new NBA_t(new Graphviz_Representation(graph));
    NBA_t::Edge_Iterator_Pair_t ep = nba->edge_pair();


    for(int i = 0; ep.first != ep.second; ++ep.first, ++i){
	switch(i){
	    case 0: TEST_ASSERT(nba->edge_label(*ep.first) == "epsilon");
		break;
	    case 1: TEST_ASSERT(nba->edge_label(*ep.first) == "a");
		break;
	    case 2: TEST_ASSERT(nba->edge_label(*ep.first) == "b");
		break;
	    case 3: TEST_ASSERT(nba->edge_label(*ep.first) == "b");
		break;
	}
    }

    delete nba;
}

/**
 * @brief Vertex:     Expected SCC:
 *            0       2
 *            1       1
 *            2       0
 */ 
void test__strong_components(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_buechi_transformed, graph);
    
    NBA_t * nba = new NBA_t(new Graphviz_Representation(graph));
    NBA_t::Vertex_Iterator_Pair_t vp = nba->vertex_pair();
    
    NBA_t::Strong_Components_t components;
    nba->strong_components(components);
  
    for(int i = 2; vp.first != vp.second; ++vp.first, --i){
	switch(i){
	    case 2: TEST_ASSERT(components[*vp.first] == 2);
		break;
	    case 1: TEST_ASSERT(components[*vp.first] == 1);
		break;
	    case 0: TEST_ASSERT(components[*vp.first] == 0);
		break;
	}
    }
    delete nba;
}





TEST_COMPONENT_TEST_NAMESPACE_END;
TEST_NAMESPACE_END;
LTL2FSM__END__NAMESPACE__LTL2FSM;


TEST_SUITE_BEGIN;
TEST_NORMAL_CASE(&test__Nondet_Buechi_Automaton_Constructor, LEVEL_PROD);
TEST_NORMAL_CASE(&test__Nondet_Buechi_Automaton_Copy_Constructor, LEVEL_PROD);
TEST_NORMAL_CASE(&test__vertex_pair, LEVEL_PROD);
TEST_NORMAL_CASE(&test__edge_pair, LEVEL_PROD);
TEST_NORMAL_CASE(&test__accepting, LEVEL_PROD);
TEST_NORMAL_CASE(&test__vertex_label, LEVEL_PROD);
TEST_NORMAL_CASE(&test__edge_label, LEVEL_PROD);
TEST_NORMAL_CASE(&test__strong_components, LEVEL_PROD);
TEST_SUITE_END;


STREAM_TEST_SYSTEM_MAIN;
