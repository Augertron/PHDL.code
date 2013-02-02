// standard headers
#include <cassert>
#include <memory>
#include <stdexcept>
#include <string>

// boost headers
#include <boost/exception/all.hpp>
#include <boost/format.hpp>

namespace phdl {

	//struct File_Information {
	//	std::string filename;
	//	std::string text;
	//	size_t position;
	//};

	//namespace position {
	//	size_t to_line_number(const std::string &text, size_t position);
	//	size_t to_column_number(const std::string &text, size_t position);
	//	std::string to_line_content(const std::string &text, size_t position);
	//	std::string to_line_pointer(const std::string &text, size_t position);
	//}
	//std::string file_error_prefix(const File_Information &);

	// Interface for manipulating Unicode strings.
	namespace unicode {

		// Normalize the given string to NFC. (Invalid sequences are replaced
		// with the standard Unicode replacement character, U+FFFD).
		std::string normalize(const std::string &);

		// Split the given string into individual characters. (By characters,
		// we mean standard Unicode extended grapheme clusters.)
		std::vector<std::string> split_characters(const std::string &);

	}
}
