#include <phdl/error.h++>
#include <phdl/unicode.h++>

// ICU headers
#include <unicode/normalizer2.h>

namespace phdl { namespace unicode {

	std::string normalize(const std::string &unnormalized_utf8) {
		// FIXME: until ICU supports doing normalization over a UText interface
		// directly on our UTF-8, we'll use the less efficient approach of
		// converting to UTF-16, normalizing, and converting back to UTF-8.

		// Convert to UTF-16 string
		const auto unnormalized_utf16 =
			icu::UnicodeString::fromUTF8(unnormalized_utf8);

		// Get a pointer to the global NFC normalizer
		UErrorCode icu_error = U_ZERO_ERROR;
		const auto *normalizer = icu::Normalizer2::getInstance (
			nullptr, "nfc", UNORM2_COMPOSE, icu_error
		);
		dynamic_assert(U_SUCCESS(icu_error), "couldn't get normalizer instance");

		// Normalize our string
		icu::UnicodeString normalized_utf16;
		normalizer->normalize(unnormalized_utf16, normalized_utf16, icu_error);
		dynamic_assert(U_SUCCESS(icu_error), "normalization failed");

		// Convert back to UTF-8
		std::string normalized_utf8;
		normalized_utf16.toUTF8String(normalized_utf8);

		return normalized_utf8;
	}

}}
