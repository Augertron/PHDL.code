// standard headers
#include <cassert>
#include <memory>
#include <stdexcept>
#include <string>

// boost headers
#include <boost/exception/all.hpp>

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
	//
	// Some important points to remember:
	//
	//   1. Everything besides the "validate" function assumes that strings
	//   are already correctly encoded in UTF-8.
	//
	//   2. Everything besides the "normalize" function assumes that strings
	//   are already correctly normalized to NFC.
	namespace unicode {

		// Validate the given (supposedly) UTF-8 string, throwing an
		// exception if an invalid sequence is found.
		struct Validation_Error {
			size_t position;
		};
		void validate(const std::string &);

		// Normalize the given UTF-8 string to NFC. Invalid sequences are
		// replaced with U+FFFD ("REPLACEMENT CHARACTER").
		std::string normalize(const std::string &);

	}
}
