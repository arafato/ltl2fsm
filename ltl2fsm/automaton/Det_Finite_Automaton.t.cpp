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

#include <ltl2fsm/automaton/Det_Finite_Automaton.impl.hpp>
#include <ltl2fsm/representation/Graphviz_Representation.hpp>

#include <sstream>


#define TEST_COMPONENT_NAME Det_Finite_Automaton
#define TEST_COMPONENT_NAMESPACE ltl2fsm


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;
TEST_NAMESPACE_BEGIN;
TEST_COMPONENT_TEST_NAMESPACE_BEGIN;
using namespace ::diagnostics::unittest;


/**
 * @brief A DFA with a graphviz representation
 */ 
typedef Det_Finite_Automaton<Graphviz_Representation> DFA_t;

/**
 * @brief Testinput for this testsuite. This dot file is a transformed
 * version of buechi_orig.dot.  It is extended with those labels that
 * are added in Ltl_To_Nba_Module, such as peripheries and epsilon
 * labels.
 */ 
::std::string const & _buechi_transformed = "test/input/buechi_transformed.dot";


void test__Det_Finite_Automaton_Constructor(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_buechi_transformed, graph);
    
    DFA_t dfa(new Graphviz_Representation(graph));

    DFA_t::Vertex_Iterator_Pair_t vp = dfa.vertex_pair();
    DFA_t::Edge_Iterator_Pair_t ep = dfa.edge_pair();

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

void test__Det_Finite_Automaton_Copy_Constructor(Test_Data & test_data)
{

     ::boost::GraphvizDigraph graph;
     ::boost::read_graphviz(_buechi_transformed, graph);

     DFA_t dfa(new Graphviz_Representation(graph));
     DFA_t dfa_copy(dfa);

     DFA_t::Vertex_Iterator_Pair_t vp = dfa_copy.vertex_pair();
     DFA_t::Edge_Iterator_Pair_t ep = dfa_copy.edge_pair();

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

void test__Det_Finite_Automaton_Std_Constructor(Test_Data & test_data)
{
    DFA_t dfa;

    DFA_t::Vertex_Iterator_Pair_t vp(dfa.vertex_pair());
    TEST_ASSERT(vp.first == vp.second);

    DFA_t::Edge_Iterator_Pair_t ep(dfa.edge_pair());
    TEST_ASSERT(ep.first == ep.second);
}


void test__add_vertex(Test_Data & test_data)
{
    DFA_t dfa;
    DFA_t::Vertex_Descriptor_t v = dfa.add_vertex();
    DFA_t::Property_Vertex_Map_t vmap = dfa.property_vertex_map();
    
    
    TEST_ASSERT(!dfa.accepting(v));
    TEST_ASSERT(dfa.vertex_label(v) == "unnamed");
    TEST_ASSERT(vmap[v]["shape"] == "ellipse");
    TEST_ASSERT(vmap[v]["fontname"] == "\"TimesNewRoman\"");
}

void test__add_edge(Test_Data & test_data)
{
    DFA_t dfa;
    DFA_t::Vertex_Descriptor_t v1 = dfa.add_vertex();
    DFA_t::Vertex_Descriptor_t v2 = dfa.add_vertex();

    DFA_t::Edge_Descriptor_t e = dfa.add_edge(v1, v2);
    TEST_ASSERT(dfa.edge_label(e) == "unnamed");

    DFA_t::Property_Edge_Map_t emap = dfa.property_edge_map();
    TEST_ASSERT(emap[e]["fontname"] == "\"TimesNewRoman\"");
}

void test__set_accepting(Test_Data & test_data)
{
    DFA_t dfa;
    DFA_t::Vertex_Descriptor_t v = dfa.add_vertex();

    dfa.set_accepting(v);
    TEST_ASSERT(dfa.accepting(v));
    dfa.set_accepting(v);
    TEST_ASSERT(dfa.accepting(v));
}

void test__set_label_vertex(Test_Data & test_data)
{
    DFA_t dfa;
    DFA_t::Vertex_Descriptor_t v = dfa.add_vertex();

    dfa.set_label(v, "Vertex");
    TEST_ASSERT(dfa.vertex_label(v) == "Vertex");
    dfa.set_label(v, "newName");
    TEST_ASSERT(dfa.vertex_label(v) == "newName");
}

void test__set_label_edge(Test_Data & test_data)
{
    DFA_t dfa;
    DFA_t::Vertex_Descriptor_t v1 = dfa.add_vertex();
    DFA_t::Vertex_Descriptor_t v2 = dfa.add_vertex();
    DFA_t::Edge_Descriptor_t e = dfa.add_edge(v1, v2);
    
    dfa.set_label(e, "Edge");
    TEST_ASSERT(dfa.edge_label(e) == "Edge");
    dfa.set_label(e, "newName");
    TEST_ASSERT(dfa.edge_label(e) == "newName");
}

void test__out_edges(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_buechi_transformed, graph);
    
    DFA_t dfa(new Graphviz_Representation(graph));    
    DFA_t::Vertex_Iterator_Pair_t vp = dfa.vertex_pair();
    DFA_t::Out_Edge_Iterator_Pair_t oep;
    
    // Init vertex (#0)
    TEST_ASSERT(vp.first != vp.second);
    oep = dfa.out_edges(*vp.first);
    TEST_ASSERT(oep.first != oep.second);
    TEST_ASSERT(dfa.edge_label(*oep.first) == "epsilon");
    ++oep.first; TEST_ASSERT(oep.first == oep.second);
    
    // Vertex #1
    ++vp.first; TEST_ASSERT(vp.first != vp.second);
    oep = dfa.out_edges(*vp.first);
    TEST_ASSERT(oep.first != oep.second);
    TEST_ASSERT(dfa.edge_label(*oep.first) == "a");
    ++oep.first; TEST_ASSERT(dfa.edge_label(*oep.first) == "b");
    ++oep.first; TEST_ASSERT(oep.first == oep.second);
    
    // Vertex #2
    ++vp.first; TEST_ASSERT(vp.first != vp.second);
    oep = dfa.out_edges(*vp.first);
    TEST_ASSERT(oep.first != oep.second);
    TEST_ASSERT(dfa.edge_label(*oep.first) == "b");
    ++oep.first; TEST_ASSERT(oep.first == oep.second);

    ++vp.first; TEST_ASSERT(vp.first == vp.second);
}

