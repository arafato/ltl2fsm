/**
 * @file ltl2fsm/base/exception/Invalid_Ltl_Syntax.hpp
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


#ifndef LTL2FSM__BASE__EXCEPTION__INVALID_LTL_SYNTAX__HPP
#define LTL2FSM__BASE__EXCEPTION__INVALID_LTL_SYNTAX__HPP


#include <ltl2fsm/base/exception/Exception.hpp>

//#include <string>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


class Invalid_Ltl_Syntax
    : public Exception
{
    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Construction / Destruction
     * @{
     */
public:
    Invalid_Ltl_Syntax(Source_Location const & source_location, 
		       ::std::string const & what);
    virtual ~Invalid_Ltl_Syntax() throw();
    // @}
    
public:
    virtual char const * class_name() const;
};


LTL2FSM__END__NAMESPACE__LTL2FSM;


#endif
