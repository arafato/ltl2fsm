/**
 * @file ltl2fsm/representation/Graphviz_Representation.t.cpp
 *
 * $Id$
 *
 * @author Oliver Arafat
 *
 * @brief [LEVEL: alpha] testing Ltl_To_Nba_Module.hpp
 *
 * @note DOCUMENTED
 *
 * @test
 *
 * @todo
 */


#include <diagnostics/unittest.hpp>

#include <ltl2fsm/representation/Graphviz_Representation.hpp>

#include <sstream>


#define TEST_COMPONENT_NAME Graphviz_Representation
#define TEST_COMPONENT_NAMESPACE ltl2fsm


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;
TEST_NAMESPACE_BEGIN;
TEST_COMPONENT_TEST_NAMESPACE_BEGIN;
using namespace ::diagnostics::unittest;


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


void test__Graphviz_Constructor(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_buechi_transformed, graph);

    Graphviz_Representation rep(graph);
    Graphviz_Representation::Property_Vertex_Map_t vmap = rep.property_vertex_map();
    Graphviz_Representation::Property_Edge_Map_t emap = rep.property_edge_map();
    Graphviz_Representation::Vertex_Iterator_Pair_t vp = rep.vertex_pair();
    Graphviz_Representation::Edge_Iterator_Pair_t ep = rep.edge_pair();
    
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

    // Testing property maps...really meaningful?  Is already tested
    // in test__add_vertex and test__add_edge
}

void test__Graphviz_Copy_Constructor(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_buechi_transformed, graph);

    Graphviz_Representation rep(graph);
    Graphviz_Representation rep_copy(rep);

    Graphviz_Representation::Vertex_Iterator_Pair_t vp = rep_copy.vertex_pair();
    Graphviz_Representation::Edge_Iterator_Pair_t ep = rep_copy.edge_pair();

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


/**
 * @brief The dot-file used here as test-input describes a graph with three vertices
 * (including the init vertex).
 */ 
void test__vertex_pair(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_buechi_transformed, graph);

    Graphviz_Representation rep(graph);
    Graphviz_Representation::Vertex_Iterator_Pair_t vp(rep.vertex_pair());
    
    int num_of_vertices = 0;
    for(; vp.first != vp.second; ++vp.first){
	++num_of_vertices;
    }

    TEST_ASSERT(num_of_vertices == NUM_OF_VERTICES);


    // Testing en empty pair
    Graphviz_Representation rep2;
    Graphviz_Representation::Vertex_Iterator_Pair_t vp2(rep2.vertex_pair());
    TEST_ASSERT(vp.first == vp.second);
}

/**
 * @brief The dot-file used here as test-input describes a graph with
 * four edges (including epsilon transitions).
 */ 
void test__edge_pair(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_buechi_transformed, graph);

    Graphviz_Representation rep(graph);
    Graphviz_Representation::Edge_Iterator_Pair_t ep = rep.edge_pair();
    
    int num_of_edges = 0;
    for(; ep.first != ep.second; ++ep.first){
	++num_of_edges;
    }

    TEST_ASSERT(num_of_edges == NUM_OF_EDGES);


    // Testing an empty pair
    Graphviz_Representation rep2;
    Graphviz_Representation::Edge_Iterator_Pair_t ep2(rep2.edge_pair());
    TEST_ASSERT(ep2.first == ep2.second);
}

/**
 * @brief The third vertex is supposed to be accepting, the first and
 * second one are supposed to be non-accepting.
 */ 
void test__accepting(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_buechi_transformed, graph);
    
    Graphviz_Representation rep(graph);
    Graphviz_Representation::Vertex_Iterator_Pair_t vp(rep.vertex_pair());


    TEST_ASSERT(vp.first != vp.second);
    TEST_ASSERT(!rep.accepting(*vp.first));

    ++vp.first; TEST_ASSERT(vp.first != vp.second);
    TEST_ASSERT(!rep.accepting(*vp.first));

    ++vp.first; TEST_ASSERT(vp.first != vp.second);
    TEST_ASSERT(rep.accepting(*vp.first));

    ++vp.first; TEST_ASSERT(vp.first == vp.second);
}

