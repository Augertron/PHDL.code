#ifndef phdl__Error__header
#define phdl__Error__header

// Rather than completely abstract the few features that Boost Exception gives
// us, we are instead explicitly declaring Boost Exception a public part of our
// interface. This means that direct use of boost::error_info tag definitions,
// BOOST_THROW_EXCEPTION, and boost::diagnostic_information is allowed.
#include <boost/throw_exception.hpp>

namespace phdl {

	// We inherit from boost::exception to enable the use of generic error info
	// annotation. This feature is of primary value when debugging rather than
	// normal program operation, but can occassionally be of use for generating
	// high-quality error messages for end users.
	//
	// We also inherit from std::exception both because this is a requirement
	// of Boost Exception, as well as in order to benefit in scenerios where
	// our errors fall-through normal handling, but a generic std::exception is
	// handled at least somewhat intelligently.
	struct Error : boost::exception, std::exception {
		virtual const char *what() const noexcept override final;
	};

}

#endif
