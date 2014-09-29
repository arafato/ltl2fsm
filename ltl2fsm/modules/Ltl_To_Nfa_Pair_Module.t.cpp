/**
 * @file ltl2fsm/modules/Ltl_To_Nfa_Pair_Module.t.cpp
 *
 * $Id$
 *
 * @author Oliver Arafat
 *
 * @brief [LEVEL: alpha] testing Ltl_To_Nfa_Pair_Module.impl.hpp
 *
 * @note DOCUMENTED
 *
 * @test
 *
 * @todo
 */


#include <diagnostics/unittest.hpp>

#include <ltl2fsm/modules/Ltl_To_Nfa_Pair_Module.impl.hpp>
#include <ltl2fsm/automaton/Nondet_Finite_Automaton.impl.hpp>
#include <ltl2fsm/representation/Graphviz_Representation.hpp>

#include <sstream>


#define TEST_COMPONENT_NAME Ltl_To_Nfa_Pair_Module
#define TEST_COMPONENT_NAMESPACE ltl2fsm


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;
TEST_NAMESPACE_BEGIN;
TEST_COMPONENT_TEST_NAMESPACE_BEGIN;
using namespace ::diagnostics::unittest;


typedef Nondet_Finite_Automaton<Graphviz_Representation> NFA_t;
typedef Ltl_To_Nfa_Pair_Module<Graphviz_Representation> Pair_Module_t;


void test__execute(Test_Data & test_data)
{
    Pair_Module_t pair_module;
    Pair_Module_t::Nfa_Pair_t nfa_pair = pair_module.execute("a U G b");

    ////////////////////////////////
    // Testing nfa_pair.first (p) //
    ////////////////////////////////
    {
	NFA_t nfa = *nfa_pair.first;
	NFA_t::Vertex_Iterator_Pair_t vp = nfa.vertex_pair();
	NFA_t::Out_Edge_Iterator_Pair_t oep;

	::std::ostringstream reference_stream, output_stream;

	// Building reference stream (the expected output)
	reference_stream << "---------" << ::std::endl
			 << " <NOT ACCEPTING>" << ::std::endl
			 << "epsilon" << ::std::endl
			 << "({(a U Gb)},\\n {(a U Gb)})" << ::std::endl
			 << "---------" << ::std::endl
			 << "({(a U Gb)},\\n {(a U Gb)}) <NOT ACCEPTING>" << ::std::endl
			 << "a" << ::std::endl
			 << "({(a U Gb)},\\n {(a U Gb)})" << ::std::endl
			 << "b" << ::std::endl
			 << "({Gb}, ø)" << ::std::endl
			 << "---------" << ::std::endl
			 << "({Gb}, ø) <ACCEPTING>" << ::std::endl
			 << "b" << ::std::endl
			 << "({Gb}, ø)" << ::std::endl;
	
	// Building output stream (observed output)               
	for(; vp.first != vp.second; ++vp.first){
	    output_stream << "---------" << ::std::endl;
	    output_stream << nfa.vertex_label(*vp.first);
	    oep = nfa.out_edges(*vp.first);
	    if(nfa.accepting(*vp.first))
		output_stream << " <ACCEPTING>" << ::std::endl;
	    else
		output_stream << " <NOT ACCEPTING>" << ::std::endl;

	    for(; oep.first != oep.second; ++oep.first){
		output_stream << nfa.edge_label(*oep.first) << ::std::endl;
		output_stream << nfa.vertex_label(nfa.target(*oep.first)) << ::std::endl;
	    }
	}  
	
	TEST_ASSERT(reference_stream.str() == output_stream.str());
    }


    //////////////////////////////////
    // Testing nfa_pair.second (!p) //
    //////////////////////////////////
    {
	NFA_t nfa = *nfa_pair.second;
	NFA_t::Vertex_Iterator_Pair_t vp = nfa.vertex_pair();
	NFA_t::Out_Edge_Iterator_Pair_t oep;

	::std::ostringstream reference_stream, output_stream;

	// Building reference stream (the expected output)
	reference_stream << "---------" << ::std::endl
			 << " <NOT ACCEPTING>" << ::std::endl
			 << "epsilon" << ::std::endl
			 << "({(!a V F!b)}, ø)" << ::std::endl
			 << "---------" << ::std::endl
			 << "({F!b}, {F!b}) <NOT ACCEPTING>" << ::std::endl
			 << "tt" << ::std::endl
			 << "({F!b}, {F!b})" << ::std::endl
			 << "!b" << ::std::endl
			 << "({tt}, ø)" << ::std::endl
			 << "---------" << ::std::endl
			 << "({(!a V F!b)}, ø) <ACCEPTING>" << ::std::endl
			 << "!a" << ::std::endl
			 << "({F!b}, {F!b})" << ::std::endl
			 << "!b" << ::std::endl
			 << "({(!a V F!b)}, ø)" << ::std::endl
			 << "!a & !b" << ::std::endl
			 << "({tt}, ø)" << ::std::endl
			 << "tt" << ::std::endl
			 << "({F!b, (!a V F!b)},\\n {F!b})" << ::std::endl
			 << "---------" << ::std::endl
			 << "({tt}, ø) <ACCEPTING>" << ::std::endl
			 << "tt" << ::std::endl
			 << "({tt}, ø)" << ::std::endl
			 << "---------" << ::std::endl
			 << "({F!b, (!a V F!b)},\\n {F!b}) <ACCEPTING>" << ::std::endl
			 << "!a" << ::std::endl
			 << "({F!b}, {F!b})" << ::std::endl
			 << "!b" << ::std::endl
			 << "({(!a V F!b)}, ø)" << ::std::endl
			 << "!a & !b" << ::std::endl
			 << "({tt}, ø)" << ::std::endl
			 << "tt" << ::std::endl
			 << "({F!b, (!a V F!b)},\\n {F!b})" << ::std::endl;
		
	// Building output stream (observed output)               
	for(; vp.first != vp.second; ++vp.first){
	    output_stream << "---------" << ::std::endl;
	    output_stream << nfa.vertex_label(*vp.first);
	    oep = nfa.out_edges(*vp.first);
	    if(nfa.accepting(*vp.first))
		output_stream << " <ACCEPTING>" << ::std::endl;
	    else
		output_stream << " <NOT ACCEPTING>" << ::std::endl;

	    for(; oep.first != oep.second; ++oep.first){
		output_stream << nfa.edge_label(*oep.first) << ::std::endl;
		output_stream << nfa.vertex_label(nfa.target(*oep.first)) << ::std::endl;
	    }
	}  
	
	TEST_ASSERT(reference_stream.str() == output_stream.str());
    }
}


TEST_COMPONENT_TEST_NAMESPACE_END;
TEST_NAMESPACE_END;
LTL2FSM__END__NAMESPACE__LTL2FSM;


TEST_SUITE_BEGIN;
TEST_NORMAL_CASE(&test__execute, LEVEL_PROD);
TEST_SUITE_END;


STREAM_TEST_SYSTEM_MAIN;
