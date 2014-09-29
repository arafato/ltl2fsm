/**
 * @file ltl2fsm/base/exception/Invalid_Vertex_Name.cpp
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


#include <ltl2fsm/base/exception/Invalid_Vertex_Name.hpp>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


Invalid_Vertex_Name::Invalid_Vertex_Name(Source_Location const & source_location,
					 ::std::string const & what)
    : Exception(source_location, what)
{
}

Invalid_Vertex_Name::~Invalid_Vertex_Name() throw()
{
}

char const * Invalid_Vertex_Name::class_name() const
{
    return "Invalid_Vertex_Name";
}


LTL2FSM__END__NAMESPACE__LTL2FSM;