void test__bottom(Test_Data & test_data)
{
    Graphviz_Representation rep;
    Graphviz_Representation::Vertex_Descriptor_t v = rep.add_vertex();
    rep.set_bottom(v);
    TEST_ASSERT(rep.bottom(v));
}

void test__top(Test_Data & test_data)
{
   Graphviz_Representation rep;
   Graphviz_Representation::Vertex_Descriptor_t v = rep.add_vertex();
   rep.set_top(v);
   TEST_ASSERT(rep.top(v));
}

void test__inconclusive(Test_Data & test_data)
{
   Graphviz_Representation rep;
   Graphviz_Representation::Vertex_Descriptor_t v = rep.add_vertex();
   rep.set_inconclusive(v);
   TEST_ASSERT(rep.inconclusive(v));
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
    
    Graphviz_Representation rep(graph);
    Graphviz_Representation::Vertex_Iterator_Pair_t vp(rep.vertex_pair());


    TEST_ASSERT(vp.first != vp.second);
    TEST_ASSERT(rep.vertex_label(*vp.first) == "");

    ++vp.first; TEST_ASSERT(vp.first != vp.second);
    TEST_ASSERT(rep.vertex_label(*vp.first) == "({(a U Gb)},\\n {(a U Gb)})");

    ++vp.first; TEST_ASSERT(vp.first != vp.second);
    TEST_ASSERT(rep.vertex_label(*vp.first) == "({Gb}, ø)");

    ++vp.first; TEST_ASSERT(vp.first == vp.second);
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
    
    Graphviz_Representation rep(graph);
    Graphviz_Representation::Edge_Iterator_Pair_t ep(rep.edge_pair());


    TEST_ASSERT(ep.first != ep.second);
    TEST_ASSERT(rep.edge_label(*ep.first) == "epsilon");

    ++ep.first; TEST_ASSERT(ep.first != ep.second);
    TEST_ASSERT(rep.edge_label(*ep.first) == "a");

    ++ep.first; TEST_ASSERT(ep.first != ep.second);
    TEST_ASSERT(rep.edge_label(*ep.first) == "b");

    ++ep.first; TEST_ASSERT(ep.first != ep.second);
    TEST_ASSERT(rep.edge_label(*ep.first) == "b");

    ++ep.first; TEST_ASSERT(ep.first == ep.second);
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
    
    Graphviz_Representation rep(graph);
    Graphviz_Representation::Vertex_Iterator_Pair_t vp(rep.vertex_pair());
    Graphviz_Representation::Strong_Components_t components;

    rep.strong_components(components);


    TEST_ASSERT(vp.first!=vp.second);
    TEST_ASSERT(components[*vp.first] == 2);

    ++vp.first; TEST_ASSERT(vp.first!=vp.second);
    TEST_ASSERT(components[*vp.first] == 1);

    ++vp.first; TEST_ASSERT(vp.first!=vp.second);
    TEST_ASSERT(components[*vp.first] == 0);

    ++vp.first; TEST_ASSERT(vp.first==vp.second);
}

void test__empty_Graphviz_Constructor(Test_Data & test_data)
{
   Graphviz_Representation rep;

   Graphviz_Representation::Vertex_Iterator_Pair_t vp(rep.vertex_pair());
   TEST_ASSERT(vp.first == vp.second);

   Graphviz_Representation::Edge_Iterator_Pair_t ep(rep.edge_pair());
   TEST_ASSERT(ep.first == ep.second);
}

void test__add_vertex(Test_Data & test_data)
{
    Graphviz_Representation rep;
    Graphviz_Representation::Vertex_Descriptor_t v = rep.add_vertex();
    Graphviz_Representation::Property_Vertex_Map_t const & vmap = rep.property_vertex_map();

    
    //TEST_THROWING_BLOCK_ENTER;
    // ...
    //TEST_THROWING_BLOCK_EXIT(Unknown_Vertex_Descriptor);


    TEST_ASSERT(!rep.accepting(v));
    TEST_ASSERT(rep.vertex_label(v) == "unnamed");
    TEST_ASSERT(vmap[v]["shape"] == "ellipse");
    TEST_ASSERT(vmap[v]["fontname"] == "\"TimesNewRoman\"");
}

