/**
 * @file Edge_Already_Added.cpp
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


#include <ltl2fsm/base/exception/Edge_Already_Added.hpp>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


Edge_Already_Added::Edge_Already_Added(Source_Location const & source_location,
				       ::std::string const & what)
    : Exception(source_location, what)
{
}

Edge_Already_Added::~Edge_Already_Added() throw()
{
}

char const * Edge_Already_Added::class_name() const
{
    return "Edge_Already_Added";
}


LTL2FSM__END__NAMESPACE__LTL2FSM;
