/**
 * @file ltl2fsm/module/Nfa_To_Cpp_Module.t.cpp
 *
 * $Id$
 *
 * @author Oliver Arafat
 *
 * @brief [LEVEL: alpha] testing Nfa_To_Cpp_Module.impl.hpp
 *
 * @note DOCUMENTED
 *
 * @test
 *
 * @todo
 */


#include <diagnostics/unittest.hpp>

#include <ltl2fsm/modules/Nfa_To_Cpp_Module.impl.hpp>
#include <ltl2fsm/representation/Graphviz_Representation.hpp>
#include <ltl2fsm/modules/Nba_To_Nfa_Module.impl.hpp>
#include <ltl2fsm/modules/Ltl_To_Nfa_Pair_Module.impl.hpp>

#include <sstream>


#define TEST_COMPONENT_NAME Nfa_To_Cpp_Module
#define TEST_COMPONENT_NAMESPACE ltl2fsm


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;
TEST_NAMESPACE_BEGIN;
TEST_COMPONENT_TEST_NAMESPACE_BEGIN;
using namespace ::diagnostics::unittest;


typedef Graphviz_Representation Representation_t;

typedef Nondet_Finite_Automaton<Representation_t> NFA_t;
typedef Nondet_Buechi_Automaton<Representation_t> NBA_t;

typedef Nfa_To_Cpp_Module<Representation_t> Cpp_Module_t;
typedef Nba_To_Nfa_Module<Representation_t> Nba_To_Nfa_Module_t;
typedef Ltl_To_Nfa_Pair_Module<Representation_t> Ltl_To_Nfa_Pair_Module_t;
typedef ::ltl2fsm::internal::Cpp_Module_Assistant<Representation_t> Cpp_Assistant_t;


/**
 * @brief Testinput for this testsuite. This dot file is a transformed
 * version of buechi_orig.dot.  It is extended with those labels that
 * are added in Ltl_To_Nba_Module, such as peripheries and epsilon
 * labels.
 */ 
::std::string const & _buechi_transformed = "test/input/buechi_transformed.dot";

::std::string const & _nfa3 = "test/input/nfa_3.dot";


void test__assistant_constructor(Test_Data & test_data)
{
    // Just to make it compile!
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_buechi_transformed, graph);
    
    NFA_t nfa(new Representation_t(graph));

    Cpp_Assistant_t assistant(nfa);
}

void test__p_init_proposition_map(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_buechi_transformed, graph);
    
    NFA_t nfa(new Representation_t(graph));

    // p_init_proposition_map is called in constructor
    Cpp_Assistant_t assistant(nfa);

    Cpp_Assistant_t::Proposition_Map_t const & prop_map = assistant.proposition_map();
    Cpp_Assistant_t::Proposition_Map_t::const_iterator it = prop_map.begin();
    Cpp_Assistant_t::Proposition_Map_t::const_iterator const end = prop_map.end();
    
    ::std::ostringstream reference_stream, output_stream;

    reference_stream << "a : 0" << ::std::endl
    		     << "b : 1" << ::std::endl;
        
    for(; it != end; ++it)
	output_stream << it->first << " : " << it->second << ::std::endl;

    TEST_ASSERT(reference_stream.str() == output_stream.str());
}

void test__p_init_proposition_map2(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_nfa3, graph);
    
    NFA_t nfa(new Representation_t(graph));

    // p_init_proposition_map is called in constructor
    Cpp_Assistant_t assistant(nfa);

    Cpp_Assistant_t::Proposition_Map_t const & prop_map = assistant.proposition_map();
    Cpp_Assistant_t::Proposition_Map_t::const_iterator it = prop_map.begin();
    Cpp_Assistant_t::Proposition_Map_t::const_iterator const end = prop_map.end();
    
    ::std::ostringstream reference_stream, output_stream;

    reference_stream << "a : 0" << ::std::endl
    		     << "b : 1" << ::std::endl;
        
    for(; it != end; ++it)
	output_stream << it->first << " : " << it->second << ::std::endl;

    TEST_ASSERT(reference_stream.str() == output_stream.str());
}

void test__gen_code__initial_states(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_nfa3, graph);
    
    NBA_t nba(new Representation_t(graph));
    Nba_To_Nfa_Module_t nba2nfa;
    NFA_t * nfa(nba2nfa.execute(nba));
    
    Cpp_Assistant_t assistant(*nfa);
    TEST_ASSERT(assistant.gen_code__initial_states() == "initial_states().insert(2);\n");
}

