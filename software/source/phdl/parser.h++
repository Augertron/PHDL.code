#ifndef phdl__parser__header
#define phdl__parser__header

// standard headers
#include <cassert>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>

// boost headers
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
	//
	// A parser is just a concept: any function or object which can be called
	// with a Context object and either returns some type of result, or throws
	// a Parse_Error exception. Parsers can be called directly, but for
	// convenience will usually be called through these helper methods in order
	// to promote uniformity and avoid boilerplate.
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

		// Get the underlying text
		Characters text() const;

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

		// Optionally match the current context with the given parser. If there
		// is a match, the result is returned and the position is advanced.
		// Otherwise, no result is returned, and the context stays unchanged.
		template <typename Result, typename Parser>
		optional<Result> match (Parser &parser);

		// Require the current context to match with the given parser and
		// return the result. This is really no different from calling a parser
		// directly with the current context, but is provided to allow a
		// similar calling convention to the match function.
		template <typename Result, typename Parser>
		Result expect (Parser &parser);

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

}}

#include "parser/Context.template.h++"

#endif
