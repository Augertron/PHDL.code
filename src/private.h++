// standard headers
#include <cassert>
#include <memory>
#include <stdexcept>
#include <string>

// boost headers
#include <boost/exception/all.hpp>
#include <boost/format.hpp>

namespace phdl {

	// A character is a UTF-8 encoded Unicode extended grapheme cluster.
	using Character  = std::string;
	using Characters = std::vector<Character>;

	// Functions abstracting Unicode manipulation.
	namespace unicode {
		// Normalize the given string to NFC. Invalid sequences are replaced
		// with the standard Unicode replacement character, U+FFFD.
		std::string normalize(const std::string &);

		// Split the given string into individual characters.
		Characters split_characters(const std::string &);

		// Combine split characters back into a string.
		std::string combine_characters(const Characters &);

		// Return whether the given character is a newline or not.
		bool is_newline(const Character &c);
	}

	// Transform position into line and column numbers as well as content
	// and pointer strings for use primarily in error reporting.
	namespace position {
		size_t      line_number  (const Characters &text, size_t position);
		size_t      column_number(const Characters &text, size_t position);
		std::string line_content (const Characters &text, size_t position);
		std::string line_pointer (const Characters &text, size_t position);
	}


	//std::string file_error_prefix(const File_Information &);
	//
	//struct File_Information {
	//	std::string filename;
	//	std::string text;
	//	size_t position;
	//};


}
