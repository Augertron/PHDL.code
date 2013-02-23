#include <phdl/error.h++>
#include <phdl/unicode.h++>

// ICU headers
#include <unicode/uchar.h>
#include <unicode/utf8.h>

namespace phdl { namespace unicode {

	bool is_whitespace(const Character &c) {
		// Check if the given character is whitespace as defined by the
		// Unicode White_Space property.
		const char    *data  = c.data();
		const int32_t length = c.size();
		phdl_assert(length >= 0, "character too big");
		int32_t position = 0;
		char32_t codepoint;
		U8_NEXT(data, position, length, codepoint);
		return u_isUWhiteSpace(codepoint);
	}

}}
