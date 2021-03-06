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
		dynamic_assert(length >= 0, "character too big");
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