void test__target(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_buechi_transformed, graph);
    
    DFA_t dfa(new Graphviz_Representation(graph));    
    DFA_t::Vertex_Iterator_Pair_t vp = dfa.vertex_pair();
    DFA_t::Out_Edge_Iterator_Pair_t oep;

    // Init vertex (#0)
    TEST_ASSERT(vp.first != vp.second);
    oep = dfa.out_edges(*vp.first);
    TEST_ASSERT(oep.first != oep.second);
    TEST_ASSERT(dfa.vertex_label(dfa.target(*oep.first)) == "({(a U Gb)},\\n {(a U Gb)})");
    ++oep.first; TEST_ASSERT(oep.first == oep.second);
    
    // Vertex #1
    ++vp.first; TEST_ASSERT(vp.first != vp.second);
    oep = dfa.out_edges(*vp.first);
    TEST_ASSERT(oep.first != oep.second);
    TEST_ASSERT(dfa.vertex_label(dfa.target(*oep.first)) == "({(a U Gb)},\\n {(a U Gb)})");
    ++oep.first; TEST_ASSERT(dfa.vertex_label(dfa.target(*oep.first)) == "({Gb}, ø)");
    
    // Vertex #2
    ++vp.first; TEST_ASSERT(vp.first != vp.second);
    oep = dfa.out_edges(*vp.first);
    TEST_ASSERT(oep.first != oep.second);
    TEST_ASSERT(dfa.vertex_label(dfa.target(*oep.first)) == "({Gb}, ø)");
    ++oep.first; TEST_ASSERT(oep.first == oep.second);

    ++vp.first; TEST_ASSERT(vp.first == vp.second);
}

/**
 * @brief Vertex descriptor:   Label:                        
 *           0                 "" (empty string)             
 *           1                 ({(a U Gb)},\n {(a U Gb)})    
 *           2                 ({Gb}, ø)                     
 */ 
void test__vertex_descriptor(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_buechi_transformed, graph);
    
    DFA_t dfa(new Graphviz_Representation(graph));

    TEST_EXCEPTIONLESS_BLOCK_ENTER;
    TEST_ASSERT(dfa.vertex_descriptor("") == 0);
    TEST_ASSERT(dfa.vertex_descriptor("({(a U Gb)},\\n {(a U Gb)})") == 1);
    TEST_ASSERT(dfa.vertex_descriptor("({Gb}, ø)") == 2);
    TEST_EXCEPTIONLESS_BLOCK_EXIT;
    
    TEST_THROWING_BLOCK_ENTER;
    dfa.vertex_descriptor("Not_Existing_Vertex_Name");
    TEST_THROWING_BLOCK_EXIT(Exception);
}


TEST_COMPONENT_TEST_NAMESPACE_END;
TEST_NAMESPACE_END;
LTL2FSM__END__NAMESPACE__LTL2FSM;


TEST_SUITE_BEGIN;
TEST_NORMAL_CASE(&test__Det_Finite_Automaton_Constructor, LEVEL_PROD);
TEST_NORMAL_CASE(&test__Det_Finite_Automaton_Copy_Constructor, LEVEL_PROD);
TEST_NORMAL_CASE(&test__Det_Finite_Automaton_Std_Constructor, LEVEL_PROD);
TEST_NORMAL_CASE(&test__add_vertex, LEVEL_PROD);
TEST_NORMAL_CASE(&test__add_edge, LEVEL_PROD);
TEST_NORMAL_CASE(&test__set_accepting, LEVEL_PROD);
TEST_NORMAL_CASE(&test__set_label_vertex, LEVEL_PROD);
TEST_NORMAL_CASE(&test__set_label_edge, LEVEL_PROD);
TEST_NORMAL_CASE(&test__out_edges, LEVEL_PROD);
TEST_NORMAL_CASE(&test__target, LEVEL_PROD);
TEST_NORMAL_CASE(&test__vertex_descriptor, LEVEL_PROD);
TEST_SUITE_END;


STREAM_TEST_SYSTEM_MAIN;
