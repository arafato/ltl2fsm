/*
 * Diagnostics - A Framework for Program Monitoring and Diagnostics
 * Copyright (C) 2002-2005 Christian Schallhart
 *
 * For permission to use, modify, copy, or distribute this software,
 * contact the author (christian@schallhart.net). 
 */

/**
 * @file ltl2fsm/base/annotations.hpp
 *
 * $Id: lib_front_end.hpp,v 1.18 2005/06/23 09:54:22 esdentem Exp $
 *
 * @author Christian Schallhart
 *
 * @brief [LEVEL: alpha] Conditional Macros for logging and debuging
 * specific for a library. This file is meant as a template for a
 * library specific interface. Copy the this file and replace XXX with
 * the library name.
 *
 * @test none -- copied in from diagnostics
 */
#ifndef LTL2FSM__BASE__ANNOTATIONS__HPP
#define LTL2FSM__BASE__ANNOTATIONS__HPP

#include <diagnostics/macros/check_annotation.hpp>
#include <diagnostics/macros/assert_annotation.hpp>
#include <diagnostics/macros/block_annotation.hpp>
#include <diagnostics/macros/invariance_annotation.hpp>
#include <diagnostics/macros/exceptionless_block_annotation.hpp>
#include <diagnostics/macros/relation_modifier.hpp>

#ifndef LTL2FSM_DEBUG__LEVEL__
#  define LTL2FSM_DEBUG__LEVEL__ 2
#endif

#ifndef LTL2FSM_INLINE
#  if LTL2FSM_DEBUG__LEVEL__ == 0
#    define LTL2FSM_INLINE inline 
#  else
#    define LTL2FSM_INLINE 
#  endif
#endif


#ifndef LTL2FSM_BASE_EXCEPTION_TYPE
#  define LTL2FSM_BASE_EXCEPTION_TYPE ::ltl2fsm::Exception
#  include <ltl2fsm/base/exception/Exception.hpp>
#  define LTL2FSM_BASE_EXCEPTION_STR_WHAT(EX) (::std::string("EXCEPTION=\"") + (EX).class_name() + "\" WHAT=\"" + (EX).what() + "\"")
#endif

#ifndef LTL2FSM_HANDLE_TEST_EXCEPTION_EXPLICITLY
#  define LTL2FSM_HANDLE_TEST_EXCEPTION_EXPLICITLY 1
#endif


#ifndef LTL2FSM_FAILED_ASSERT_ACTION
#  define LTL2FSM_FAILED_ASSERT_ACTION(LEVEL,NAME,COND) throw NAME(#COND)
#endif

#ifndef LTL2FSM_FAILED_ASSERT_ACTION1
#  define LTL2FSM_FAILED_ASSERT_ACTION1(LEVEL,NAME,COND,EXCEP_WHAT) throw NAME(EXCEP_WHAT)
#endif

#ifndef LTL2FSM_FAILED_CHECK_ACTION
#  define LTL2FSM_FAILED_CHECK_ACTION(LEVEL,NAME,COND) throw NAME(#COND)
#endif

#ifndef LTL2FSM_FAILED_CHECK_ACTION1
#  define LTL2FSM_FAILED_CHECK_ACTION1(LEVEL,NAME,COND,EXCEP_WHAT) throw NAME(EXCEP_WHAT)
#endif


#ifndef LTL2FSM_NR_WHAT_DEFAULT
#  define LTL2FSM_NR_WHAT_DEFAULT 0
#endif 

////////////////////////////////////////////////////////////////////////////////

// derived config


#if LTL2FSM_HANDLE_TEST_EXCEPTION_EXPLICITLY == 1
#  include <diagnostics/unittest/test_exception.hpp>
#endif

#ifndef LTL2FSM_BASE_EXCEPTION_STR_WHAT
#  warning LTL2FSM_BASE_EXCEPTION_STR_WHAT not defined
#endif

////////////////////////////////////////////////////////////////////////////////

// AUDIT

#if LTL2FSM_DEBUG__LEVEL__ > 1

#  define LTL2FSM_AUDIT_TRACE(STR_WHAT) \
    DIAGNOSTICS_BASE_LOG(::diagnostics::LEVEL_AUDIT,::diagnostics::TYPE_TRACE,LTL2FSM_NR_WHAT_DEFAULT,(STR_WHAT))
#  define LTL2FSM_AUDIT_BINARY_TRACE(STR_WHAT) \
    DIAGNOSTICS_BASE_LOG(::diagnostics::LEVEL_AUDIT,::diagnostics::TYPE_TRACE_BINARY,LTL2FSM_NR_WHAT_DEFAULT,(STR_WHAT))

