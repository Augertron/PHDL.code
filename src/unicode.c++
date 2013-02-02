#include "private.h++"

// ICU headers
#include <unicode/normalizer2.h>
#include <unicode/unistr.h>
#include <unicode/utf8.h>

namespace phdl { namespace unicode {

	void validate(const std::string &hopefully_utf8) {
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

	std::string normalize(const std::string &unnormalized_utf8) {
		// FIXME: until ICU supports doing normalization over a UText
		// interface directly on our UTF-8, we'll use the insanely less
		// efficient approach of converting to UTF-16, normalizing, and
		// converting back to UTF-8.

		// Convert to UTF-16 string
		auto unnormalized_utf16 = icu::UnicodeString::fromUTF8(unnormalized_utf8);

		// Get a pointer to the global NFC normalizer
		UErrorCode icu_error = U_ZERO_ERROR;
		const auto *normalizer = icu::Normalizer2::getInstance(nullptr, "nfc", UNORM2_COMPOSE, icu_error);
		assert(U_SUCCESS(icu_error));

		// Normalize our string
		icu::UnicodeString normalized_utf16;
		normalizer->normalize(unnormalized_utf16, normalized_utf16, icu_error);
		assert(U_SUCCESS(icu_error));

		// Convert back to UTF-8
		std::string normalized_utf8;
		normalized_utf16.toUTF8String(normalized_utf8);

		return normalized_utf8;
	}

}}
