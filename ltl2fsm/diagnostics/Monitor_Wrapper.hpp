/**
 * @file ltl2fsm/modules/Ltl_To_Nba_Module.hpp
 *
 * $Id$
 *
 * @author Christian Schallhart
 *
 * @brief [LEVEL: pre-alpha] @ref ::ltl2fsm::Monitor_Wrapper
 *
 * @test
 *
 * @todo document
 */


#ifndef LTL2FSM__DIAGNOSTICS__MONITOR_WRAPPER__HPP
#define LTL2FSM__DIAGNOSTICS__MONITOR_WRAPPER__HPP

#include <ltl2fsm/monitor_code/Fsm.hpp>

#include <diagnostics/frame/logger.hpp>

#include <vector>

LTL2FSM__BEGIN__NAMESPACE__LTL2FSM;

/**
 * @class Monitor_Code
 *
 * @brief This class wrapps an @ref Fsm into a @ref ::diagnostics::Logger
 *
 * @nosubgrouping
 */

class Monitor_Wrapper
    : public ::diagnostics::Logger
{
    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Types
     * @{
     */
protected:
    typedef Fsm::Bit_Vector_t Bit_Vector_t;
private:
    typedef ::diagnostics::Record Record_t;
    typedef Monitor_Wrapper Self;
public:
    typedef Fsm::Status_t Status_t;
    // @}

    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Disabled Creation
     * @{
     */
public:
    Self & operator=(Self const & ohter);
    Monitor_Wrapper(Self const & other);
    // @}

    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Create
     * @{
     */
public:
    /**
     * @todo clarify ownership of fsm
     * @todo document
     */ 
    Monitor_Wrapper(Fsm * const fsm,
		    int const bit_vector_size);
    virtual ~Monitor_Wrapper();
    // @}

    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Logger Interface
     * @{
     */
public:
    /**
     * @todo document
     */ 
    virtual void log(Record_t const & record);
    /**
     * @todo document
     */ 
    virtual void close();
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Template Method: Abstracting Records
     * @{
     */
protected:
    /**
     * @todo document
     */ 
    virtual bool p_translate(Record_t const & record, Bit_Vector_t & bit_vector)=0;
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Status 
     * @{
     */
public:
    Status_t status() const;
    // @}


    ////////////////////////////////////////////////////////////////////////////////
    /**
     * @name Encapsulated State
     * @{
     */
private:
    Fsm * m_fsm;
    Bit_Vector_t m_bit_vector;
    // @}
};



LTL2FSM__END__NAMESPACE__LTL2FSM;


#endif