#  define LTL2FSM_AUDIT_BLOCK_ENTER(STR_WHAT)  \
    DIAGNOSTICS_BASE_BLOCK_ENTER(::diagnostics::LEVEL_AUDIT, \
                                 ::diagnostics::TYPE_BLOCK_ENTER, \
                                 ::diagnostics::TYPE_BLOCK_EXIT,LTL2FSM_NR_WHAT_DEFAULT,(STR_WHAT))
#  define LTL2FSM_AUDIT_BLOCK_EXIT DIAGNOSTICS_BASE_BLOCK_EXIT
#  define LTL2FSM_AUDIT_BLOCK_GUARD(STR_WHAT)  \
    DIAGNOSTICS_BASE_BLOCK_GUARD(::diagnostics::LEVEL_AUDIT, \
                                 ::diagnostics::TYPE_BLOCK_ENTER, \
                                 ::diagnostics::TYPE_BLOCK_EXIT,LTL2FSM_NR_WHAT_DEFAULT,(STR_WHAT))

#  define LTL2FSM_AUDIT_PROCEDURE_ENTER(STR_WHAT) \
    DIAGNOSTICS_BASE_BLOCK_ENTER(::diagnostics::LEVEL_AUDIT, \
                                 ::diagnostics::TYPE_PROCEDURE_ENTER, \
                                 ::diagnostics::TYPE_PROCEDURE_EXIT,LTL2FSM_NR_WHAT_DEFAULT, \
                                 ::std::string("PROCEDURE=\"") + DIAGNOSTICS_FUNC_NAME + "\" " + (STR_WHAT))
#  define LTL2FSM_AUDIT_PROCEDURE_EXIT DIAGNOSTICS_BASE_BLOCK_EXIT
#  define LTL2FSM_AUDIT_PROCEDURE_GUARD(STR_WHAT) \
    DIAGNOSTICS_BASE_BLOCK_GUARD(::diagnostics::LEVEL_AUDIT, \
                                 ::diagnostics::TYPE_PROCEDURE_ENTER, \
                                 ::diagnostics::TYPE_PROCEDURE_EXIT,LTL2FSM_NR_WHAT_DEFAULT, \
                                 ::std::string("PROCEDURE=\"") + DIAGNOSTICS_FUNC_NAME + "\" " + (STR_WHAT))

#  define LTL2FSM_AUDIT_METHOD_ENTER(STR_WHAT) \
    DIAGNOSTICS_BASE_BLOCK_ENTER(::diagnostics::LEVEL_AUDIT, \
                                 ::diagnostics::TYPE_METHOD_ENTER, \
                                 ::diagnostics::TYPE_METHOD_EXIT,LTL2FSM_NR_WHAT_DEFAULT, \
                                 ::std::string("MEHTOD=\"") + DIAGNOSTICS_FUNC_NAME + "\" " + (STR_WHAT))
#  define LTL2FSM_AUDIT_METHOD_EXIT DIAGNOSTICS_BASE_BLOCK_EXIT
#  define LTL2FSM_AUDIT_METHOD_GUARD(STR_WHAT) \
    DIAGNOSTICS_BASE_BLOCK_GUARD(::diagnostics::LEVEL_AUDIT, \
                                 ::diagnostics::TYPE_METHOD_ENTER, \
                                 ::diagnostics::TYPE_METHOD_EXIT,LTL2FSM_NR_WHAT_DEFAULT, \
                                 ::std::string("METHOD=\"") + DIAGNOSTICS_FUNC_NAME + "\" " + (STR_WHAT))

#  define LTL2FSM_AUDIT_ASSERT(NAME,COND) \
    DIAGNOSTICS_BASE_ASSERT(::diagnostics::LEVEL_AUDIT,NAME,\
                            LTL2FSM_NR_WHAT_DEFAULT,\
                            "CONDITION=\"" #COND "\" EXCEPTION=\"" #NAME "\"",\
                            (COND),\
                            LTL2FSM_FAILED_ASSERT_ACTION(::diagnostics::LEVEL_AUDIT,NAME,COND))
