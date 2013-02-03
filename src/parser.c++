#include "private.h++"

namespace phdl { namespace parser {

	using unicode::character;
	using unicode::characters;

	static bool is_newline(const character &c) {
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

	size_t position::line_number(const characters &text, size_t position) {
		// We can easily determine the line number of our current position by
		// counting the newlines that occur before our current position.
		//
		// Line numbers probably ought to start at 0, but are conventially
		// numbered starting at 1. So we start counting there to be more
		// friendly to our users.
		auto nl    = [](const unicode::character &c) { return is_newline(c); };
		auto start = text.begin();
		auto end   = text.begin() + std::min(position, text.size());
		return std::count_if(start, end, nl) + 1;
	}


	//
	// 01234 5678 position
	// 01234 1234 column
	// abcd\nwxyz
	//        ^

	size_t position::column_number(const characters &text, size_t position) {
		// We can determine our column number very easily by counting the
		// characters that occur before our position up to the previous
		// newline or beginning of the text.
		//
		// Column numbers probably ought to start at 0, but are conventially
		// numbered starting at 1. So we start counting there to be more
		// friendly to our users.
		auto nl    = [](const unicode::character &c) { return  is_newline(c); };
		auto notnl = [](const unicode::character &c) { return !is_newline(c); };
		auto start = text.rbegin() + (text.size() - std::min(position, text.size()));
		auto end   = std::find_if(start, text.rend(), nl);
		return std::count_if(start, end, notnl) + 1;
	}

	//}
	//	std::string line_content (const unicode::characters &text, size_t position);
	//	std::string line_pointer (const unicode::characters &text, size_t position);
	//}

}}
