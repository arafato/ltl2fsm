/**
 * @file ltl2fsm/monitor_code/Automaton.hpp
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


#ifndef LTL2FSM__MONITOR_CODE__AUTOMATON__HPP
#define LTL2FSM__MONITOR_CODE__AUTOMATON__HPP


#include <ltl2fsm/base/config.hpp>

#include <string>
#include <vector>
#include <set>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


/**
 * @class Automaton
 *
 * @brief Abstract base class. It defines the common interface for all
 * automata types (non-deterministic and deterministic) that are
 * represented by CPP-code.
 *
 * @nosubgrouping
 */
class Automaton
{
    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Types
     * @{
     */
public:
  /**
     * @brief The values of Bit_Vector_t correspond to the truth
     * values of the propositions to observe. It is assumed, that they
     * appear alphabetically in the order of their corresponding
     * propositions.
     */ 
    typedef ::std::vector<bool> Bit_Vector_t;
    
    /**
     * @brief Dstate_t represents a single DFA state. A DFA state can
     * consist of multiple NFA states. They are referred to through
     * their ids.
     */     
    typedef ::std::set<int> Dstate_t;
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Destruction
     * @{
     */
public:
    virtual ~Automaton();
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Input processing
     * @{
     */
public:
    /**
     * @brief Returns true if a state within @dstate is an
     * accepting state, false if it is a non-accepting state.
     */ 
    virtual bool accepting(Dstate_t const & dstate) const = 0;
    // @}
};


LTL2FSM__END__NAMESPACE__LTL2FSM;


#endif