void test__gen_code__accepting_set(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_nfa3, graph);
    
    NBA_t nba(new Representation_t(graph));
    Nba_To_Nfa_Module_t nba2nfa;
    NFA_t * nfa(nba2nfa.execute(nba));
    
    Cpp_Assistant_t assistant(*nfa);
    ::std::ostringstream reference_output;
    reference_output << "accepting_set().push_back(1);" << ::std::endl
		     << "accepting_set().push_back(2);" << ::std::endl
		     << "accepting_set().push_back(3);" << ::std::endl
		     << "accepting_set().push_back(4);" << ::std::endl;

    
    TEST_ASSERT(assistant.gen_code__accepting_set() == reference_output.str());
}

void test__label_to_condition(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_nfa3, graph);
    
    NBA_t nba(new Representation_t(graph));
    Nba_To_Nfa_Module_t nba2nfa;
    NFA_t * nfa(nba2nfa.execute(nba));
    
    Cpp_Assistant_t assistant(*nfa);

    TEST_ASSERT("!bitvector[0] && !bitvector[1]" == assistant.label_to_condition("!a & !b"));
}

void test__gen_code__switch_case(Test_Data & test_data)
{
    ::boost::GraphvizDigraph graph;
    ::boost::read_graphviz(_nfa3, graph);
    
    NBA_t nba(new Representation_t(graph));
    Nba_To_Nfa_Module_t nba2nfa;
    NFA_t * nfa(nba2nfa.execute(nba));

    Cpp_Assistant_t assistant(*nfa);
    ::std::ostringstream reference_stream;
    
    reference_stream << "switch(*state){" << ::std::endl
		     << "case 4:" << ::std::endl
		     << "if(!bitvector[0])" << ::std::endl
		     << "target.insert(1);" << ::std::endl
		     << "if(!bitvector[1])" << ::std::endl
		     << "target.insert(2);" << ::std::endl
		     << "if(!bitvector[0] && !bitvector[1])" << ::std::endl
		     << "target.insert(3);" << ::std::endl
		     << "if(true)" << ::std::endl
		     << "target.insert(4);" << ::std::endl
		     << "break;" << ::std::endl
		     << "case 3:" << ::std::endl
		     << "if(true)" << ::std::endl
		     << "target.insert(3);" << ::std::endl
		     << "break;" << ::std::endl
		     << "case 2:" << ::std::endl
		     << "if(!bitvector[0])" << ::std::endl
		     << "target.insert(1);" << ::std::endl
		     << "if(!bitvector[1])" << ::std::endl
		     << "target.insert(2);" << ::std::endl
		     << "if(!bitvector[0] && !bitvector[1])" << ::std::endl
		     << "target.insert(3);" << ::std::endl
		     << "if(true)" << ::std::endl
		     << "target.insert(4);" << ::std::endl
		     << "break;" << ::std::endl
		     << "case 1:" << ::std::endl
		     << "if(true)" << ::std::endl
		     << "target.insert(1);" << ::std::endl
		     << "if(!bitvector[1])" << ::std::endl
		     << "target.insert(3);" << ::std::endl
		     << "break;" << ::std::endl
		     << "}" << ::std::endl;
    
    TEST_ASSERT(assistant.gen_code__switch_case() == reference_stream.str());
}


//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////


void test__execute(Test_Data & test_data)
{
    Ltl_To_Nfa_Pair_Module_t pair_module;
    Ltl_To_Nfa_Pair_Module_t::Nfa_Pair_t nfa_pair;
    ::std::string formula("!a | F !b");
    
    nfa_pair = pair_module.execute(formula);
    
    Cpp_Module_t cpp_module;
    cpp_module.execute(*nfa_pair.first, 
                       ::std::cout, 
                       ::std::cout,
		       ::std::string("Pos_Nfa"),
		       "",
		       "",
                       formula,
		       false);
    
    cpp_module.execute(*nfa_pair.second, 
                       ::std::cout, 
                       ::std::cout,
                       ::std::string("Neg_Nfa"),
                       "",
		       "",
                       ::std::string("!(" + formula + ")"),
		       false);
}

TEST_COMPONENT_TEST_NAMESPACE_END;
TEST_NAMESPACE_END;
LTL2FSM__END__NAMESPACE__LTL2FSM;


TEST_SUITE_BEGIN;
TEST_NORMAL_CASE(&test__assistant_constructor, LEVEL_PROD);
TEST_NORMAL_CASE(&test__p_init_proposition_map, LEVEL_PROD);
TEST_NORMAL_CASE(&test__p_init_proposition_map2, LEVEL_PROD);
TEST_NORMAL_CASE(&test__gen_code__initial_states, LEVEL_PROD);
TEST_NORMAL_CASE(&test__gen_code__accepting_set, LEVEL_PROD);
TEST_NORMAL_CASE(&test__label_to_condition, LEVEL_PROD);
TEST_NORMAL_CASE(&test__gen_code__switch_case, LEVEL_PROD);
//////////////////////////////////////////////////
TEST_NORMAL_CASE(&test__execute, LEVEL_PROD);
TEST_SUITE_END;


STREAM_TEST_SYSTEM_MAIN;
