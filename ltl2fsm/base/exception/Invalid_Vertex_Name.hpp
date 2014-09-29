/**
 * @file ltl2fsm/base/exception/Invalid_Vertex_Name.hpp
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


#ifndef LTL2FSM__BASE__EXCEPTION__INVALID_VERTEX_NAME__HPP
#define LTL2FSM__BASE__EXCEPTION__INVALID_VERTEX_NAME__HPP


#include<ltl2fsm/base/exception/Exception.hpp>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;

class Invalid_Vertex_Name
    : public Exception
{
    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Construction / Destruction
     * @{
     */
public:
    Invalid_Vertex_Name(Source_Location const & source_location, 
			::std::string const & what);
    virtual ~Invalid_Vertex_Name() throw();
    // @}
    
public:
    virtual char const * class_name() const;
};


LTL2FSM__END__NAMESPACE__LTL2FSM;


#endif
