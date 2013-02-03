#include "private.h++"

namespace phdl { namespace parser {

	using unicode::Character;
	using unicode::Characters;

	static bool is_newline(const Character &c) {
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

	size_t position::line_number(const Characters &text, size_t position) {
		// We can easily determine the line number of our current position by
		// counting the newlines that occur before our current position.
		//
		// Line numbers probably ought to start at 0, but are conventially
		// numbered starting at 1. So we start counting there to be more
		// friendly to our users.
		auto nl    = [](const Character &c) { return is_newline(c); };
		auto start = text.begin();
		auto end   = text.begin() + std::min(position, text.size());
		return std::count_if(start, end, nl) + 1;
	}


	//
	// 01234 5678 position
	// 01234 1234 column
	// abcd\nwxyz
	//        ^

	size_t position::column_number(const Characters &text, size_t position) {
		// We can determine our column number very easily by counting the
		// characters that occur before our position up to the previous
		// newline.
		//
		// Column numbers probably ought to start at 0, but are conventially
		// numbered starting at 1. So we start counting there to be more
		// friendly to our users.
		auto nl    = [](const Character &c) { return  is_newline(c); };
		auto notnl = [](const Character &c) { return !is_newline(c); };
		auto rstart = text.rbegin() + (text.size() - std::min(position, text.size()));
		auto rend   = std::find_if(rstart, text.rend(), nl);
		return std::count_if(rstart, rend, notnl) + 1;
	}

	Characters position::line_content(const Characters &text, size_t position) {
		// We can get the content of the current line by finding the previous
		// newline and the next newline and grabbing everything between them.
		auto nl    = [](const Character &c) { return  is_newline(c); };
		auto rstart = text.rbegin() + (text.size() - std::min(position, text.size()));
		auto rend   = std::find_if(rstart, text.rend(), nl);
		auto start = text.begin() + std::min(position, text.size());
		auto end   = std::find_if(start, text.end(), nl);
		Characters line;
		std::copy(rend.base(), end, std::back_inserter(line));
		return line;
	}

	Characters position::line_pointer(const Characters &text, size_t position) {
		// All we need to generate our line pointer is to get the column
		// number, which we already have a function to do conveniently. Then,
		// we just line up our pointer with the right number of spaces.
		size_t column = column_number(text, position);
		Characters line;
		std::fill_n(std::back_inserter(line), column-1, " ");
		line.push_back("^");
		return line;
	}

}}
