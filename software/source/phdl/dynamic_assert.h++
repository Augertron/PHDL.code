#ifndef __dynamic_assert__header__
#define __dynamic_assert__header__

#include <cassert>

// Wrapper for assert to give a nicer, more consistent interface.
//
// This gives the same interface as static_assert, and also forces the message
// to be given as a string literal, to ensure that the message is actually
// available and human-readable when the assert triggers.
#define dynamic_assert(test, message) \
	static_assert(\
		(\
			 std::is_convertible      <decltype(message), const char *>::value &&\
			!std::is_rvalue_reference <decltype(message)>::value &&\
			!std::is_pointer          <decltype(message)>::value &&\
			!std::is_array            <decltype(message)>::value &&\
			!std::is_class            <decltype(message)>::value\
		),\
		"string literal required"\
	);\
	assert((test) && message)

#endif
