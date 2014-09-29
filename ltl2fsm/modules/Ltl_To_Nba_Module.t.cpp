/**
 * @file ltl2fsm/modules/Ltl_To_Nba_Module.t.cpp
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

#include <ltl2fsm/modules/Ltl_To_Nba_Module.hpp>

#include <boost/graph/graphviz.hpp>
#include <fstream>
#include <sstream>


#define TEST_COMPONENT_NAME Ltl_To_Nba_Module
#define TEST_COMPONENT_NAMESPACE ltl2fsm


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;
TEST_NAMESPACE_BEGIN;
TEST_COMPONENT_TEST_NAMESPACE_BEGIN;
using namespace ::diagnostics::unittest;


::std::string const & _valid_ltl_formula = "(a U G b)";
::std::string const & _invalid_ltl_formula = "((a U G b)";

/**
 * @brief A link to an empty file.
 */ 
::std::string const & _empty_file = "test/input/empty_file";

/**
 * @brief A link to a dot-file as generated by ltl2nba from the ltl
 * formula @a valid_ltl_formula -- without any modifications.
 */ 
::std::string const & _buechi_orig = "test/input/buechi_orig.dot";


void test__convert__invalid_ltl_syntax(Test_Data & test_data)
{
    Ltl_To_Nba_Module nba_module("tools/ltlnba/", "/tmp/");

    TEST_THROWING_BLOCK_ENTER;
    nba_module.execute(_invalid_ltl_formula);
    TEST_THROWING_BLOCK_EXIT(Exception);

    TEST_EXCEPTIONLESS_BLOCK_ENTER;
    nba_module.execute(_valid_ltl_formula);
    TEST_EXCEPTIONLESS_BLOCK_EXIT;
}


void test__empty_file(Test_Data & test_data)
{
    Ltl_To_Nba_Module nba_module("ltlnba/", "/tmp/");
    
    TEST_ASSERT(nba_module.empty_file(_empty_file));
    TEST_ASSERT(!nba_module.empty_file(_buechi_orig));
}

void test__add_peripheries(Test_Data & test_data)
{
    Ltl_To_Nba_Module nba_module("tools/ltlnba/", 
				 "test/input/");

    nba_module.add_peripheries(_buechi_orig);
    ::std::string temporary_path(nba_module.tmp_path() + nba_module.tmp_file());
    
   ::std::ifstream file(temporary_path.c_str());
    file.seekg(0);
    ::std::string full_line;
    ::std::string::size_type length;
    char line[256];
    bool correct = false;
        
    while (file.getline(line, 256)){
	full_line = line;
	length = full_line.find("shape=");
	if(length != ::std::string::npos){
	    length = full_line.find("peripheries");
	    TEST_ASSERT(length != ::std::string::npos);
	    correct = true;
	}
    }

    TEST_ASSERT(correct);
}

void test__add_epsilon_labels(Test_Data & test_data)
{
    Ltl_To_Nba_Module nba_module("tools/ltlnba/", 
				 "test/input/");
    
    nba_module.add_epsilon_labels(_buechi_orig);
    ::std::string temporary_path(nba_module.tmp_path() + nba_module.tmp_file());
    
    ::std::ifstream file(temporary_path.c_str());
    file.seekg(0);
    ::std::string full_line;
    ::std::string::size_type length;
    char line[256];
    bool correct = false;

    while (file.getline(line, 256)){
	full_line = line;
	length = full_line.find("init ->");
	if(length != ::std::string::npos){
	    length = full_line.find("[label=\"epsilon\"];");
	    TEST_ASSERT(length != ::std::string::npos);
	    correct = true;
	}
    }
    
    TEST_ASSERT(correct);
}


// void test_bla(Test_Data & test_data)
// {
// 	TEST_ASSERT(4==6);
// 	int i(4);
// 	int j(6);
// 	TEST_ASSERT_RELATION(i,==,j);

// 	TEST_EXCEPTIONLESS_BLOCK_ENTER;
// 	// throw "we are a funny bunch of people";
// 	TEST_EXCEPTIONLESS_BLOCK_EXIT;

// 	TEST_THROWING_BLOCK_ENTER;
// 	//throw Test_Exception("hhh");
// 	// throw "hhh";
// 	TEST_THROWING_BLOCK_EXIT(Test_Exception);


// 	TEST_ASSERT(test_data.compare("1","plok"));
// 	TEST_ASSERT(test_data.compare("2","plok2\nblub\nblub"));
// }


TEST_COMPONENT_TEST_NAMESPACE_END;
TEST_NAMESPACE_END;
LTL2FSM__END__NAMESPACE__LTL2FSM;


TEST_SUITE_BEGIN;
TEST_NORMAL_CASE(&test__convert__invalid_ltl_syntax, LEVEL_PROD);
TEST_NORMAL_CASE(&test__empty_file, LEVEL_PROD);
TEST_NORMAL_CASE(&test__add_peripheries, LEVEL_PROD);
TEST_NORMAL_CASE(&test__add_epsilon_labels, LEVEL_PROD);
TEST_SUITE_END;


STREAM_TEST_SYSTEM_MAIN;