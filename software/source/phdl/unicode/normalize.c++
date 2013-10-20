#include <phdl/error.h++>
#include <phdl/unicode.h++>

// Standard headers
#include <locale>

// ICU headers
#include <unicode/normalizer2.h>

// Qt headers
#include <QtCore/QString>

// Boost headers
#include <boost/locale.hpp>

namespace phdl { namespace unicode {

	std::string normalize(const std::string &unnormalized_utf8) {
		// TODO: once ICU supports doing normalization over a UText interface
		// directly on our UTF-8, we'll directly normalize UTF-8. Until then,
		// we'll convert to UTF-16, normalize, and convert back to UTF-8.

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

	//std::string normalize(const std::string &unnormalized_utf8) {
	//	// Convert to QString (UTF-16)
	//	const QString unnormalized_utf16 = QString::fromUtf8(unnormalized_utf8.data(), unnormalized_utf8.size());

	//	// Normalize our string
	//	const QString normalized_utf16 = unnormalized_utf16.normalized(QString::NormalizationForm_C);

	//	// Convert back to UTF-8
	//	const std::string normalized_utf8 = normalized_utf16.toStdString();

	//	return normalized_utf8;
	//}
	
	//static bool normalize_init() {
	//	boost::locale::generator gen;
	//	std::locale::global(gen("en_US.UTF-8"));
	//	return true;
	//};

	//std::string normalize(const std::string &unnormalized_utf8) {
	//	static bool init = normalize_init();
	//	(void)init;
	//	return boost::locale::normalize(unnormalized_utf8, boost::locale::norm_nfc);
	//}


}}
