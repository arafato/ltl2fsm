/**
 * @file ltl2fsm/base/exception/Invalid_Ltl_Syntax.cpp
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


#include <ltl2fsm/base/exception/Invalid_Ltl_Syntax.hpp>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


Invalid_Ltl_Syntax::Invalid_Ltl_Syntax(Source_Location const & source_location,
				       ::std::string const & what)
    : Exception(source_location, what)
{
}

Invalid_Ltl_Syntax::~Invalid_Ltl_Syntax() throw()
{
}

char const * Invalid_Ltl_Syntax::class_name() const
{
    return "Invalid_Ltl_Syntax";
}


LTL2FSM__END__NAMESPACE__LTL2FSM;
