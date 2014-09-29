/**
 * @file ltl2fsm/base/exception/Fork_Error.cpp
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


#include <ltl2fsm/base/exception/Fork_Error.hpp>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


Fork_Error::Fork_Error(Source_Location const & source_location,
		       ::std::string const & what)
    : Exception(source_location, what)
{
}

Fork_Error::~Fork_Error() throw()
{
}

char const * Fork_Error::class_name() const
{
    return "Fork_Error";
}


LTL2FSM__END__NAMESPACE__LTL2FSM;
