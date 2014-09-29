/**
 * @file ltl2fsm/module/Nba_To_Nfa_Module.t.cpp
 *
 * $Id$
 *
 * @author Oliver Arafat
 *
 * @brief [LEVEL: alpha] testing Nba_To_Nfa_Module.impl.hpp
 *
 * @note DOCUMENTED
 *
 * @test
 *
 * @todo
 */


#include <diagnostics/unittest.hpp>

#include <ltl2fsm/modules/Nba_To_Nfa_Module.impl.hpp>
#include <ltl2fsm/automaton/Nondet_Buechi_Automaton.impl.hpp>
#include <ltl2fsm/automaton/Nondet_Finite_Automaton.impl.hpp>
#include <ltl2fsm/representation/Graphviz_Representation.hpp>

#include <sstream>


#define TEST_COMPONENT_NAME Nba_To_Nfa_Module
#define TEST_COMPONENT_NAMESPACE ltl2fsm


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;
TEST_NAMESPACE_BEGIN;
TEST_COMPONENT_TEST_NAMESPACE_BEGIN;
using namespace ::diagnostics::unittest;


typedef Nondet_Buechi_Automaton<Graphviz_Representation> NBA_t;
typedef Nondet_Finite_Automaton<Graphviz_Representation> NFA_t;

/**
 * @brief Testinput for this testsuite. This dot file is a transformed
 * version of buechi_orig.dot.  It is extended with those labels that
 * are added in Ltl_To_Nba_Module, such as peripheries and epsilon
 * labels.
 */ 
::std::string const & _buechi_transformed = "test/input/buechi_transformed.dot";

/**
 * @brief Another testinput (extended). It is a NBA constructed "by
 * hand" with 5 vertices and 3 SCC's.
 */ 
::std::string const & _buechi2_transformed = "test/input/buechi2_transformed.dot";


/**
 * @brief The third vertex and second vertex are supposed to be
 * accepting, the first one is supposed to be non-accepting (init).
 *
 * As there are three vertices and three SCC's the automaton is
 * supposed to stay the same.
 */ 
void test__execute_1(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_buechi_transformed, graph);
    NBA_t * nba = new NBA_t(new Graphviz_Representation(graph));
    Nba_To_Nfa_Module<Graphviz_Representation> nfa_constr_module;
    NFA_t * nfa = nfa_constr_module.execute(*nba);
    delete nba;

    NFA_t::Vertex_Iterator_Pair_t vp = nfa->vertex_pair();
    TEST_ASSERT(vp.first != vp.second);
    TEST_ASSERT(!nfa->accepting(*vp.first));
    
    ++vp.first; TEST_ASSERT(vp.first != vp.second);
    TEST_ASSERT(nfa->accepting(*vp.first));
    
    ++vp.first; TEST_ASSERT(vp.first != vp.second);
    TEST_ASSERT(nfa->accepting(*vp.first));

    ++vp.first; TEST_ASSERT(vp.first == vp.second);


    delete nfa;
}

/**
 * @brief Vertex        Expected value:     Original value:
 *      0 (init)        non-accepting       non-accepting
 *      1               accepting           non-accepting
 *      2               accepting           non-accepting
 *      3               non-accepting       non-accepting
 *      4               accepting           accepting
 */ 
void test__execute_2(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_buechi2_transformed, graph);
    NBA_t * nba = new NBA_t(new Graphviz_Representation(graph));
    Nba_To_Nfa_Module<Graphviz_Representation> nfa_constr_module;
    NFA_t * nfa = nfa_constr_module.execute(*nba);
    delete nba;

    NFA_t::Vertex_Iterator_Pair_t vp = nfa->vertex_pair();

    TEST_ASSERT(vp.first != vp.second);
    TEST_ASSERT(!nfa->accepting(*vp.first));
    
    ++vp.first; TEST_ASSERT(vp.first != vp.second);
    TEST_ASSERT(nfa->accepting(*vp.first));
    
    ++vp.first; TEST_ASSERT(vp.first != vp.second);
    TEST_ASSERT(nfa->accepting(*vp.first));

    ++vp.first; TEST_ASSERT(vp.first != vp.second);
    TEST_ASSERT(!nfa->accepting(*vp.first));

    ++vp.first; TEST_ASSERT(vp.first != vp.second);
    TEST_ASSERT(nfa->accepting(*vp.first));

    ++vp.first; TEST_ASSERT(vp.first == vp.second);


    delete nfa;
}


TEST_COMPONENT_TEST_NAMESPACE_END;
TEST_NAMESPACE_END;
LTL2FSM__END__NAMESPACE__LTL2FSM;


TEST_SUITE_BEGIN;
TEST_NORMAL_CASE(&test__execute_1, LEVEL_PROD);
TEST_NORMAL_CASE(&test__execute_2, LEVEL_PROD);
TEST_SUITE_END;


STREAM_TEST_SYSTEM_MAIN;