void test__add_edge(Test_Data & test_data)
{
    Graphviz_Representation rep;
    Graphviz_Representation::Vertex_Descriptor_t v1 = rep.add_vertex();
    Graphviz_Representation::Vertex_Descriptor_t v2 = rep.add_vertex();
    Graphviz_Representation::Edge_Descriptor_t e = rep.add_edge(v1, v2);
    Graphviz_Representation::Property_Edge_Map_t emap = rep.property_edge_map();


    //TEST_THROWING_BLOCK_ENTER;
    // ...
    //TEST_THROWING_BLOCK_EXIT(Unknown_Edge_Descriptor);


    TEST_ASSERT(rep.edge_label(e) == "unnamed");
    TEST_ASSERT(emap[e]["fontname"] == "\"TimesNewRoman\"");
}

void test__set_accepting(Test_Data & test_data)
{
    Graphviz_Representation rep;
    Graphviz_Representation::Vertex_Descriptor_t v = rep.add_vertex();

    //TEST_THROWING_BLOCK_ENTER;
    // ...
    //TEST_THROWING_BLOCK_EXIT(Unknown_Edge_Descriptor);

    rep.set_accepting(v);
    TEST_ASSERT(rep.accepting(v));
    rep.set_accepting(v);
    TEST_ASSERT(rep.accepting(v));
}

void test__set_label_vertex(Test_Data & test_data)
{
    Graphviz_Representation rep;
    Graphviz_Representation::Vertex_Descriptor_t v = rep.add_vertex();


    //TEST_THROWING_BLOCK_ENTER;
    // ...
    //TEST_THROWING_BLOCK_EXIT(Unknown_Edge_Descriptor);


    rep.set_label(v, "Vertex");
    TEST_ASSERT(rep.vertex_label(v) == "Vertex");
    rep.set_label(v, "newName");
    TEST_ASSERT(rep.vertex_label(v) == "newName");
}

void test__set_label_edge(Test_Data & test_data)
{
    Graphviz_Representation rep;
    Graphviz_Representation::Vertex_Descriptor_t v1 = rep.add_vertex();
    Graphviz_Representation::Vertex_Descriptor_t v2 = rep.add_vertex();
    Graphviz_Representation::Edge_Descriptor_t e = rep.add_edge(v1, v2);

    
    //TEST_THROWING_BLOCK_ENTER;
    // ...
    //TEST_THROWING_BLOCK_EXIT(Unknown_Edge_Descriptor);


    rep.set_label(e, "Edge");
    TEST_ASSERT(rep.edge_label(e) == "Edge");
    rep.set_label(e, "newName");
    TEST_ASSERT(rep.edge_label(e) == "newName");
}

void test__out_edges(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_buechi_transformed, graph);
    
    Graphviz_Representation rep(graph);
    Graphviz_Representation::Vertex_Iterator_Pair_t vp(rep.vertex_pair());
    Graphviz_Representation::Out_Edge_Iterator_Pair_t oep;
            

    // Init vertex (#0)
    TEST_ASSERT(vp.first != vp.second);
    oep = rep.out_edges(*vp.first);
    TEST_ASSERT(oep.first != oep.second);
    TEST_ASSERT(rep.edge_label(*oep.first) == "epsilon");
    ++oep.first; TEST_ASSERT(oep.first == oep.second);
    
    // Vertex #1
    ++vp.first; TEST_ASSERT(vp.first != vp.second);
    oep = rep.out_edges(*vp.first);
    TEST_ASSERT(oep.first != oep.second);
    TEST_ASSERT(rep.edge_label(*oep.first) == "a");
    ++oep.first; TEST_ASSERT(rep.edge_label(*oep.first) == "b");
    ++oep.first; TEST_ASSERT(oep.first == oep.second);
    
    // Vertex #2
    ++vp.first; TEST_ASSERT(vp.first != vp.second);
    oep = rep.out_edges(*vp.first);
    TEST_ASSERT(oep.first != oep.second);
    TEST_ASSERT(rep.edge_label(*oep.first) == "b");
    ++oep.first; TEST_ASSERT(oep.first == oep.second);

    ++vp.first; TEST_ASSERT(vp.first == vp.second);
}

