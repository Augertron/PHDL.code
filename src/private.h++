// standard headers
#include <cassert>
#include <memory>
#include <stdexcept>
#include <string>

// boost headers
#include <boost/exception/all.hpp>
#include <boost/format.hpp>

namespace phdl {

	namespace unicode {

		// A character is a UTF-8 encoded Unicode extended grapheme cluster.
		using character  = std::string;
		using characters = std::vector<std::string>;

		// Normalize the given string to NFC. Invalid sequences are replaced
		// with the standard Unicode replacement character, U+FFFD.
		std::string normalize(const std::string &);

		// Split the given string into individual characters.
		std::vector<std::string> split_characters(const std::string &);
	}

	namespace parser {
		namespace position {
			size_t      line_number  (const unicode::characters &text, size_t position);
			size_t      column_number(const unicode::characters &text, size_t position);
			std::string line_content (const unicode::characters &text, size_t position);
			std::string line_pointer (const unicode::characters &text, size_t position);
		}
	}

	//std::string file_error_prefix(const File_Information &);
	//
	//struct File_Information {
	//	std::string filename;
	//	std::string text;
	//	size_t position;
	//};


}
