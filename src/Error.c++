#include "Error.h++"

#include <typeinfo>

namespace phdl {

	const char *Error::what() const noexcept {
		return typeid(*this).name();
	}

}
