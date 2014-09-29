/**
 * @file ltl2fsm/monitor_code/Nfa.hpp
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


#ifndef LTL2FSM__MONITOR_CODE__NFA__HPP
#define LTL2FSM__MONITOR_CODE__NFA__HPP


#include <ltl2fsm/base/config.hpp>
#include <ltl2fsm/monitor_code/Automaton.hpp>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


/**
 * @class Base class for all NFA's.
 *
 * @todo DOCU
 *
 * @brief
 *
 * @nosubgrouping
 */
class Nfa
    : public Automaton
{
    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Types
     * @{
     */
public:
    typedef Automaton::Bit_Vector_t Bit_Vector_t;
    typedef Automaton::Dstate_t Dstate_t;

    /**
     * @brief Holds the id of all accepting states.
     */ 
    typedef ::std::vector<int> Accepting_Set_t;

private:
    typedef Nfa Self;
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Construction / Destruction
     * @{
     */
public:
    Nfa();
    virtual ~Nfa();
    Nfa(Self const & other);
    Self & operator=(Self const & other);
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Input processing
     * @{
     */
public:
    /**
     * @brief The NFA jumps to a set of target states that
     * can be reached via input @a bitvector.
     */ 
    virtual Dstate_t step(Dstate_t const & dstate,
			  Bit_Vector_t const & bitvector) = 0;
        
    /**
     * @brief Returns true if a state within @dstate is an
     * accepting state, false if it is a non-accepting state.
     */ 
    virtual bool accepting(Dstate_t const & dstate) const;
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Field access
     * @{
     */
public:
    /**
     * @brief Returns the set of initial states.
     *
     * @note This field will be initialized by POS_NFA respectively
     * NEG_DFA.
     */ 
    Dstate_t & initial_states();

    /**
     * @brief Returns the accepting set.
     *
     * @note This field will be initialized by POS_NFA respectively
     * NEG_DFA.
     */ 
    Accepting_Set_t & accepting_set();
    // @}

    
    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name State
     * @{
     */
private:
    Accepting_Set_t m_accepting_set;
    Dstate_t m_initial_states;
    //@}
};


LTL2FSM__END__NAMESPACE__LTL2FSM;


#endif
