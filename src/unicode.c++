#include "private.h++"

// ICU headers
#include <unicode/utf8.h>

namespace phdl { namespace unicode {

	void validate (const std::string &hopefully_utf8) {
		// Iterate through the UTF-8 string using ICU's low-level UTF-8
		// interface, looking for any invalid encoding sequences.
		const char *string = hopefully_utf8.data();
		const int32_t length = hopefully_utf8.size();
		assert(size_t(length) == hopefully_utf8.size());
		int32_t position = 0;
		int32_t codepoint;
		while (position < length) {
			int32_t error_position = position;
			U8_NEXT(string, position, length, codepoint);
			if (codepoint < 0) throw Validation_Error { size_t(error_position) };
		}
	}

}}
