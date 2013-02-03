// standard headers
#include <cassert>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>

// boost headers
#include <boost/exception/all.hpp>
#include <boost/format.hpp>
#include <boost/optional.hpp>

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

		// Return whether the given character is whitespace or not.
		bool is_whitespace(const Character &c);

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

	namespace parser {

		// Our individual parsers work on a parser context, which keeps track
		// of the filename, text, and current position. This context also
		// provides a convenient view of the file as a stream of characters.
		struct Context {

			// Read the content of the given file an create a context ready
			// for parsing.
			Context(const std::string &filename);

			// Copy a context (to allow indepedent positions).
			Context(const Context &);

			// Get the filename.
			std::string filename() const;

			// Get or set the current absolute position.
			size_t position() const;
			void set_position(size_t);

			// Returns the character relative to the given position. An
			// empty character is returned on out-of-bounds access.
			const Character &operator[](int offset) const;
			const Character &operator*() const;
			const Character *operator->() const;

			// Move the position forward or backward.
			Context &operator++();
			Context  operator++(int);
			Context &operator+=(int);
			Context &operator--();
			Context  operator--(int);
			Context &operator-=(int);

			private:
			struct Detail;
			std::unique_ptr<Detail> detail;
			//const std::string filename;
			//const std::shared_ptr<const Characters> text;
			//size_t position;
		};
	}

	//std::string file_error_prefix(const File_Information &);
	//
	//struct File_Information {
	//	std::string filename;
	//	std::string text;
	//	size_t position;
	//};


}
