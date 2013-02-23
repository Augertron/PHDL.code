#include "phdl/private.h++"

// ICU headers
#include <unicode/brkiter.h>
#include <unicode/normalizer2.h>

namespace phdl { namespace unicode {

	Characters split_characters(const std::string &string) {
		// FIXME: ICU makes us use int32_t for positions, instead of size_t.
		// We can get around this by doing something more complex, but for
		// now, we're just going to make sure this works and hope that we
		// never see a string that's too long.
		int32_t length = int32_t(string.size());
		assert(size_t(length) == string.size());

		// Create a UText object to let our string to work break iterator
		UErrorCode icu_error = U_ZERO_ERROR;
		UText *string_utext = utext_openUTF8(
			nullptr,
			string.data(),
			string.size(),
			&icu_error
		);
		assert(U_SUCCESS(icu_error));

		// Create a break iterator
		auto *break_iterator = icu::BreakIterator::createCharacterInstance(
			icu::Locale::getRoot(),
			icu_error
		);
		assert(U_SUCCESS(icu_error));

		// Point the break iterator at our string
		break_iterator->setText(
			string_utext,
			icu_error
		);
		assert(U_SUCCESS(icu_error));

		// Iterate through all of the boundaries and grab all of the
		// characters between them.
		int32_t start = break_iterator->first();
		Characters characters;
		while (start < length) {
			int32_t end = break_iterator->next();
			if (end == icu::BreakIterator::DONE) end = string.size();
			characters.push_back(std::string(
				string.begin() + start,
				string.begin() + end
			));
			start = end;
		}

		// Delete our break iterator
		delete break_iterator;

		// Delete our UText
		utext_close(string_utext);

		// Return the results
		return characters;
	}

	std::string combine_characters(const Characters &characters) {
		// Much different from the complexity needed for splitting, combining
		// is just a matter of pasting the characters back together.
		std::string string;
		for (auto c : characters) string.append(c);
		return string;
	}

	bool is_whitespace(const Character &c) {
		// Check if the given character is whitespace as defined by the
		// Unicode White_Space property.
		const char    *data  = c.data();
		const int32_t length = c.size();
		int32_t position = 0;
		char32_t codepoint;
		U8_NEXT(data, position, length, codepoint);
		return u_isUWhiteSpace(codepoint);
	}

	bool is_newline(const Character &c) {
		// The newline sequences we use are conformant to the Unicode
		// Technical Standard #18, "Unicode Regulator Expressions", revision
		// 15, section 1.6, "Line Boundaries"[1].
		//
		// [1] http://www.unicode.org/reports/tr18/#Line_Boundaries
		//
		// These sequences specifically recommended by the standard for use
		// as logical line numbers reported by compiler error messages. These
		// are the same sequences matched by the "\R" meta-character in
		// regular expression engines following the Unicode standard
		// recommendation (such as Perl and PCRE).
		//
		// The sequences are shown here for easy reference:
		//
		//   \u{A}       LINE FEED (LF)
		//   \u{B}       LINE TABULATION (VT)
		//   \u{C}       FORM FEED (FF)
		//   \u{D}       CARRIAGE RETURN (CR)
		//   \u{85}      NEXT LINE (NEL)
		//   \u{2028}    LINE SEPARATOR (LS)
		//   \u{2029}    PARAGRAPH SEPARATOR (PS)
		//   \u{D}\u{A}  CARRIAGE RETURN (CR), LINE FEED (LF)
		return (
			(c == u8"\u000a"      ) | // LF
			(c == u8"\u000b"      ) | // VT
			(c == u8"\u000c"      ) | // FF
			(c == u8"\u000d"      ) | // CR
			(c == u8"\u0085"      ) | // NEL
			(c == u8"\u2028"      ) | // LS
			(c == u8"\u2029"      ) | // PS
			(c == u8"\u000d\u000a")   // CR+LF
		);
	}

}}
