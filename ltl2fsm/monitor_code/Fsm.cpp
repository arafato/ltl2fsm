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


#include <ltl2fsm/monitor_code/Fsm.hpp>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


Fsm::Fsm(Dfa * const pos_dfa,
	 Dfa * const neg_dfa)
    : m_pos_dfa(pos_dfa),
      m_neg_dfa(neg_dfa)
{
}

Fsm::~Fsm()
{
}

void Fsm::process_input(Bit_Vector_t const & bitvector)
{
    m_pos_dfa->step(bitvector);
    m_neg_dfa->step(bitvector);
}

Fsm::Status_t Fsm::status() const
{
    if(!m_pos_dfa->accepting(m_pos_dfa->state()))
	return BOTTOM;

    if(!m_neg_dfa->accepting(m_neg_dfa->state()))
	return TOP;

    return INCONCLUSIVE;
}

void Fsm::reset()
{
    m_pos_dfa->reset();
    m_neg_dfa->reset();
}

LTL2FSM__END__NAMESPACE__LTL2FSM;
