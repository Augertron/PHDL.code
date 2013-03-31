// Dynamic Assert -- dynamic assertion macro
// by Wesley J. Landaker <wjl@icecavern.net>
// =========================================
// Version 1.0.0
//
// The dynamic_assert macro is a wrapper for the standard assert macro that
// gives a nicer, more consistent interface.
//
// This gives the same interface as static_assert, and also forces the message
// to be given as a string literal, to ensure that the message is actually
// available and human-readable when the assert triggers.
//
// Copyright & License Information
// -------------------------------
//
// Copyright © 2013 Wesley J. Landaker
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// this program. If not, see <http://www.gnu.org/licenses/>.

#ifndef dynamic_assert__header__
#define dynamic_assert__header__

#include <cassert>

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
