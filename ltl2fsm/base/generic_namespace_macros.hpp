/**
 * @file generic_namespace_macros.hpp
 *
 * @author Oliver Arafat
 *
 * @brief Provides @ref LTL2FSM__BASE__GENERIC_BEGIN_NAMESPACE and
 * @ref LTL2FSM__BASE__GENERIC_END_NAMESPACE
 *
 * These two macros are used to declare new namespace marocs which
 * enforce correct usage as far as possible. More precisely, they
 * enforce that
 * @arg a semi-colon is placed after them, 
 * @arg their usages form proper nesting.
 *
 * The latter means that
 *
 * LTL2FSM__BASE__GENERIC_BEGIN_NAMESPACE(NS1); LTL2FSM__BASE__GENERIC_BEGIN_NAMESPACE(NS2); 
 *
 * must be followed by 
 *
 * LTL2FSM__BASE__GENERIC_END_NAMESPACE(NS2); LTL2FSM__BASE__GENERIC_END_NAMESPACE(NS1); 
 *
 * Missing one of the ending macros, adding another one, or changing
 * their order is mistake that will lead to a compile time error.
 *
 * Currently, these macros do not report an error if somebody would use 
 *
 * LTL2FSM__BASE__GENERIC_END_NAMESPACE(NS1); LTL2FSM__BASE__GENERIC_END_NAMESPACE(NS1); 
 * 
 * in the example above.
 *
 *
 * @note DOCUMENTED -- NO TEST
 */

#ifndef LTL2FSM__BASE__GENERIC_NAMESPACE_MACROS__HPP
#define LTL2FSM__BASE__GENERIC_NAMESPACE_MACROS__HPP


/**
 * @brief Opens a namesapce of with name @a NAME. Requires that the
 * macros instantiation is terminated by a semi-colon and that a
 * corresponding instantiation of @ref
 * LTL2FSM__BASE__GENERIC_END_NAMESPACE(@a NAME) is used.
 */ 
#define LTL2FSM__BASE__GENERIC_BEGIN_NAMESPACE(NAME) \
namespace NAME { namespace NS__USELESS__NS__ ## NAME {typedef int NS__USELESS__ ## NAME;} \
void NS__USELESS_FUN1__ ## NAME (int)

/**
 * @brief See @ref LTL2FSM__BASE__GENERIC_BEGIN_NAMESPACE
 */  
#define LTL2FSM__BASE__GENERIC_END_NAMESPACE(NAME) \
void NS__USELESS_FUN2__ ## NAME (NS__USELESS__NS__ ## NAME :: NS__USELESS__ ## NAME); } \
void NS__USELESS_FUN_NEVER_USE_THIS (int)

#endif

