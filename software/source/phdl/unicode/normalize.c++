#include <phdl/error.h++>
#include <phdl/unicode.h++>

// Standard headers
//#include <locale>

// ICU headers
#include <unicode/normalizer2.h>

// Qt headers
//#include <QtCore/QString>

// Boost headers
//#include <boost/locale.hpp>

namespace phdl { namespace unicode {

	std::string normalize(const std::string &unnormalized_utf8, Normalization mode) {
		// TODO: once ICU supports doing normalization over a UText interface
		// directly on our UTF-8 string, we'll be able to directly normalize
		// without intermediate conversion. Until then, we'll convert to
		// UTF-16, then normalize, then convert back to UTF-8.

		// Convert to UTF-16 string
		auto const unnormalized_utf16 = icu::UnicodeString::fromUTF8(unnormalized_utf8);

		// Determine the desired ICU normalizer name and mode.
		char const *icu_norm2_name = nullptr;
		UNormalization2Mode icu_norm2_mode;

		switch (mode) {
			case Normalization::NFD          : icu_norm2_name = "nfd"    ; icu_norm2_mode = UNORM2_DECOMPOSE; break;
			case Normalization::NFKD         : icu_norm2_name = "nfkd"   ; icu_norm2_mode = UNORM2_DECOMPOSE; break;
			case Normalization::NFC          : icu_norm2_name = "nfc"    ; icu_norm2_mode = UNORM2_COMPOSE;   break;
			case Normalization::NFKC         : icu_norm2_name = "nfkc"   ; icu_norm2_mode = UNORM2_COMPOSE;   break;
			case Normalization::NFKC_Casefold: icu_norm2_name = "nfkc_cf"; icu_norm2_mode = UNORM2_COMPOSE;   break;
			case Normalization::None         :                                                                break;
			default: dynamic_assert(false, "invalid normalization mode");
		}

		// If normalization was requested, normalize our string.
		UErrorCode icu_error = U_ZERO_ERROR;
		icu::UnicodeString normalized_utf16;
		if (icu_norm2_name) {
			auto const * const icu_norm2 = icu::Normalizer2::getInstance (nullptr, icu_norm2_name, icu_norm2_mode, icu_error);
			dynamic_assert(U_SUCCESS(icu_error), "couldn't get normalizer instance");
			icu_norm2->normalize(unnormalized_utf16, normalized_utf16, icu_error);
			dynamic_assert(U_SUCCESS(icu_error), "normalization failed");
		}

		// Convert back to UTF-8
		std::string normalized_utf8;
		normalized_utf16.toUTF8String(normalized_utf8);

		return normalized_utf8;
	}

}}
