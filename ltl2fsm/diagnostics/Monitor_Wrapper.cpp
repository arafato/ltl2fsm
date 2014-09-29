
#include <ltl2fsm/diagnostics/Monitor_Wrapper.hpp>

#include <ltl2fsm/monitor_code/Fsm.hpp>

LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;

Monitor_Wrapper::~Monitor_Wrapper()
{
}

Monitor_Wrapper::Monitor_Wrapper(Fsm * const fsm,
				 int const bit_vector_size)
    : m_fsm(fsm),
      m_bit_vector(bit_vector_size)
{
}

void Monitor_Wrapper::log(Record_t const & record)
{
    if(p_translate(record,m_bit_vector))
	m_fsm->process_input(m_bit_vector);
}

void Monitor_Wrapper::close()
{
}

Monitor_Wrapper::Status_t Monitor_Wrapper::status() const
{
    return m_fsm->status();
}



LTL2FSM__END__NAMESPACE__LTL2FSM;
