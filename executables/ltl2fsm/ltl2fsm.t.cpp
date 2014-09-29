/**
 * @file executables/ltl2fsm/ltl2fsm.t.cpp
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


#include <ltl2fsm/modules/Nfa_To_Cpp_Module.impl.hpp>
#include <ltl2fsm/modules/Ltl_To_Nfa_Pair_Module.impl.hpp>
#include <ltl2fsm/representation/Graphviz_Representation.hpp>
#include <ltl2fsm/automaton/Nondet_Finite_Automaton.impl.hpp>
#include <ltl2fsm/base/exception/Fork_Error.hpp>
#include <ltl2fsm/base/exception/Invalid_Ltl_Syntax.hpp>

#include <iostream>
#include <ostream>
#include <fstream>

#include <diagnostics/configuration.hpp>
#include <diagnostics/logger/file_logger.hpp>
#include <diagnostics/logger/intending_file_logger.hpp>


DIAGNOSTICS_NAMESPACE_BEGIN;
void set_initial_loggers(::std::vector<Logger *> & loggers)
{
	//loggers.push_back(new File_Logger("test/exec.log"));
	loggers.push_back(new Intending_File_Logger("test/exec.log"));
}
DIAGNOSTICS_NAMESPACE_END;


void usage()
{
    ::std::cout << "LTL2FSM - a tool for automated monitor code generation from LTL formulae" << ::std::endl;
    ::std::cout << ::std::endl;
    ::std::cout << "usage: ltl2fsm [-option] ... [-option] -f <'formula'>" << ::std::endl;
    ::std::cout << " -r <'root/dir'>  root-directory for cpp output - default is stdout" << ::std::endl;
    ::std::cout << " -n <'namespace'> namespace of implementation - default is none" << ::std::endl;
    ::std::cout << " -p <'include prefix'> include prefix - default is none" << ::std::endl;
    ::std::cout << " -i <'true'|'false'> include paths are enclosed by <> (true) or \"\" (false)" << ::std::endl;
    ::std::cout << "                      - default is false" << ::std::endl;
    ::std::cout << " -c <'classname'> classname/filename of generated monitorcode," << ::std::endl;
    ::std::cout << "                  always prefixed by 'Pos_' respectively 'Neg_'." << ::std::endl;
    ::std::cout << "                  - default is 'Pos_Nfa' respectively 'Neg_Nfa'" << ::std::endl;
}

int main(int argc, char ** argv)
{
    ::diagnostics::Logging_Config::init();
    
    if(argc < 2){
	usage();
	return 1;
    }

    ::std::string root_dir("");
    ::std::string _namespace("");
    ::std::string include_prefix("");
    ::std::string formula = ("");
    bool include_type = false;
    ::std::string bool_str("false");
    ::std::string pos_nfa("Pos_Nfa");
    ::std::string neg_nfa("Neg_Nfa");
    ::std::string classname("");
            
    while(argc > 1 && argv[1][0] == '-'){
	switch(argv[1][1]){
        case 'r': 
            root_dir = argv[2];
            --argc;
            ++argv;
            break;
        case 'n':
            _namespace = argv[2];
            --argc;
            ++argv;		
            break;
        case 'p':
            include_prefix = argv[2];
            --argc;
            ++argv;		
            break;
        case 'i':
            bool_str = argv[2];
            --argc;
            ++argv;		
            break;
        case 'c':
            classname = argv[2];
            pos_nfa = "";
            neg_nfa = "";
            pos_nfa = "Pos_" + classname;
            neg_nfa = "Neg_" + classname;
            --argc;
            ++argv;
            break;
        case 'f':
            formula = argv[2];
            --argc;
            ++argv;		
            break;
        default: usage();
            break;
	}
	--argc;
	++argv;
    }

    if(formula == ""){
	::std::cout << "You must specify a formula! Exiting..." << ::std::endl;
	return 1;
    }

    if(bool_str == "true")
	include_type = true;
        
    typedef ::ltl2fsm::Graphviz_Representation Representation_t;
    typedef ::ltl2fsm::Ltl_To_Nfa_Pair_Module<Representation_t> Ltl_To_Nfa_Pair_Module_t;
    typedef ::ltl2fsm::Nfa_To_Cpp_Module<Representation_t> Nfa_To_Cpp_Module_t;
    
    Ltl_To_Nfa_Pair_Module_t ltl_to_nfa_pair_module;
    Ltl_To_Nfa_Pair_Module_t::Nfa_Pair_t nfa_pair;
    Nfa_To_Cpp_Module_t nfa_to_cpp_module;
    
    try{

	nfa_pair = ltl_to_nfa_pair_module.execute(formula);
    }
    catch( ::ltl2fsm::Invalid_Ltl_Syntax const & ils){
	::std::cout << ils.what() << ::std::endl;
	return 1;
    }
    catch( ::ltl2fsm::Fork_Error const & fe ){
	::std::cout << fe.what() << ::std::endl;
	return 1;
    }

    if(root_dir != ""){
	::std::string root_dir_pos_nfa = root_dir + pos_nfa;
	::std::ofstream pos_nfa_cpp((root_dir_pos_nfa + ".cpp").c_str());
        ::std::ofstream pos_nfa_hpp((root_dir_pos_nfa + ".hpp").c_str());
	nfa_to_cpp_module.execute(*nfa_pair.first, 
				  pos_nfa_cpp,
                                  pos_nfa_hpp,
                                  pos_nfa,
                                  _namespace,
				  include_prefix,
                                  formula,
				  include_type);
	
	::std::string root_dir_neg_nfa = root_dir + neg_nfa;
	::std::ofstream neg_nfa_cpp((root_dir_neg_nfa + ".cpp").c_str());
	::std::ofstream neg_nfa_hpp((root_dir_neg_nfa + ".hpp").c_str());
	nfa_to_cpp_module.execute(*nfa_pair.second, 
				  neg_nfa_cpp,
                                  neg_nfa_hpp,
                                  neg_nfa,
                                  _namespace,
				  include_prefix,
                                  ::std::string("!(" + formula + ")"),
				  include_type);
    }
    else{
	nfa_to_cpp_module.execute(*nfa_pair.first, 
				  ::std::cout,
                                  ::std::cout,
				  pos_nfa,
				  _namespace,
				  include_prefix,
                                  formula,
				  include_type);
	
	nfa_to_cpp_module.execute(*nfa_pair.second, 
				  ::std::cout,
                                  ::std::cout,
				  neg_nfa,
				  _namespace,
				  include_prefix,
                                  ::std::string("!(" + formula + ")"),
				  include_type);
    }
            
    return 0;
}

