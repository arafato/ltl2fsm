/**
 * @file ltl2fsm/monitor_code/Dfa.hpp
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


#ifndef LTL2FSM__MONITOR_CODE__DFA__HPP
#define LTL2FSM__MONITOR_CODE__DFA__HPP


#include <ltl2fsm/base/config.hpp>

#include <ltl2fsm/monitor_code/Automaton.hpp>
#include <ltl2fsm/monitor_code/Nfa.hpp>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


/**
 * @class Dfa
 *
 * @todo DOCU
 *
 * @brief
 *
 * @nosubgrouping
 */
class Dfa
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

private:
    typedef Dfa Self;
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Construction / Destruction
     * @{
     */
public:
    Dfa(Nfa * const nfa);
    virtual ~Dfa();
    Dfa(Self const & other);
    Self & operator=(Self const & other);
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Input processing
     * @{
     */
public:
    /**
     * @brief The DFA jumps to a set of target states that
     * can be reached via input @a bitvector.
     */ 
    void step(Bit_Vector_t const & bitvector);

    /**
     * @brief Returns the actual state the DFA is in.
     */ 
    Dstate_t const & state();

    /**
     * @brief Resets the Dfa to its initial state.
     */ 
    void reset();
                
    /**
     * @brief Returns true if a state within @dstate is an accepting
     * state, false if there is no accepting state.
     */ 
    virtual bool accepting(Dstate_t const & dstate) const;
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name State
     * @{
     */
private:
    /**
     * @brief The corresponding NFA.
     */ 
    Nfa * m_nfa;

    /**
     * @brief The set of states the DFA is simultanously in at the
     * moment.
     */ 
    Dstate_t m_dstate;
    // @}
};


LTL2FSM__END__NAMESPACE__LTL2FSM;


#endif
