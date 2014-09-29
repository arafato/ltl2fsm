/**
 * @file ltl2fsm/base/exception/Edge_Already_Added.hpp
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


#ifndef LTL2FSM__BASE__EXCEPTION__EDGE_ALREADY_ADDED__HPP
#define LTL2FSM__BASE__EXCEPTION__EDGE_ALREADY_ADDED__HPP


#include<ltl2fsm/base/exception/Exception.hpp>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


class Edge_Already_Added
    : public Exception
{
    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Construction / Destruction
     * @{
     */
public:
    Edge_Already_Added(Source_Location const & source_location, 
		       ::std::string const & what);
    virtual ~Edge_Already_Added() throw();
    // @}
    
public:
    virtual char const * class_name() const;
};


LTL2FSM__END__NAMESPACE__LTL2FSM;


#endif
