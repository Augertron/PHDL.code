#ifndef phdl__error__header
#define phdl__error__header

// Standard headers
#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>

// Rather than completely abstract the few features that Boost Exception gives
// us, we are instead explicitly declaring Boost Exception a public part of our
// interface. This means that direct use of boost::error_info tag definitions,
// BOOST_THROW_EXCEPTION, and boost::diagnostic_information is allowed.
#include <boost/exception/all.hpp>
#include <boost/throw_exception.hpp>

// Wrapper for assert to give a nicer, more consistent interface.
//
// This gives the same interface as static_assert, and also forces the message
// to be given as a string literal, to ensure that the message is actually
// available and human-readable when the assert triggers.
#define phdl_assert(test, message) \
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

namespace phdl { namespace error {

	// We inherit from boost::exception to enable the use of generic error info
	// annotation. This feature is of primary value when debugging rather than
	// normal program operation, but can occassionally be of use for generating
	// high-quality error messages for end users.
	//
	// We also inherit from std::exception both because this is a requirement
	// of Boost Exception, as well as in order to benefit in scenerios where
	// our errors fall-through normal handling, but a generic std::exception is
	// handled at least somewhat intelligently.
	struct Error : virtual boost::exception, virtual std::exception {
		virtual const char *what() const noexcept override final;
		protected:
		Error() = default;
	};

	// Error severity. These are used primarily by User_Visible_Error, but are
	// declared outside of that class to simplify the usage syntax.
	enum class Severity { Error, Warning, Debug };

	// All user-visible errors must contain information about the location of
	// the error in the user's source files in order to be able to generate
	// consistent and helpful error messages.
	struct User_Visible_Error : virtual Error {
		User_Visible_Error (
			Severity severity,
			const std::string &file_name,
			size_t line_number,
			size_t column_number,
			const std::string &message
		);

		// Stitches the file name & position with the given message in a
		// unified format intended to be directly useable by the end user.
		friend std::ostream &operator<<(std::ostream &, const User_Visible_Error &);

		private:
		Severity _severity;
		std::string _file_name;
		size_t _line_number;
		size_t _column_number;
		std::string _message;
	};

}}

namespace phdl {
	// We use errors everywhere, so bring them into our main namespace.
	using phdl::error::Error;
	using phdl::error::User_Visible_Error;
}

#endif
