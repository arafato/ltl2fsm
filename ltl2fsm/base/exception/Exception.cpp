/**
 * @file ltl2fsm/base/exception/Exception.cpp
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


#include <ltl2fsm/base/exception/Exception.hpp>

#include <sstream>
#include <iostream>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


Exception::Exception(Source_Location const & source_location,
		     ::std::string const & what)
    : m_source_location(source_location),
      m_what(what)
{
}

Exception::~Exception() throw()
{
}

char const * Exception::what() const throw()
{
    ::std::ostringstream msg;
    
    msg << class_name() << " at " << m_source_location 
	<< ": " << plain_what() << ::std::endl;

    return (msg.str()).c_str();
}

char const * Exception::plain_what() const
{
    return m_what.c_str();
}


LTL2FSM__END__NAMESPACE__LTL2FSM;

