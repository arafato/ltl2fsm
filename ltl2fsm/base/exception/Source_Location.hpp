/**
 * @file ltl2fsm/base/exception/Source_Location.hpp
 *
 * $Id$
 *
 * @author Christian Schallhart, Oliver Arafat
 *
 * @brief @ref
 *
 * @note DOCUMENTED
 *
 * @test
 *
 * @todo
 */


#ifndef LTL2FSM__BASE__EXCEPTION__SOURCE_LOCATION__HPP
#define LTL2FSM__BASE__EXCEPTION__SOURCE_LOCATION__HPP

#include <ltl2fsm/base/namespace.hpp>

#include <iostream>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


/**
 * @class Source_Location
 *
 * @brief A source location is represented by a file, base_file and a
 * line number. This information is generated with the __FILE__ , @ref
 * BASE_FILE , and the __LINE__ macros. To generate a Source_Location
 * which corresponds to the current position, use @ref
 * LTL2FSM__SOURCE_LOCATION.
 *
 * Source_Location is a Record-Class, i.e., it is a compound for its
 * fields, not more, not less.
 *
 * @nosubgrouping
 */
class Source_Location
{
private:
    typedef Source_Location Self;
public:
    Source_Location(char const * const file,
		    char const * const base_file,
		    int const line);
    inline ~Source_Location();
    Source_Location(Self const & other);
    Self & operator=(Self const & other);
    bool operator==(Self const & other) const;
		
    inline char const * file() const;
    inline char const * base_file() const;
    inline int line() const;
private:
    char const * m_file;
    char const * m_base_file;
    int m_line;
};

Source_Location::~Source_Location()
{
}

char const * Source_Location::file() const
{
    return m_file;
}

char const * Source_Location::base_file() const
{
    return m_base_file;
}

int Source_Location::line() const
{
    return m_line;
}


::std::ostream & operator<<(::std::ostream & stream, 
			    Source_Location const & sl);


inline bool operator!=(Source_Location const & lhs,
		       Source_Location const & rhs)
{
    return !lhs.operator==(rhs);
}


LTL2FSM__END__NAMESPACE__LTL2FSM;


/**
 * @brief A @ref ::ltl2fsm::Source_Location object which represents
 * the current source location.
 */ 
#define LTL2FSM__SOURCE_LOCATION ::ltl2fsm::Source_Location(__FILE__,__BASE_FILE__,__LINE__)


#endif
