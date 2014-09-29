/**
 * @file ltl2fsm/modules/Nfa_To_Cpp_Module.hpp
 *
 * $Id$
 *
 * @author Oliver Arafat
 *
 * @brief @ref
 *
 * @test
 *
 * @todo Documentation
 */


#ifndef LTL2FSM__MODULES__NFA_TO_CPP_MODULE__HPP
#define LTL2FSM__MODULES__NFA_TO_CPP_MODULE__HPP


#include <ltl2fsm/base/config.hpp>
#include <ltl2fsm/modules/Module.hpp>

#include <iostream>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


template <typename REP> class Nondet_Finite_Automaton;


/**
 * @class Nfa_To_Cpp_Module
 *
 * @brief This module offers an interface that allows to generate
 * C++-code from an according @ref Nondet_Finite_Automaton object
 * based on a certain representation.
 *
 * @todo more docu on several interfaces wrt to code generation dir
 *
 * @nosubgrouping
 */
template<typename REP>
class Nfa_To_Cpp_Module
    : public Module
{
    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Types
     * @{
     */
public:
    typedef REP Representation_t;
    typedef Nondet_Finite_Automaton<Representation_t> NFA_t;
    
private:
    typedef Nfa_To_Cpp_Module<REP> Self;
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Construction / Destruction
     * @{
     */
public:
    Nfa_To_Cpp_Module();
    virtual ~Nfa_To_Cpp_Module();
    Nfa_To_Cpp_Module(Self const & other);
    Self & operator=(Self const & other);
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Translation
     * @{
     */
public:
    /**
     * @brief The C++-implementation of the NFA @a nfa is written to
     * @a cpp. Depending on the value of @a nfa_type, the resulting
     * class will be called Pos_Nfa (true) or Neg_Nfa (false).
     *
     * @a _namespace determines the namespace of the implementation
     * code, @a include_prefix the prefix of the include path and @a
     * include_type whether include paths are enclosed by '<>' (true)
     * or ' "" ' (false).
     *
     * @note It is assumed that the include prefix ends with '/'.
     */ 
    void execute(NFA_t & nfa, 
		 ::std::ostream & cpp,
                 ::std::ostream & hpp,
		 ::std::string const & classname,
		 ::std::string const & _namespace,
		 ::std::string const & include_prefix,
                 ::std::string const & ltl_formula,
		 bool include_type);
    // @}
    
    
#if LTL2FSM_DEBUG__LEVEL__ > 1
public:
    void m_class_invariance() const;
#endif
};


LTL2FSM__END__NAMESPACE__LTL2FSM;


#endif
