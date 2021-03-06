#ifndef phdl__unicode__header
#define phdl__unicode__header

// standard headers
#include <string>
#include <vector>

namespace phdl { namespace unicode {

	// A character is a UTF-8 encoded Unicode extended grapheme cluster.
	using Character  = std::string;
	using Characters = std::vector<Character>;

	// Normalize the given string to the given mode. Invalid sequences are
	// replaced with the standard Unicode replacement character, U+FFFD.
	enum class Normalization { None, NFD, NFKD, NFC, NFKC, NFKC_Casefold };
	std::string normalize(const std::string &, Normalization = Normalization::NFC);

	// Split the given string into individual characters.
	Characters split_characters(const std::string &);

	// Combine split characters back into a string.
	std::string combine_characters(const Characters &);

	// Return whether the given character is whitespace or not.
	bool is_whitespace(const Character &c);

	// Return whether the given character is a newline or not.
	bool is_newline(const Character &c);

}}

namespace phdl {
	// We use our unicode character types everywhere, so we want to import them
	// into the top level namespace.
	using phdl::unicode::Character;
	using phdl::unicode::Characters;
}

#endif