#  define LTL2FSM_AUDIT_ASSERT1(NAME,COND,EXCEP_WHAT) \
    DIAGNOSTICS_BASE_ASSERT(::diagnostics::LEVEL_AUDIT,NAME,\
                            LTL2FSM_NR_WHAT_DEFAULT,\
                            ::std::string("CONDITION=\"" #COND "\" EXCEPTION=\"" #NAME "\" WHAT=\"") + (EXCEP_WHAT) + "\"",\
                            (COND),\
                            LTL2FSM_FAILED_ASSERT_ACTION1(::diagnostics::LEVEL_AUDIT,NAME,COND,EXCEP_WHAT))

#  define LTL2FSM_AUDIT_ASSERT_LOG(NAME,COND) \
    DIAGNOSTICS_BASE_ASSERT(::diagnostics::LEVEL_AUDIT,NAME,\
                            LTL2FSM_NR_WHAT_DEFAULT,\
                            "CONDITION=\"" #COND "\" EXCEPTION=\"" #NAME "\"",\
                            (COND),\
                            ((void)0))
#  define LTL2FSM_AUDIT_ASSERT_LOG1(NAME,COND,EXCEP_WHAT) \
    DIAGNOSTICS_BASE_ASSERT(::diagnostics::LEVEL_AUDIT,NAME,\
                            LTL2FSM_NR_WHAT_DEFAULT,\
                            ::std::string("CONDITION=\"" #COND "\" EXCEPTION=\"" #NAME "\" WHAT=\"") + (EXCEP_WHAT) + "\"",\
                            (COND),\
                            ((void)0))


#  define LTL2FSM_AUDIT_CHECK(NAME,COND) \
    DIAGNOSTICS_BASE_CHECK(::diagnostics::LEVEL_AUDIT,NAME,\
                           LTL2FSM_NR_WHAT_DEFAULT,\
                           "CONDITION=\"" #COND "\" EXCEPTION=\"" #NAME "\"",\
                           (COND),\
                           LTL2FSM_FAILED_CHECK_ACTION(::diagnostics::LEVEL_AUDIT,NAME,COND))
#  define LTL2FSM_AUDIT_CHECK1(NAME,COND,STR_WHAT) \
    DIAGNOSTICS_BASE_CHECK(::diagnostics::LEVEL_AUDIT,NAME,\
                           LTL2FSM_NR_WHAT_DEFAULT,\
                           ::std::string("CONDITION=\"" #COND "\" EXCEPTION=\"" #NAME "\" WHAT=\"") + (STR_WHAT) + "\"",\
                           (COND),\
                           LTL2FSM_FAILED_CHECK_ACTION1(::diagnostics::LEVEL_AUDIT,NAME,COND,STR_WHAT))

#  define LTL2FSM_AUDIT_CLASS_THROWING_INVARIANCE_ENTER DIAGNOSTICS_BASE_CLASS_THROWING_INVARIANCE_ENTER
#  define LTL2FSM_AUDIT_CLASS_THROWING_INVARIANCE_EXIT  \
    DIAGNOSTICS_BASE_CLASS_THROWING_INVARIANCE_EXIT(::diagnostics::LEVEL_AUDIT,\
                                           LTL2FSM_NR_WHAT_DEFAULT, \
                                           LTL2FSM_HANDLE_TEST_EXCEPTION_EXPLICITLY,\
                                           LTL2FSM_BASE_EXCEPTION_TYPE,\
                                           LTL2FSM_BASE_EXCEPTION_STR_WHAT)

#  define LTL2FSM_AUDIT_CLASS_INVARIANCE_GUARD DIAGNOSTICS_BASE_CLASS_INVARIANCE_GUARD
#  define LTL2FSM_AUDIT_CLASS_INVARIANCE_ENTER DIAGNOSTICS_BASE_CLASS_INVARIANCE_ENTER
#  define LTL2FSM_AUDIT_CLASS_INVARIANCE_EXIT  DIAGNOSTICS_BASE_CLASS_INVARIANCE_EXIT

#  define LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT  DIAGNOSTICS_BASE_CLASS_INVARIANCE_ASSERT

#  define LTL2FSM_AUDIT_EXCEPTIONLESS_BLOCK_ENTER  DIAGNOSTICS_BASE_EXCEPTIONLESS_BLOCK_ENTER
#  define LTL2FSM_AUDIT_EXCEPTIONLESS_BLOCK_EXIT  \
    DIAGNOSTICS_BASE_EXCEPTIONLESS_BLOCK_EXIT(::diagnostics::LEVEL_AUDIT,\
                                              LTL2FSM_NR_WHAT_DEFAULT, \
                                              true,\
                                              LTL2FSM_HANDLE_TEST_EXCEPTION_EXPLICITLY,\
                                              LTL2FSM_BASE_EXCEPTION_TYPE,\
                                              LTL2FSM_BASE_EXCEPTION_STR_WHAT)

#else

#  define LTL2FSM_AUDIT_TRACE(STR_WHAT)        ((void)0)
#  define LTL2FSM_AUDIT_BINARY_TRACE(STR_WHAT) ((void)0)

#  define LTL2FSM_AUDIT_BLOCK_ENTER(STR_WHAT) do {
#  define LTL2FSM_AUDIT_BLOCK_EXIT            } while(false)
#  define LTL2FSM_AUDIT_BLOCK_GUARD(STR_WHAT) ((void)0)

#  define LTL2FSM_AUDIT_PROCEDURE_ENTER(STR_WHAT) do {
#  define LTL2FSM_AUDIT_PROCEDURE_EXIT            } while(false)
#  define LTL2FSM_AUDIT_PROCEDURE_GUARD(STR_WHAT) ((void)0)

#  define LTL2FSM_AUDIT_METHOD_ENTER(STR_WHAT) do {
#  define LTL2FSM_AUDIT_METHOD_EXIT            } while(false)
#  define LTL2FSM_AUDIT_METHOD_GUARD(STR_WHAT) ((void)0)

#  define LTL2FSM_AUDIT_ASSERT(NAME,COND)           ((void)0)
#  define LTL2FSM_AUDIT_ASSERT1(NAME,COND,STR_WHAT) ((void)0)
#  define LTL2FSM_AUDIT_CHECK(NAME,COND)            ((void)0)
#  define LTL2FSM_AUDIT_CHECK1(NAME,COND,STR_WHAT)  ((void)0)

#  define LTL2FSM_AUDIT_CLASS_THROWING_INVARIANCE_ENTER do {
#  define LTL2FSM_AUDIT_CLASS_THROWING_INVARIANCE_EXIT  } while(false)

#  define LTL2FSM_AUDIT_CLASS_INVARIANCE_GUARD ((void)0)
#  define LTL2FSM_AUDIT_CLASS_INVARIANCE_ENTER do {
#  define LTL2FSM_AUDIT_CLASS_INVARIANCE_EXIT  } while(false)

#  define LTL2FSM_AUDIT_CLASS_INVARIANCE_ASSERT  ((void)0)


#  define LTL2FSM_AUDIT_EXCEPTIONLESS_BLOCK_ENTER do {
#  define LTL2FSM_AUDIT_EXCEPTIONLESS_BLOCK_EXIT  } while(false)

#endif


////////////////////////////////////////////////////////////////////////////////

#if LTL2FSM_DEBUG__LEVEL__ > 0

#  define LTL2FSM_DEBUG_TRACE(STR_WHAT) \
    DIAGNOSTICS_BASE_LOG(::diagnostics::LEVEL_DEBUG,::diagnostics::TYPE_TRACE,LTL2FSM_NR_WHAT_DEFAULT,(STR_WHAT))
#  define LTL2FSM_DEBUG_BINARY_TRACE(STR_WHAT) \
    DIAGNOSTICS_BASE_LOG(::diagnostics::LEVEL_DEBUG,::diagnostics::TYPE_TRACE_BINARY,LTL2FSM_NR_WHAT_DEFAULT,(STR_WHAT))

#  define LTL2FSM_DEBUG_BLOCK_ENTER(STR_WHAT)  \
    DIAGNOSTICS_BASE_BLOCK_ENTER(::diagnostics::LEVEL_DEBUG, \
                                 ::diagnostics::TYPE_BLOCK_ENTER, \
                                 ::diagnostics::TYPE_BLOCK_EXIT,LTL2FSM_NR_WHAT_DEFAULT,(STR_WHAT))
#  define LTL2FSM_DEBUG_BLOCK_EXIT DIAGNOSTICS_BASE_BLOCK_EXIT
#  define LTL2FSM_DEBUG_BLOCK_GUARD(STR_WHAT)  \
    DIAGNOSTICS_BASE_BLOCK_GUARD(::diagnostics::LEVEL_DEBUG, \
                                 ::diagnostics::TYPE_BLOCK_ENTER, \
                                 ::diagnostics::TYPE_BLOCK_EXIT,LTL2FSM_NR_WHAT_DEFAULT,(STR_WHAT))

#  define LTL2FSM_DEBUG_PROCEDURE_ENTER(STR_WHAT) \
    DIAGNOSTICS_BASE_BLOCK_ENTER(::diagnostics::LEVEL_DEBUG, \
                                 ::diagnostics::TYPE_PROCEDURE_ENTER, \
                                 ::diagnostics::TYPE_PROCEDURE_EXIT,LTL2FSM_NR_WHAT_DEFAULT, \
                                 ::std::string("PROCEDURE=\"" DIAGNOSTICS_FUNC_NAME "\" ") + (STR_WHAT))
#  define LTL2FSM_DEBUG_PROCEDURE_EXIT DIAGNOSTICS_BASE_BLOCK_EXIT
#  define LTL2FSM_DEBUG_PROCEDURE_GUARD(STR_WHAT) \
    DIAGNOSTICS_BASE_BLOCK_GUARD(::diagnostics::LEVEL_DEBUG, \
                                 ::diagnostics::TYPE_PROCEDURE_ENTER, \
                                 ::diagnostics::TYPE_PROCEDURE_EXIT,LTL2FSM_NR_WHAT_DEFAULT, \
                                 ::std::string("PROCEDURE=\"" DIAGNOSTICS_FUNC_NAME "\" ") + (STR_WHAT))

#  define LTL2FSM_DEBUG_METHOD_ENTER(STR_WHAT) \
    DIAGNOSTICS_BASE_BLOCK_ENTER(::diagnostics::LEVEL_DEBUG, \
                                 ::diagnostics::TYPE_METHOD_ENTER, \
                                 ::diagnostics::TYPE_METHOD_EXIT,LTL2FSM_NR_WHAT_DEFAULT, \
                                 ::std::string("MEHTOD=\"" DIAGNOSTICS_FUNC_NAME "\" ") + (STR_WHAT))
#  define LTL2FSM_DEBUG_METHOD_EXIT DIAGNOSTICS_BASE_BLOCK_EXIT
#  define LTL2FSM_DEBUG_METHOD_GUARD(STR_WHAT) \
    DIAGNOSTICS_BASE_BLOCK_GUARD(::diagnostics::LEVEL_DEBUG, \
                                 ::diagnostics::TYPE_METHOD_ENTER, \
                                 ::diagnostics::TYPE_METHOD_EXIT,LTL2FSM_NR_WHAT_DEFAULT, \
                                 ::std::string("METHOD=\"" DIAGNOSTICS_FUNC_NAME "\" ") + (STR_WHAT))

#  define LTL2FSM_DEBUG_ASSERT(NAME,COND) \
    DIAGNOSTICS_BASE_ASSERT(::diagnostics::LEVEL_DEBUG,NAME,\
                            LTL2FSM_NR_WHAT_DEFAULT,\
                            "CONDITION=\"" #COND "\" EXCEPTION=\"" #NAME "\"",\
                            (COND),\
                            LTL2FSM_FAILED_ASSERT_ACTION(::diagnostics::LEVEL_DEBUG,NAME,COND))
#  define LTL2FSM_DEBUG_ASSERT1(NAME,COND,STR_WHAT) \
    DIAGNOSTICS_BASE_ASSERT(::diagnostics::LEVEL_DEBUG,NAME,\
                            LTL2FSM_NR_WHAT_DEFAULT,\
                            ::std::string("CONDITION=\"" #COND "\" EXCEPTION=\"" #NAME "\" WHAT=\"") + (STR_WHAT) + "\"",\
                            (COND),\
                            LTL2FSM_FAILED_ASSERT_ACTION1(::diagnostics::LEVEL_DEBUG,NAME,COND,STR_WHAT))
#  define LTL2FSM_DEBUG_CHECK(NAME,COND) \
    DIAGNOSTICS_BASE_CHECK(::diagnostics::LEVEL_DEBUG,NAME,\
                           LTL2FSM_NR_WHAT_DEFAULT,\
                           "CONDITION=\"" #COND "\" EXCEPTION=\"" #NAME "\"",\
                           (COND),\
                           LTL2FSM_FAILED_CHECK_ACTION(::diagnostics::LEVEL_DEBUG,NAME,COND))
#  define LTL2FSM_DEBUG_CHECK1(NAME,COND,STR_WHAT) \
    DIAGNOSTICS_BASE_CHECK(::diagnostics::LEVEL_DEBUG,NAME,\
                           LTL2FSM_NR_WHAT_DEFAULT,\
                           ::std::string("CONDITION=\"" #COND "\" EXCEPTION=\"" #NAME "\" WHAT=\"") + (STR_WHAT) + "\"",\
                           (COND),\
                           LTL2FSM_FAILED_CHECK_ACTION1(::diagnostics::LEVEL_DEBUG,NAME,COND,STR_WHAT))

#  define LTL2FSM_DEBUG_CLASS_THROWING_INVARIANCE_ENTER DIAGNOSTICS_BASE_CLASS_THROWING_INVARIANCE_ENTER
#  define LTL2FSM_DEBUG_CLASS_THROWING_INVARIANCE_EXIT  \
    DIAGNOSTICS_BASE_CLASS_THROWING_INVARIANCE_EXIT(::diagnostics::LEVEL_DEBUG,\
                                           LTL2FSM_NR_WHAT_DEFAULT, \
                                           LTL2FSM_HANDLE_TEST_EXCEPTION_EXPLICITLY,\
                                           LTL2FSM_BASE_EXCEPTION_TYPE,\
                                           LTL2FSM_BASE_EXCEPTION_STR_WHAT)

#  define LTL2FSM_DEBUG_CLASS_INVARIANCE_GUARD DIAGNOSTICS_BASE_CLASS_INVARIANCE_GUARD
#  define LTL2FSM_DEBUG_CLASS_INVARIANCE_ENTER DIAGNOSTICS_BASE_CLASS_INVARIANCE_ENTER
#  define LTL2FSM_DEBUG_CLASS_INVARIANCE_EXIT  DIAGNOSTICS_BASE_CLASS_INVARIANCE_EXIT

#  define LTL2FSM_DEBUG_CLASS_INVARIANCE_ASSERT DIAGNOSTICS_BASE_CLASS_INVARIANCE_ASSERT


#  define LTL2FSM_DEBUG_EXCEPTIONLESS_BLOCK_ENTER  DIAGNOSTICS_BASE_EXCEPTIONLESS_BLOCK_ENTER
#  define LTL2FSM_DEBUG_EXCEPTIONLESS_BLOCK_EXIT  \
    DIAGNOSTICS_BASE_EXCEPTIONLESS_BLOCK_EXIT(::diagnostics::LEVEL_DEBUG,\
                                              LTL2FSM_NR_WHAT_DEFAULT, \
                                              true,\
                                              LTL2FSM_HANDLE_TEST_EXCEPTION_EXPLICITLY,\
                                              LTL2FSM_BASE_EXCEPTION_TYPE,\
                                              LTL2FSM_BASE_EXCEPTION_STR_WHAT)

#else

#  define LTL2FSM_DEBUG_TRACE(STR_WHAT)        ((void)0)
#  define LTL2FSM_DEBUG_BINARY_TRACE(STR_WHAT) ((void)0)

#  define LTL2FSM_DEBUG_BLOCK_ENTER(STR_WHAT) do {
#  define LTL2FSM_DEBUG_BLOCK_EXIT            } while(false)
#  define LTL2FSM_DEBUG_BLOCK_GUARD(STR_WHAT) ((void)0)

#  define LTL2FSM_DEBUG_PROCEDURE_ENTER(STR_WHAT) do {
#  define LTL2FSM_DEBUG_PROCEDURE_EXIT            } while(false)
#  define LTL2FSM_DEBUG_PROCEDURE_GUARD(STR_WHAT) ((void)0)

#  define LTL2FSM_DEBUG_METHOD_ENTER(STR_WHAT) do {
#  define LTL2FSM_DEBUG_METHOD_EXIT            } while(false)
#  define LTL2FSM_DEBUG_METHOD_GUARD(STR_WHAT) ((void)0)

#  define LTL2FSM_DEBUG_ASSERT(NAME,COND)           ((void)0)
#  define LTL2FSM_DEBUG_ASSERT1(NAME,COND,STR_WHAT) ((void)0)
#  define LTL2FSM_DEBUG_CHECK(NAME,COND)            ((void)0)
#  define LTL2FSM_DEBUG_CHECK1(NAME,COND,STR_WHAT)  ((void)0)

#  define LTL2FSM_DEBUG_CLASS_THROWING_INVARIANCE_ENTER do {
#  define LTL2FSM_DEBUG_CLASS_THROWING_INVARIANCE_EXIT  } while(false)

#  define LTL2FSM_DEBUG_CLASS_INVARIANCE_GUARD ((void)0)
#  define LTL2FSM_DEBUG_CLASS_INVARIANCE_ENTER do {
#  define LTL2FSM_DEBUG_CLASS_INVARIANCE_EXIT  } while(false)

#  define LTL2FSM_DEBUG_CLASS_INVARIANCE_ASSERT  ((void)0)


#  define LTL2FSM_DEBUG_EXCEPTIONLESS_BLOCK_ENTER do {
#  define LTL2FSM_DEBUG_EXCEPTIONLESS_BLOCK_EXIT  } while(false)

#endif


////////////////////////////////////////////////////////////////////////////////


#define LTL2FSM_PROD_TRACE(STR_WHAT) \
    DIAGNOSTICS_BASE_LOG(::diagnostics::LEVEL_PROD,::diagnostics::TYPE_TRACE,LTL2FSM_NR_WHAT_DEFAULT,(STR_WHAT))
#define LTL2FSM_PROD_BINARY_TRACE(STR_WHAT) \
    DIAGNOSTICS_BASE_LOG(::diagnostics::LEVEL_PROD,::diagnostics::TYPE_TRACE_BINARY,LTL2FSM_NR_WHAT_DEFAULT,(STR_WHAT))

#define LTL2FSM_PROD_TRACE1(NR_WHAT,STR_WHAT) \
    DIAGNOSTICS_BASE_LOG(::diagnostics::LEVEL_PROD,::diagnostics::TYPE_TRACE,(NR_WHAT),(STR_WHAT))
#define LTL2FSM_PROD_BINARY_TRACE1(NR_WHAT,STR_WHAT) \
    DIAGNOSTICS_BASE_LOG(::diagnostics::LEVEL_PROD,::diagnostics::TYPE_TRACE_BINARY,(NR_WHAT),(STR_WHAT))


#define LTL2FSM_PROD_BLOCK_ENTER(STR_WHAT)  \
    DIAGNOSTICS_BASE_BLOCK_ENTER(::diagnostics::LEVEL_PROD, \
                                 ::diagnostics::TYPE_BLOCK_ENTER, \
                                 ::diagnostics::TYPE_BLOCK_EXIT,LTL2FSM_NR_WHAT_DEFAULT,(STR_WHAT))
#define LTL2FSM_PROD_BLOCK_EXIT DIAGNOSTICS_BASE_BLOCK_EXIT
#define LTL2FSM_PROD_BLOCK_GUARD(STR_WHAT)  \
    DIAGNOSTICS_BASE_BLOCK_GUARD(::diagnostics::LEVEL_PROD, \
                                 ::diagnostics::TYPE_BLOCK_ENTER, \
                                 ::diagnostics::TYPE_BLOCK_EXIT,LTL2FSM_NR_WHAT_DEFAULT,(STR_WHAT))

#define LTL2FSM_PROD_PROCEDURE_ENTER(STR_WHAT) \
    DIAGNOSTICS_BASE_BLOCK_ENTER(::diagnostics::LEVEL_PROD, \
                                 ::diagnostics::TYPE_PROCEDURE_ENTER, \
                                 ::diagnostics::TYPE_PROCEDURE_EXIT,LTL2FSM_NR_WHAT_DEFAULT, \
                                 ::std::string("PROCEDURE=\"" DIAGNOSTICS_FUNC_NAME "\" ") + (STR_WHAT))
#define LTL2FSM_PROD_PROCEDURE_EXIT DIAGNOSTICS_BASE_BLOCK_EXIT
#define LTL2FSM_PROD_PROCEDURE_GUARD(STR_WHAT) \
    DIAGNOSTICS_BASE_BLOCK_GUARD(::diagnostics::LEVEL_PROD, \
                                 ::diagnostics::TYPE_PROCEDURE_ENTER, \
                                 ::diagnostics::TYPE_PROCEDURE_EXIT,LTL2FSM_NR_WHAT_DEFAULT, \
                                 ::std::string("PROCEDURE=\"" DIAGNOSTICS_FUNC_NAME "\" ") + (STR_WHAT))

#define LTL2FSM_PROD_METHOD_ENTER(STR_WHAT) \
    DIAGNOSTICS_BASE_BLOCK_ENTER(::diagnostics::LEVEL_PROD, \
                                 ::diagnostics::TYPE_METHOD_ENTER, \
                                 ::diagnostics::TYPE_METHOD_EXIT,LTL2FSM_NR_WHAT_DEFAULT, \
                                 ::std::string("MEHTOD=\"" DIAGNOSTICS_FUNC_NAME "\" ") + (STR_WHAT))
#define LTL2FSM_PROD_METHOD_EXIT DIAGNOSTICS_BASE_BLOCK_EXIT
#define LTL2FSM_PROD_METHOD_GUARD(STR_WHAT) \
    DIAGNOSTICS_BASE_BLOCK_GUARD(::diagnostics::LEVEL_PROD, \
                                 ::diagnostics::TYPE_METHOD_ENTER, \
                                 ::diagnostics::TYPE_METHOD_EXIT,LTL2FSM_NR_WHAT_DEFAULT, \
                                 ::std::string("METHOD=\"" DIAGNOSTICS_FUNC_NAME "\" ") + (STR_WHAT))

#define LTL2FSM_PROD_ASSERT(NAME,COND) \
    DIAGNOSTICS_BASE_ASSERT(::diagnostics::LEVEL_PROD,NAME,\
                            LTL2FSM_NR_WHAT_DEFAULT,\
                            "CONDITION=\"" #COND "\" EXCEPTION=\"" #NAME "\"",\
                            (COND),\
                            LTL2FSM_FAILED_ASSERT_ACTION(::diagnostics::LEVEL_PROD,NAME,COND))
#define LTL2FSM_PROD_ASSERT1(NAME,COND,STR_WHAT) \
    DIAGNOSTICS_BASE_ASSERT(::diagnostics::LEVEL_PROD,NAME,\
                            LTL2FSM_NR_WHAT_DEFAULT,\
                            ::std::string("CONDITION=\"" #COND "\" EXCEPTION=\"" #NAME "\" WHAT=\"") + (STR_WHAT) + "\"",\
                            (COND),\
                            LTL2FSM_FAILED_ASSERT_ACTION1(::diagnostics::LEVEL_PROD,NAME,COND,STR_WHAT))

#define LTL2FSM_PROD_CLASS_THROWING_INVARIANCE_ENTER DIAGNOSTICS_BASE_CLASS_THROWING_INVARIANCE_ENTER
#define LTL2FSM_PROD_CLASS_THROWING_INVARIANCE_EXIT  \
    DIAGNOSTICS_BASE_CLASS_THROWING_INVARIANCE_EXIT(::diagnostics::LEVEL_PROD,\
                                           LTL2FSM_NR_WHAT_DEFAULT, \
                                           LTL2FSM_HANDLE_TEST_EXCEPTION_EXPLICITLY,\
                                           LTL2FSM_BASE_EXCEPTION_TYPE,\
                                           LTL2FSM_BASE_EXCEPTION_STR_WHAT)

#define LTL2FSM_PROD_CLASS_INVARIANCE_GUARD DIAGNOSTICS_BASE_CLASS_INVARIANCE_GUARD
#define LTL2FSM_PROD_CLASS_INVARIANCE_ENTER DIAGNOSTICS_BASE_CLASS_INVARIANCE_ENTER
#define LTL2FSM_PROD_CLASS_INVARIANCE_EXIT  DIAGNOSTICS_BASE_CLASS_INVARIANCE_EXIT

#define LTL2FSM_PROD_CLASS_INVARIANCE_ASSERT  DIAGNOSTICS_BASE_CLASS_INVARIANCE_ASSERT

#define LTL2FSM_PROD_EXCEPTIONLESS_BLOCK_ENTER  DIAGNOSTICS_BASE_EXCEPTIONLESS_BLOCK_ENTER
#define LTL2FSM_PROD_EXCEPTIONLESS_BLOCK_EXIT  \
    DIAGNOSTICS_BASE_EXCEPTIONLESS_BLOCK_EXIT(::diagnostics::LEVEL_PROD,\
                                              LTL2FSM_NR_WHAT_DEFAULT, \
                                              true,\
                                              LTL2FSM_HANDLE_TEST_EXCEPTION_EXPLICITLY,\
                                              LTL2FSM_BASE_EXCEPTION_TYPE,\
                                              LTL2FSM_BASE_EXCEPTION_STR_WHAT)


#if LTL2FSM_DEBUG__LEVEL__ > 0

#  define LTL2FSM_PROD_CHECK(NAME,COND) \
    DIAGNOSTICS_BASE_CHECK(::diagnostics::LEVEL_PROD,NAME,\
                           LTL2FSM_NR_WHAT_DEFAULT,\
                           "CONDITION=\"" #COND "\" EXCEPTION=\"" #NAME "\"",\
                           (COND),\
                           LTL2FSM_FAILED_CHECK_ACTION(::diagnostics::LEVEL_PROD,NAME,COND))
#  define LTL2FSM_PROD_CHECK1(NAME,COND,STR_WHAT) \
    DIAGNOSTICS_BASE_CHECK(::diagnostics::LEVEL_PROD,NAME,\
                           LTL2FSM_NR_WHAT_DEFAULT,\
                           ::std::string("CONDITION=\"" #COND "\" EXCEPTION=\"" #NAME "\" WHAT=\"") + (STR_WHAT) + "\"",\
                           (COND),\
                           LTL2FSM_FAILED_CHECK_ACTION1(::diagnostics::LEVEL_PROD,NAME,COND,STR_WHAT))

#else

#  define LTL2FSM_PROD_CHECK(NAME,COND) \
    do { if(!(COND)) { LTL2FSM_FAILED_CHECK_ACTION(::diagnostics::LEVEL_PROD,NAME,COND); } } while(false)
#  define LTL2FSM_PROD_CHECK1(NAME,COND,STR_WHAT) \
    do { if(!(COND)) { LTL2FSM_FAILED_CHECK_ACTION1(::diagnostics::LEVEL_PROD,NAME,COND,STR_WHAT); } } while(false)

#endif

#define LTL2FSM_METHOD_GUARD(STR_WHAT) LTL2FSM_AUDIT_METHOD_GUARD(STR_WHAT); LTL2FSM_AUDIT_CLASS_INVARIANCE_GUARD 


#define LTL2FSM_AUDIT_ASSERT_RELATION(NAME,OP1,REL,OP2) \
       DIAGNOSTICS_RELATION(LTL2FSM_AUDIT_ASSERT1,NAME,OP1,REL,OP2)
#define LTL2FSM_AUDIT_CHECK_RELATION(NAME,OP1,REL,OP2) \
       DIAGNOSTICS_RELATION(LTL2FSM_AUDIT_CHECK1,NAME,OP1,REL,OP2)

#define LTL2FSM_DEBUG_ASSERT_RELATION(NAME,OP1,REL,OP2) \
       DIAGNOSTICS_RELATION(LTL2FSM_DEBUG_ASSERT1,NAME,OP1,REL,OP2)
#define LTL2FSM_DEBUG_CHECK_RELATION(NAME,OP1,REL,OP2) \
       DIAGNOSTICS_RELATION(LTL2FSM_DEBUG_CHECK1,NAME,OP1,REL,OP2)

#define LTL2FSM_PROD_ASSERT_RELATION(NAME,OP1,REL,OP2) \
       DIAGNOSTICS_RELATION(LTL2FSM_PROD_ASSERT1,NAME,OP1,REL,OP2)
#define LTL2FSM_PROD_CHECK_RELATION(NAME,OP1,REL,OP2) \
       DIAGNOSTICS_RELATION(LTL2FSM_PROD_CHECK1,NAME,OP1,REL,OP2)


#endif

// vim:ts=4:sw=4
