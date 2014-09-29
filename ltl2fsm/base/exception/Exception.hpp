/**
 * @file ltl2fsm/base/exception/Exception.hpp
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


#ifndef LTL2FSM__BASE__EXCEPTION__EXCEPTION__HPP
#define LTL2FSM__BASE__EXCEPTION__EXCEPTION__HPP


#include <ltl2fsm/base/namespace.hpp>
#include <ltl2fsm/base/exception/Source_Location.hpp>

#include <exception>
#include <string>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


/**
 * @class Exception
 *
 * @brief The exception base class within ltl2fsm. All specific
 * exception classes are to be derived from this class.
 *
 * @nosubgrouping
 */
class Exception 
    : public ::std::exception
{
public:
    explicit Exception(Source_Location const & source_location,
		       ::std::string const & what);
    virtual ~Exception() throw();
    
public:
    /**
     * @brief The usual what string, i.e., this string should describe
     * the complete contents (ideally) of the exception in human
     * readable fashion -- with the description of the source
     * location.
     */ 
    virtual char const * what() const throw();


    /**
     * @brief The usual what string, i.e., this string should describe
     * the complete contents (ideally) of the exception in human
     * readable fashion -- without the description of the source
     * location and the exception name.
     */ 
    virtual char const * plain_what() const;
    
    
    /**
     * @brief Returns the name of the class.
     */ 
    virtual char const * class_name() const = 0;
    
private: 
    Source_Location m_source_location;

    /**
     * @brief The usual what description.
     */ 
    ::std::string m_what;
};


LTL2FSM__END__NAMESPACE__LTL2FSM;


#endif
