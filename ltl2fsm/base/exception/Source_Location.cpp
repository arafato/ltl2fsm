/**
 * @file ltl2fsm/base/exception/Source_Location.cpp
 *
 * $Id$
 *
 * @author Christian Schallhardt
 *
 * @brief @ref
 *
 * @note DOCUMENTED
 *
 * @test
 *
 * @todo
 */


#include <ltl2fsm/base/exception/Source_Location.hpp>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


Source_Location::Source_Location(char const * const file,
				 char const * const base_file,
				 int const line)
    : m_file(file),
      m_base_file(base_file),
      m_line(line)
{
}

Source_Location::Source_Location(Self const & other)
    : m_file(other.m_file),
      m_base_file(other.m_base_file),
      m_line(other.m_line)
{
}

Source_Location & Source_Location::operator=(Self const & other)
{
    m_file=other.m_file;
    m_base_file=other.m_base_file;
    m_line=other.m_line;
    return *this;
}

bool Source_Location::operator==(Self const & other) const
{
    return m_line==other.m_line
	&& m_file==other.m_file
	&& m_base_file==other.m_base_file;
}


::std::ostream & operator<<(::std::ostream & stream, Source_Location const & sl)
{
    return stream << "[ " << sl.file()
		  << " at " << sl.line()
		  << " in base " << sl.base_file()
		  << " ]";
}


LTL2FSM__END__NAMESPACE__LTL2FSM;
