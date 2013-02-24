#include <phdl/error.h++>

#include <typeinfo>

namespace phdl { namespace error {

	// In normal circumstances, this function will never be called. Thus, we
	// don't need anything special here, just enough to identify the exception
	// in a debugging scenerio. This means we can just use the output from
	// std::type_info::name with no worries about its human-readability.
	const char *Error::what() const noexcept {
		return typeid(*this).name();
	}

}}