void test__target(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_buechi_transformed, graph);
    
    Graphviz_Representation rep(graph);    
    Graphviz_Representation::Vertex_Iterator_Pair_t vp(rep.vertex_pair());
    Graphviz_Representation::Out_Edge_Iterator_Pair_t oep;


    // Init vertex (#0)
    TEST_ASSERT(vp.first != vp.second);
    oep = rep.out_edges(*vp.first);
    TEST_ASSERT(oep.first != oep.second);
    TEST_ASSERT(rep.vertex_label(rep.target(*oep.first)) == "({(a U Gb)},\\n {(a U Gb)})");
    ++oep.first; TEST_ASSERT(oep.first == oep.second);
    
    // Vertex #1
    ++vp.first; TEST_ASSERT(vp.first != vp.second);
    oep = rep.out_edges(*vp.first);
    TEST_ASSERT(oep.first != oep.second);
    TEST_ASSERT(rep.vertex_label(rep.target(*oep.first)) == "({(a U Gb)},\\n {(a U Gb)})");
    ++oep.first; TEST_ASSERT(rep.vertex_label(rep.target(*oep.first)) == "({Gb}, ø)");
    
    // Vertex #2
    ++vp.first; TEST_ASSERT(vp.first != vp.second);
    oep = rep.out_edges(*vp.first);
    TEST_ASSERT(oep.first != oep.second);
    TEST_ASSERT(rep.vertex_label(rep.target(*oep.first)) == "({Gb}, ø)");
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
    
    Graphviz_Representation rep(graph);    

    TEST_EXCEPTIONLESS_BLOCK_ENTER;
    TEST_ASSERT(rep.vertex_descriptor("") == 0);
    TEST_ASSERT(rep.vertex_descriptor("({(a U Gb)},\\n {(a U Gb)})") == 1);
    TEST_ASSERT(rep.vertex_descriptor("({Gb}, ø)") == 2);
    TEST_EXCEPTIONLESS_BLOCK_EXIT;

    TEST_THROWING_BLOCK_ENTER;
    rep.vertex_descriptor("Not_Existing_Vertex_Name");
    TEST_THROWING_BLOCK_EXIT(Exception);
}


TEST_COMPONENT_TEST_NAMESPACE_END;
TEST_NAMESPACE_END;
LTL2FSM__END__NAMESPACE__LTL2FSM;


TEST_SUITE_BEGIN;
TEST_NORMAL_CASE(&test__Graphviz_Constructor, LEVEL_PROD);
TEST_NORMAL_CASE(&test__empty_Graphviz_Constructor, LEVEL_PROD);
TEST_NORMAL_CASE(&test__Graphviz_Copy_Constructor, LEVEL_PROD);
TEST_NORMAL_CASE(&test__vertex_pair, LEVEL_PROD);
TEST_NORMAL_CASE(&test__edge_pair, LEVEL_PROD);
TEST_NORMAL_CASE(&test__accepting, LEVEL_PROD);
TEST_NORMAL_CASE(&test__bottom, LEVEL_PROD);
TEST_NORMAL_CASE(&test__top, LEVEL_PROD);
TEST_NORMAL_CASE(&test__inconclusive, LEVEL_PROD);
TEST_NORMAL_CASE(&test__vertex_label, LEVEL_PROD);
TEST_NORMAL_CASE(&test__edge_label, LEVEL_PROD);
TEST_NORMAL_CASE(&test__strong_components, LEVEL_PROD);
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
