/**
 * @file ltl2fsm/base/exception/Fork_Error.hpp
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


#ifndef LTL2FSM__BASE__EXCEPTION__FORK_ERROR__HPP
#define LTL2FSM__BASE__EXCEPTION__FORK_ERROR__HPP


#include <ltl2fsm/base/exception/Exception.hpp>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


class Fork_Error
    : public Exception
{
    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Construction / Destruction
     * @{
     */
public:
    Fork_Error(Source_Location const & source_location, 
	       ::std::string const & what);
    virtual ~Fork_Error() throw();
    // @}
    
public:
    virtual char const * class_name() const;
};


LTL2FSM__END__NAMESPACE__LTL2FSM;


#endif
