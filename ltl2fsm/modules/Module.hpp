/**
 * @file ltl2fsm/modules/Module.hpp
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


#ifndef LTL2FSM__MODULES__MODULE__HPP
#define LTL2FSM__MODULES__MODULE__HPP


#include <ltl2fsm/base/config.hpp>


LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;


/**
 * @class Module
 *
 * @brief Abstract base class for all module classes. In LTL2FSM,
 * functionality wrt to the conversion process is swapped out to
 * different modules.
 *
 * At the moment, this class does not really define an interface.
 *
 * @nosubgrouping
 */
class Module
{
    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Destruction
     * @{
     */
public:
    virtual ~Module();
    // @}
};


LTL2FSM__END__NAMESPACE__LTL2FSM;


#endif
