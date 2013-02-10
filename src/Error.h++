#ifndef phdl__Error__header
#define phdl__Error__header

// For std::exception
#include <exception>

// Boost Exception is explicitly a public part of our error handling scheme.
// Features such as boost::error_info tag definitions, BOOST_THROW_EXCEPTION
// and boost::diagnostic_information are expected to be used directly.
#include <boost/throw_exception.hpp>

namespace phdl {

	// We inherit from boost::exception to enable the use of generic error info
	// annotation. This feature is of primary value when debugging rather than
	// normal program operation, but can occassionally be of use for generating
	// high-quality error messages for end users.
	//
	// We also inherit from std::exception in order to benefit in accidental
	// scenerios where our errors fall-through normal handling, but generic
	// std::exceptions are handled at least somewhat intelligently.
	struct Error : boost::exception, std::exception {
		virtual const char *what() const noexcept override final;
	};

}

#endif
