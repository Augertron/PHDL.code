#ifndef phdl__parser__header
#define phdl__parser__header

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
#include <boost/variant.hpp>

// phdl headers
#include <phdl/error.h++>
#include <phdl/position.h++>
#include <phdl/unicode.h++>

namespace phdl { namespace parser {

		// We use boost optional and boost variant types in lots of parser
		// code, so just import them into this namespace.
		using boost::optional;
		using boost::none;
		using boost::variant;

		// Our individual parsers work on a parser context, which keeps track
		// of the filename, text, and current position. This context also acts
		// like both an iterator and a sequence, providing a convenient view of
		// the file as a stream of characters.
		struct Context {
			~Context();

			// Read the content of the given file an create a context ready
			// for parsing.
			Context(const std::string &filename);

			// Copy a context (to allow indepedent positions).
			Context(const Context &);
			Context &operator=(const Context &);

			// Compare two contexts (same file and position)
			bool operator==(const Context &);
			bool operator!=(const Context &);

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
		};

		// Errors thrown by failing parsers are [potentially] user-visible, and
		// keep track of the context where they were generated, and the parse
		// error(s) that caused them.
		struct Parse_Error : User_Visible_Error {
			Parse_Error (
				const Context &context,
				const std::string &expected_syntax,
				const optional<Parse_Error> cause = none
			);
		};

		// Parsers are any function or object which can be called with a
		// Context and either returns some type of Result, or throws a
		// Parse_Error exception. Parsers should be called directly when they
		// must match, but can also be called through the following helper
		// functions in order to avoid too much boilerplate.
		//
		template <typename Result, typename Parser>
		Result expect (Context &context, Parser &parser) {
			return parser(context);
		}

		template <typename Result, typename Parser>
		optional<Result> match (Context &context, Parser &parser) {
			try {
				return parser(context);
			} catch (const Parse_Error &) {
				return none;
			}
		}

		// These parsers just combine other parsers in common ways.

	}

	//std::string file_error_prefix(const File_Information &);
	//
	//struct File_Information {
	//	std::string filename;
	//	std::string text;
	//	size_t position;
	//};


}

#endif
