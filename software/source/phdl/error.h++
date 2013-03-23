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

// We use boost::optional and boost::recursive_wrapper from boost::varient for
// handling our optional arguments.
#include <boost/optional.hpp>
#include <boost/variant.hpp>

// We need the unicode Characters type using in our user-visible errors.
#include <phdl/unicode.h++>

// Wrapper for assert to give a nicer, more consistent interface.
#include <phdl/dynamic_assert.h++>

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
		virtual ~Error() throw() = default;
		protected:
		Error() = default;
	};

	// Error severity. These are used primarily by User_Visible_Error, but are
	// declared outside of that class both to simplify the usage syntax, and
	// because this severity information is semantically independent.
	enum class Severity { Error, Warning, Debug, Context };

	// All user-visible errors must contain information about the location of
	// the error in the user's source files in order to be able to generate
	// consistent and helpful error messages.
	//
	// Each error can optionally contain another wrapped User_Visible_Error.
	// The error (and any deeper down the chain) is presented to the user
	// before the current error, because we assume that is the most likely
	// order of cause and effect.
	struct User_Visible_Error : virtual Error {
		User_Visible_Error (
			Severity severity,
			const std::string &filename,
			std::shared_ptr<Characters> text,
			size_t position,
			const std::string &message,
			boost::optional<const User_Visible_Error &> wrapped = boost::none
		);
		virtual ~User_Visible_Error() throw();

		// Stitches the file name & position with the given message in a
		// unified format intended to be directly useable by the end user.
		friend std::ostream &operator<<(std::ostream &, const User_Visible_Error &);

		private:
		Severity _severity;
		std::string _filename;
		std::shared_ptr<Characters> _text;
		size_t _position;
		std::string _message;
		boost::optional<boost::recursive_wrapper<User_Visible_Error>> _wrapped;
	};

}}

namespace phdl {
	// We use errors everywhere, so bring them into our main namespace.
	using phdl::error::Error;
	using phdl::error::User_Visible_Error;
}

#endif
