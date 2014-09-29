/**
 * @file ltl2fsm/monitor_code/Fsm.hpp
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


#ifndef LTL2FSM__MONITOR_CODE__FSM__HPP
#define LTL2FSM__MONITOR_CODE__FSM__HPP


#include <ltl2fsm/base/config.hpp>
#include <ltl2fsm/monitor_code/Dfa.hpp>

#include <vector>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


/**
 * @class Fsm
 *
 * @brief TODO
 *
 * @nosubgrouping
 */

class Fsm
{
    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Types
     * @{
     */
public:
    /**
     * @brief Represents the three possible states a FSM-state can be
     * in.
     */ 
    typedef enum {
	/**
	 * @brief A FSM state is TOP if it is not element of the set
	 * of accepting states of the DFA that corresponds to some ltl
	 * formula '!p'.
	 */ 
	TOP=10,
	/**
	 * @brief A FSM state is BOTTOM if it is not element of the
	 * set of accepting states of the DFA that corresponds to some
	 * ltl formula 'p'.
	 */ 
	BOTTOM=20,
	/**
	 * @brief A FSM state is INCONCLUSIVE if it is both element of
	 * the set of accepting states of a DFA that corresponds to
	 * some ltl formula 'p' and at the same time of a DFA that
	 * corresponds to '!p'.
	 */ 
	INCONCLUSIVE=30
    } Status_t;

    /**
     * @brief The values of Bit_Vector_t correspond to the truth
     * values of the propositions to observe. It is assumed, that they
     * appear alphabetically in the order of their corresponding
     * propositions.
     *
     * @note This type is redundantly defined in @ref Automaton
     */ 
    typedef ::std::vector<bool> Bit_Vector_t;

private:
    typedef Fsm Self;
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Construction / Destruction
     * @{
     */
public:
    Fsm(Dfa * const pos_dfa,
	Dfa * const neg_dfa);
    ~Fsm();
    Fsm(Self const & other);
    Self & operator=(Self const & other);
    // @}

    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Input processing
     * @{
     */
public:
    /**
     * @brief The state machine jumps to a set of target states that
     * can be reached via input @a bitvector.
     */ 
    void process_input(Bit_Vector_t const & bitvector);

    /**
     * @brief Returns the actual status (TOP, BOTTOM, INCONCLUSIVE) of
     * the state the FSM is in.
     */ 
    Status_t status() const;

    /**
     * @brief Resets FSM to its initial state, i.e. the initial states
     * of positive DFA and negative DFA.
     */ 
    void reset();
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name State
     * @{
     */
private:
    Dfa * m_pos_dfa;
    Dfa * m_neg_dfa;
    // @}
};


LTL2FSM__END__NAMESPACE__LTL2FSM;


#endif
