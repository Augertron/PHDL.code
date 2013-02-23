#ifndef phdl__position__header
#define phdl__position__header

// standard headers
#include <string>

// phdl headers
#include <phdl/unicode.h++>

namespace phdl { namespace position {

	// Transform position into line and column numbers as well as content and
	// pointer strings for use primarily in error reporting.
	size_t      line_number  (const Characters &text, size_t position);
	size_t      column_number(const Characters &text, size_t position);
	std::string line_content (const Characters &text, size_t position);
	std::string line_pointer (const Characters &text, size_t position);

}}

#endif
