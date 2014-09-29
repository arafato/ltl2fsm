/**
 * @file ltl2fsm/modules/Nfa_To_Cpp_Module.impl.hpp
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


#ifndef LTL2FSM__MODULES__NFA_TO_CPP_MODULE_IMPL__HPP
#define LTL2FSM__MODULES__NFA_TO_CPP_MODULE_IMPL__HPP


#include <ltl2fsm/modules/Nfa_To_Cpp_Module.hpp>
#include <ltl2fsm/automaton/Nondet_Finite_Automaton.impl.hpp>

#include <map>
#include <vector>
#include <stack>
#include <string>
#include <algorithm>
#include <sstream>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;
LTL2FSM__BEGIN__NAMESPACE__INTERNAL;


/**
 * @class Cpp_Module_Assistant
 *
 * @brief This class offers all neccessary components,
 * i.e. datastructures and convenience methods, to @ref
 * Nfa_To_Cpp_Module to generate cpp-code from a NBA.
 *
 * @nosubgrouping
 */
template<typename REP>
class Cpp_Module_Assistant
{
    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Types
     * @{
     */
private:
    typedef REP Representation_t;
    typedef Cpp_Module_Assistant Self;

    typedef typename Representation_t::Vertex_Descriptor_t Vertex_Descriptor_t;
    typedef typename Representation_t::Edge_Descriptor_t Edge_Descriptor_t;
    typedef typename Representation_t::Edge_Iterator_Pair_t Edge_Iterator_Pair_t;
    typedef typename Representation_t::Vertex_Iterator_Pair_t Vertex_Iterator_Pair_t;
    typedef typename Representation_t::Out_Edge_Iterator_Pair_t Out_Edge_Iterator_Pair_t;

public:
    typedef Nondet_Finite_Automaton<Representation_t> NFA_t;
    /**
     * @brief Stores all propositions of some ltl-formula. This type
     * is only internally used by method p_init_proposition_map.
     */ 
    typedef ::std::vector< ::std::string> Propositions_t;

    /**
     * @brief Stores all propositions of some ltl-formula. The value
     * of a key alphabetically corresponds to the order of a
     * proposition that is stored in a bitvector which will serve as
     * input for the FSM.
     */ 
    typedef ::std::map< ::std::string, int> Proposition_Map_t;

    /**
     * @brief Dstate_t represents a DFA state. It consists of an
     * arbitrary number of NFA states.
     */ 
    typedef ::std::vector<Vertex_Descriptor_t> Dstate_t;
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Construction / Destruction
     * @{
     */
public:
    explicit Cpp_Module_Assistant(NFA_t & nfa);
    ~Cpp_Module_Assistant();
    
private:
    /**
     * @brief Initializes the field m_proposition_map.
     */ 
    void p_init_proposition_map();
    
    /**
     * @brief Serves as a helper method for p_init_proposition_map().
     */ 
    void p_init_proposition_map_help(::std::string const & formula,
				     Propositions_t & props);
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Code generation (Interface)
     * @{
     */
public:
    /**
     * @brief TODO
     */ 
    ::std::string gen_code__interface(::std::string const & classname,
                                      ::std::string const & ltl_formula,
                                      ::std::string const & _namespace);

    /**
     * @brief TODO
     */ 
    ::std::string gen_code__include_guard(::std::string const & classname,
                                          ::std::string const & _namespace);

    /**
     * @brief TODO
     */ 
    ::std::string gen_code__class_comments(::std::string const & classname);

    /**
     * @brief TODO
     */ 
    ::std::string gen_code__types_group(::std::string const & classname);

    /**
     * @brief TODO
     */ 
    ::std::string gen_code__construction_destruction_group(::std::string const & classname);

    /**
     * @brief TODO
     */ 
    ::std::string gen_code__input_processing_group();
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Code generation (Implementation)
     * @{
     */
public:
    /**
     * @brief Calls all code generation methods and builds the
     * complete implementation for @a classname.
     */ 
    ::std::string gen_code__impl(::std::string const & classname,
                                 ::std::string const & _namespace,
                                 ::std::string const & include_prefix,
                                 ::std::string const & ltl_formula,
                                 bool include_type);
    
    /**
     * @brief Generates the complete C++-code for method
     * 'step'. Method 'gen_code__switch_case' is called within this
     * method. Depending on @a nfa_type, the implementation will be
     * generated for either @ref Pos_Nfa (true) or @ref Neg_Nfa
     * (false).
     */ 
    ::std::string gen_code__step(::std::string const & classname);

    /**
     * @brief Generates the complete C++-Code for the constructor @ref
     * Pos_Nfa (true) and @ref Neg_Nfa (false), depending on the value
     * of @a nfa_type.
     */ 
    ::std::string gen_code__constructor(::std::string const & classname);
    
    /**
     * @brief Generates the complete C++-Code for the destructor @ref
     * Pos_Nfa (true) and @ref Neg_Nfa (false), depending on the value
     * of @a nfa_type.
     */ 
    ::std::string gen_code__destructor(::std::string const & classname);
    
    /**
     * @brief Generates the complete C++-code for method
     * 'init_initial_states'. Depending on @a nfa_type, the
     * implementation will be generated for either @ref Pos_Nfa (true)
     * or @ref Neg_Nfa (false).
     */ 
    ::std::string gen_code__init_initial_states(::std::string const & classname);
    
    /**
     * @brief Generates the complete C++-code for method
     * 'init_accepting_set'. Depending on @a nfa_type, the
     * implementation will be generated for either @ref Pos_Nfa (true)
     * or @ref Neg_Nfa (false).
     */ 
    ::std::string gen_code__init_accepting_set(::std::string const & classname);
    
    /**
     * @brief Generates C++-code for the body of method
     * 'init_initial_states' defined in class @ref Pos_Nfa and @ref
     * Neg_Nfa.
     */ 
    ::std::string gen_code__initial_states();

    /**
     * @brief Generates C++-code for the body of method
     * 'init_accepting_map' defined in class @ref Pos_Nfa and @ref
     * Neg_Nfa.
     */ 
    ::std::string gen_code__accepting_set();

    /**
     * @brief Generates C++-Code for the body of method 'step' defined
     * in class @ref Pos_Nfa and @ref Neg_Nfa.
     */ 
    ::std::string gen_code__switch_case();

    /**
     * @brief Generates C++-code for the include-directive. @a
     * include_prefix determines the the prefix of the include path,
     * @a include_type whether the path is enclosed by '<>' (true) or
     * ' "" ' (false).
     */ 
    ::std::string gen_code__include_directive(::std::string const & classname,
					      ::std::string const & include_prefix,
					      bool include_type);

    /**
     * @brief Generates a C++-comment block, that presents information
     * about the class/filename @classname, the ltl formula
     * @ltl_formula, author, license, etc...
     */ 
    ::std::string gen_code__commentblock(::std::string const & classname,
                                         ::std::string const & ltl_formula);
    
    /**
     * @brief Returns the corresponding if-condition of a transition
     * label.
     */ 
    ::std::string label_to_condition(::std::string const & label);
    
private:
    /**
     * @brief Serves as helper method for label_to_condition().
     */ 
    void label_to_condition_help(::std::string const & formula,
				 ::std::ostringstream & condition);
    
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Field access
     * @{
     */
public:
    /**
     * @brief Returns the field m_nfa.
     */ 
    NFA_t * const nfa();
    
    /**
     * @brief Returns the field m_proposition_map.
     */ 
    Proposition_Map_t const & proposition_map();
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name State
     * @{
     */
private:
    NFA_t * m_nfa;
    Proposition_Map_t m_proposition_map;
    // @}


#if LTL2FSM_DEBUG__LEVEL__ > 1
public:
    void m_class_invariance() const;
#endif
};

#if LTL2FSM_DEBUG__LEVEL__ > 1
template<typename REP>
void Cpp_Module_Assistant<REP>::m_class_invariance() const
{
}
#endif

template<typename REP>
Cpp_Module_Assistant<REP>::Cpp_Module_Assistant(NFA_t & nfa)
    : m_nfa(&nfa)
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    
    p_init_proposition_map();
    
    LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT;
}

template<typename REP>
Cpp_Module_Assistant<REP>::~Cpp_Module_Assistant()
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT;
}

template<typename REP>
typename Cpp_Module_Assistant<REP>::NFA_t * const Cpp_Module_Assistant<REP>::nfa()
{
    LTL2FSM_METHOD_GUARD("");
    return m_nfa;
}

template<typename REP>
typename Cpp_Module_Assistant<REP>::Proposition_Map_t const & Cpp_Module_Assistant<REP>::proposition_map()
{
    LTL2FSM_METHOD_GUARD("");
    return m_proposition_map;
}

template<typename REP>
void Cpp_Module_Assistant<REP>::p_init_proposition_map_help(::std::string const & formula,
							    Propositions_t & props)
{
    LTL2FSM_METHOD_GUARD("");
    ::std::string::size_type pos = 0;
    ::std::string::size_type pos_old = 0;
    ::std::string proposition;
    
    // Checking the left proposition of '&'
    while( (pos = formula.find("&", pos)) != ::std::string::npos){
	
	proposition = formula[pos-2];
	Propositions_t::const_iterator const it(::std::find(props.begin(),
							    props.end(),
							    proposition));
	if(it == props.end())
	    props.push_back(proposition);
		
	++pos;
	pos_old = pos;
	++pos_old;
    }
    // Checking the right proposition of '&'
    if(formula[pos_old] == '!')
	proposition = formula[pos_old+1];
    else
        proposition = formula[pos_old];

    Propositions_t::const_iterator const it(::std::find(props.begin(),
							props.end(),
							proposition));
    if(it == props.end())
	props.push_back(proposition);
}

template<typename REP>
void Cpp_Module_Assistant<REP>::p_init_proposition_map()
{
    LTL2FSM_METHOD_GUARD("");

    Propositions_t props;
 
    for(Edge_Iterator_Pair_t ep = m_nfa->edge_pair(); ep.first != ep.second; ++ep.first){

	::std::string label = m_nfa->edge_label(*ep.first);
	if(label == "epsilon" || label == "tt")
	    continue;
	
	::std::string::size_type pos = 0;
	::std::string::size_type comma = 0;
	
	// Checking for comma (corresponds to 'or' respectively another
	// edge with same source and target)
	while( (pos = label.find(", ", comma)) != ::std::string::npos){
	    
	    // Check the leftside boolean formula
	    ::std::string left(label, comma, pos-comma);
	    p_init_proposition_map_help(left, props);
	    
	    comma = pos;
	    comma+=2;
	}
	
	// Check the rightside boolean formula
	::std::string right(label, comma);
	p_init_proposition_map_help(right, props);
    }

    // Sorting alphabetically
    sort(props.begin(), props.end());

    // Now storing the vector in a Proposition_Map_t
    Propositions_t::const_iterator it = props.begin();
    Propositions_t::const_iterator const end = props.end();
    
    for(int order = 0; it != props.end(); ++it, ++order)
	m_proposition_map[*it] = order;
}

template<typename REP>
void Cpp_Module_Assistant<REP>::label_to_condition_help(::std::string const & formula, 
							::std::ostringstream & condition)
{
    LTL2FSM_METHOD_GUARD("");
    ::std::string::size_type pos = 0;
    ::std::string::size_type pos_old = 0;
    ::std::string proposition;
    
    // Checking the left proposition of '&'
    while( (pos = formula.find("&", pos)) != ::std::string::npos){
	
	if(formula[pos-3] == '!'){
	    proposition = formula[pos-2];
	    Proposition_Map_t::const_iterator it(m_proposition_map.find(proposition));
	    if(it == m_proposition_map.end()){
		// Throw exception Invalid_Proposition
	    }
	    condition << "!" << "bitvector[" << it->second << "]";
	}
	else{
	    proposition = formula[pos-2];
	    Proposition_Map_t::const_iterator it(m_proposition_map.find(proposition));
	    if(it == m_proposition_map.end()){
		// Throw exception Invalid_Proposition
	    }
	    condition << "bitvector[" << it->second << "]";
	}
	
	
	++pos;
	pos_old = pos;
	++pos_old;

	condition << " && ";
    }
    if(formula[pos_old] == 't')
	condition << "true";
    else{
	
	// Checking the right proposition of '&'
	if(formula[pos_old] == '!'){
	    proposition = formula[pos_old+1];
	    Proposition_Map_t::const_iterator it(m_proposition_map.find(proposition));
	    if(it == m_proposition_map.end()){
		// Throw exception Invalid_Proposition
	    }
	    condition << "!" << "bitvector[" << it->second << "]";
	}
	else{
	    proposition = formula[pos_old];
	    Proposition_Map_t::const_iterator it(m_proposition_map.find(proposition));
	    if(it == m_proposition_map.end()){
		// Throw exception Invalid_Proposition
	    }
	    condition << "bitvector[" << it->second << "]";
	}
    }
}


template<typename REP>
::std::string Cpp_Module_Assistant<REP>::label_to_condition(::std::string const & label)
{
    LTL2FSM_METHOD_GUARD("");
    ::std::ostringstream condition;
    
    ::std::string::size_type pos = 0;
    ::std::string::size_type comma = 0;
    
    // Checking for comma (corresponds to 'or' respectively another
    // edge with same source and target)
    while( (pos = label.find(", ", comma)) != ::std::string::npos){
	
	// Checking the leftside boolean formula
	::std::string left(label, comma, pos-comma);
	label_to_condition_help(left, condition);
	
	comma = pos;
	comma+=2;

	condition << " || ";
    }

    // Checking the rightside boolean formula
    ::std::string right(label, comma);
    label_to_condition_help(right, condition);

    return condition.str();
}

template<typename REP>
::std::string Cpp_Module_Assistant<REP>::gen_code__include_guard(::std::string const & classname,
                                                                 ::std::string const & _namespace)
{
    ::std::string guard(_namespace + "__" + classname);

    ::std::transform(guard.begin(),
                     guard.end(), 
                     guard.begin(),
                     toupper); 

    return guard;
}

template<typename REP>
::std::string Cpp_Module_Assistant<REP>::gen_code__class_comments(::std::string const & classname)
{
    ::std::ostringstream code;

    code << "/**" << ::std::endl
         << " * @class " << classname << ::std::endl
         << " *" << ::std::endl
         << " * @brief" << ::std::endl
         << " *" << ::std::endl
         << " * @nosubgrouping" << ::std::endl
         << " */" << ::std::endl;

    return code.str();
}

template<typename REP>
::std::string Cpp_Module_Assistant<REP>::gen_code__types_group(::std::string const & classname)
{
    ::std::ostringstream code;

    code << "////////////////////////////////////////////////////////////////////////////////" << ::std::endl
         << "/**" << ::std::endl
         << " * @name Types" << ::std::endl
         << " * @{" << ::std::endl
         << " */" << ::std::endl
         << "public:" << ::std::endl
         << "typedef Nfa::Bit_Vector_t Bit_Vector_t;" << ::std::endl
         << "typedef Nfa::Dstate_t Dstate_t;" << ::std::endl
         << "typedef Nfa::Accepting_Set_t Accepting_Set_t;" << ::std::endl
         << "private:" << ::std::endl
         << "typedef " << classname << " Self;" << ::std::endl
         << "// @}" << ::std::endl;

    return code.str();
}

template<typename REP>
::std::string Cpp_Module_Assistant<REP>::gen_code__construction_destruction_group(::std::string const & classname)
{
    ::std::ostringstream code;

    code << "////////////////////////////////////////////////////////////////////////////////" << ::std::endl
         << "/**" << ::std::endl
         << " * @name Construction / Destruction" << ::std::endl
         << " * @{" << ::std::endl
         << " */" << ::std::endl
         << "public:" << ::std::endl
         << classname << "();" << ::std::endl
         << "virtual ~" << classname << "();" << ::std::endl
         << classname << "(Self const & other);" << ::std::endl
         << "Self & operator=(Self const & other);" << ::std::endl
         << "private:" << ::std::endl
         << "/**" << ::std::endl
         << " * @brief Initializes the field m_accepting_map declared in class @ref ::ltl2fsm::Nfa." << ::std::endl
         << " */" << ::std::endl
         << "void init_accepting_set();" << ::std::endl
         << ::std::endl
         << "/**" << ::std::endl
         << " * @brief Initializes the field m_start_states declared in class @ref ::ltl2fsm::Nfa." << ::std::endl
         << " */" << ::std::endl
         << "void init_initial_states();" << ::std::endl
         << "// @}" << ::std::endl;

    return code.str();
}

template<typename REP>
::std::string Cpp_Module_Assistant<REP>::gen_code__input_processing_group()
{
    ::std::ostringstream code;

    code << "////////////////////////////////////////////////////////////////////////////////" << ::std::endl
         << "/**" << ::std::endl
         << " * @name Input processing" << ::std::endl
         << " * @{" << ::std::endl
         << " */" << ::std::endl
         << "public:" << ::std::endl
         << "/**" << ::std::endl
         << " * @brief The NFA jumps to a set of target states that" << ::std::endl
         << " * can be reached via input @a bitvector." << ::std::endl
         << " */" << ::std::endl
         << "virtual Dstate_t step(Dstate_t const & dstate," << ::std::endl
         << "                      Bit_Vector_t const & bitvector);" << ::std::endl
         << "// @}" << ::std::endl;

    return code.str();
}

template<typename REP>
::std::string Cpp_Module_Assistant<REP>::gen_code__interface(::std::string const & classname,
                                                             ::std::string const & ltl_formula,
                                                             ::std::string const & _namespace)
{
    LTL2FSM_METHOD_GUARD("");
    ::std::ostringstream code;


    code << gen_code__commentblock(classname, ltl_formula);
    code << ::std::endl;
    code << "#ifndef " << gen_code__include_guard(classname, _namespace) << ::std::endl;
    code << "#define " << gen_code__include_guard(classname, _namespace) << ::std::endl;
    code << ::std::endl;
    code << "#include <ltl2fsm/monior_code/Nfa.hpp>" << ::std::endl;
    code << ::std::endl;    
    code << gen_code__class_comments(classname);
    code << "class " << classname << ::std::endl;
    code << ": public Nfa" << ::std::endl;
    code << "{" << ::std::endl;
    code << gen_code__types_group(classname) << ::std::endl;
    code << gen_code__construction_destruction_group(classname) << ::std::endl;
    code << gen_code__input_processing_group() << ::std::endl;
    code << "};" << ::std::endl;
    code << ::std::endl;
    code << "#endif" << ::std::endl;

    return code.str();
}

template<typename REP>
::std::string Cpp_Module_Assistant<REP>::gen_code__initial_states()
{
    LTL2FSM_METHOD_GUARD("");
    ::std::ostringstream code;
            
    for(Edge_Iterator_Pair_t ep = m_nfa->edge_pair(); ep.first != ep.second; ++ep.first){
	if(m_nfa->edge_label(*ep.first) == "epsilon"){
	    code << "initial_states().insert(" 
		 << m_nfa->target(*ep.first) << ");" << ::std::endl;
	}
    }

    return code.str();
}

template<typename REP>
::std::string Cpp_Module_Assistant<REP>::gen_code__accepting_set()
{
    LTL2FSM_METHOD_GUARD("");
    ::std::ostringstream code;

    for(Vertex_Iterator_Pair_t vp = m_nfa->vertex_pair(); vp.first != vp.second; ++vp.first){
	if(m_nfa->accepting(*vp.first)){
	    code << "accepting_set().push_back("
		 << *vp.first << ");" << ::std::endl;
	}
    }

    return code.str();
}

template<typename REP>
::std::string Cpp_Module_Assistant<REP>::gen_code__include_directive(::std::string const & classname,
								     ::std::string const & include_prefix,
								     bool include_type)
{
    LTL2FSM_METHOD_GUARD("");
    if (include_type)
        return ::std::string("#include <" + include_prefix + classname + ".hpp" + ">\n");
    else
        return ::std::string("#include \"" + include_prefix + classname + ".hpp" + "\"\n");
}

template<typename REP>
::std::string Cpp_Module_Assistant<REP>::gen_code__switch_case()
{
    LTL2FSM_METHOD_GUARD("");
    typedef ::std::stack<Vertex_Descriptor_t> Vertex_Stack_t;
    Vertex_Stack_t vstack;
    ::std::ostringstream switch_case;

    switch_case << "switch(*state){" << ::std::endl;
	       
    for(Vertex_Iterator_Pair_t vp = m_nfa->vertex_pair(); vp.first != vp.second; ++vp.first){
	if(m_nfa->vertex_label(*vp.first) != "")
	    vstack.push(*vp.first);
    }

    while(!vstack.empty()){
	Vertex_Descriptor_t v = vstack.top();
	vstack.pop();
	
	switch_case << "case " << v << ":" << ::std::endl;
			   
	for(Out_Edge_Iterator_Pair_t oep = m_nfa->out_edges(v); oep.first != oep.second; ++oep.first){
	    switch_case << "if(" << label_to_condition(m_nfa->edge_label(*oep.first)) << ")" << ::std::endl;
            switch_case << "target.insert(" << m_nfa->target(*oep.first) << ");" << ::std::endl;
	}
	switch_case << "break;" << ::std::endl;
    }

    switch_case << "}" << ::std::endl;
    return switch_case.str();
}

template<typename REP>
::std::string Cpp_Module_Assistant<REP>::gen_code__init_initial_states(::std::string const & classname)
{
    LTL2FSM_METHOD_GUARD("");
    ::std::ostringstream code;
    
    code << "void " + classname + "::init_initial_states()" << ::std::endl;
        
    code << "{" << ::std::endl;
    code << gen_code__initial_states();
    code << "}" << ::std::endl;

    return code.str();
}

template<typename REP>
::std::string Cpp_Module_Assistant<REP>::gen_code__init_accepting_set(::std::string const & classname)
{
    LTL2FSM_METHOD_GUARD("");
    ::std::ostringstream code;
    
    code << "void " + classname + "::init_accepting_set()" << ::std::endl;
        
    code << "{" << ::std::endl;
    code << gen_code__accepting_set();
    code << "}" << ::std::endl;

    return code.str();
}


template<typename REP>
::std::string Cpp_Module_Assistant<REP>::gen_code__constructor(::std::string const & classname)
{
    LTL2FSM_METHOD_GUARD("");
    ::std::ostringstream code;
    
    code << classname + "::" + classname + "()" << ::std::endl;
    
    code << "{" << ::std::endl;
    code << "init_initial_states();" << ::std::endl;
    code << "init_accepting_set();" << ::std::endl;
    code << "}" << ::std::endl;
    
    return code.str();
}

template<typename REP>
::std::string Cpp_Module_Assistant<REP>::gen_code__destructor(::std::string const & classname)
{
    LTL2FSM_METHOD_GUARD("");
    ::std::ostringstream code;
    
    code << classname + "::" + "~" + classname + "()" << ::std::endl;
        
    code << "{" << ::std::endl;
    code << "}" << ::std::endl;
    
    return code.str();
}

template<typename REP>
::std::string Cpp_Module_Assistant<REP>::gen_code__step(::std::string const & classname)
{
    LTL2FSM_METHOD_GUARD("");
    ::std::ostringstream code;
    
    code << classname + "::Dstate_t " + classname + "::step(Dstate_t const & dstate, Bit_Vector_t const & bitvector)" 
         << ::std::endl;

    code << "{" << ::std::endl;
    code << "Dstate_t target;" << ::std::endl;
    code << "Dstate_t::const_iterator state(dstate.begin());" << ::std::endl;
    code << "Dstate_t::const_iterator const end(dstate.end()); " << ::std::endl;
    code << "for(; state != end; ++state){" << ::std::endl;
    code << gen_code__switch_case();
    code << "}" << ::std::endl;
    code << "return target;" << ::std::endl;
    code << "}" << ::std::endl;
    
    return code.str();
}

template<typename REP>
::std::string Cpp_Module_Assistant<REP>::gen_code__commentblock(::std::string const & classname,
                                                                ::std::string const & ltl_formula)
{
    LTL2FSM_METHOD_GUARD("");
    ::std::ostringstream code;

    code << "/*" << ::std::endl
         << " * " + classname + ".hpp" << ::std::endl
         << " *" << ::std::endl
         << " * Corresponding LTL formula: " + ltl_formula << ::std::endl
         << " * Generated by ltl2fsm, copyright (C) 2005 Oliver Arafat" << ::std::endl
         << "*/" << ::std::endl;
    
    return code.str();
}

template<typename REP>
::std::string Cpp_Module_Assistant<REP>::gen_code__impl(::std::string const & classname,
                                                        ::std::string const & _namespace,
                                                        ::std::string const & include_prefix,
                                                        ::std::string const & ltl_formula,
                                                        bool include_type)
{
    LTL2FSM_METHOD_GUARD("");
    ::std::ostringstream code;
    
    code << gen_code__commentblock(classname, ltl_formula);
    code << ::std::endl;
    code << gen_code__include_directive(classname, include_prefix, include_type);
    code << ::std::endl;
    
    if(_namespace != "")
	code << "namespace " << _namespace << "{" << ::std::endl;
    code << gen_code__constructor(classname) << ::std::endl;
    code << gen_code__destructor(classname) << ::std::endl;
    code << gen_code__init_initial_states(classname) << ::std::endl;
    code << gen_code__init_accepting_set(classname) << ::std::endl;
    code << gen_code__step(classname);
    if(_namespace != "")
	code << "}" << ::std::endl;
    
    return code.str();
}


LTL2FSM__END__NAMESPACE__INTERNAL;


//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////


#if LTL2FSM_DEBUG__LEVEL__ > 1
template<typename REP>
void Nfa_To_Cpp_Module<REP>::m_class_invariance() const
{
}
#endif


template<typename REP>
Nfa_To_Cpp_Module<REP>::Nfa_To_Cpp_Module()
    : Module()
{    
    LTL2FSM_AUDIT_METHOD_GUARD("");
    LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT;
}

template<typename REP>
Nfa_To_Cpp_Module<REP>::~Nfa_To_Cpp_Module()
{    
    LTL2FSM_AUDIT_METHOD_GUARD("");
    LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT;
}

template<typename REP>
Nfa_To_Cpp_Module<REP>::Nfa_To_Cpp_Module(Self const & other)
{
    LTL2FSM_AUDIT_METHOD_GUARD("");
    LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT;
}

template<typename REP>
Nfa_To_Cpp_Module<REP> & Nfa_To_Cpp_Module<REP>::operator=(Self const & other)
{
    LTL2FSM_METHOD_GUARD("");
    return *this;
}


template<typename REP>
void Nfa_To_Cpp_Module<REP>::execute(NFA_t & nfa, 
				     ::std::ostream & cpp,
                                     ::std::ostream & hpp,
				     ::std::string const & classname,
				     ::std::string const & _namespace,
				     ::std::string const & include_prefix,
                                     ::std::string const & ltl_formula,
				     bool include_type)
{
    LTL2FSM_METHOD_GUARD("");
    using namespace ::ltl2fsm::internal;
    
    Cpp_Module_Assistant<REP> assistant(nfa);
    cpp << assistant.gen_code__impl(classname,
                                    _namespace,
                                    include_prefix,
                                    ltl_formula,
                                    include_type);
    
    hpp << assistant.gen_code__interface(classname,
                                         ltl_formula,
                                         _namespace);
}


LTL2FSM__END__NAMESPACE__LTL2FSM;


#endif
