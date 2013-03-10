#ifndef phdl__parser__header
#define phdl__parser__header

// standard headers
#include <cassert>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// boost headers
#include <boost/format.hpp>
#include <boost/optional.hpp>
#include <boost/variant.hpp>

// phdl headers
#include <phdl/error.h++>
#include <phdl/position.h++>
#include <phdl/unicode.h++>

namespace phdl { namespace parser {

	// This helper function reads the entire text from the given filename and
	// throws a parse error if there is a problem. The return text is
	// guaranteed to be normalized, split into Unicode characters, and directly
	// in the format expected for use by parser context objects.
	std::shared_ptr<Characters> read_file(const std::string &filename);

	// Our individual parsers work on a parser context, which keeps track
	// of the filename, text, and current position. This context also acts
	// like both an iterator and a sequence, providing a convenient view of
	// the file as a stream of characters.
	//
	// A parser is just a concept: any function or object which can be called
	// with a Context object and either returns some type of result, or throws
	// a user-visible error exception. Parsers can be called directly, but for
	// convenience will usually be called through these helper methods in order
	// to promote uniformity and avoid boilerplate.
	struct Context {
		Context();
		~Context();

		// With the given filename and text (assumed to be the content of that
		// file) create a context ready for parsing.
		Context(
			const std::string &filename,
			std::shared_ptr<Characters> text
		);

		// Copy a context (to allow indepedent positions).
		Context(const Context &);
		Context &operator=(const Context &);

		// Compare two contexts (same file and position)
		bool operator==(const Context &);
		bool operator!=(const Context &);

		// Get the filename.
		std::string filename() const;

		// Get the underlying text
		const std::shared_ptr<Characters> text() const;

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

		// Parse errors should be be thrown via this interface, which will
		// conveniently populates and throws a User_Visible_Error. Errors
		// thrown by failing parsers are not always reported to the user (e.g.
		// during normal parser backtracking), but we always track all the
		// context needed to generate good error messages when necessary.
		void throw_error (
			const std::string &message,
			boost::optional<const phdl::error::User_Visible_Error> wrapped = boost::none
		);

		private:
		struct Detail;
		std::unique_ptr<Detail> detail;
	};

	// Our PHDL AST is made of simple data types. This is appropriate here, as the
	// AST is intended to be as simple as possible so that the parser can
	// convert text to structure with very little unnecessary intelligence. The
	// parser is very forgiving, and most language checking is done after this
	// is converted into our more robust internal representation.

	namespace ast {

		// The node base type exists just to hold common attributes, such as a
		// pointer to the context corresponding to the particular node. This is
		// not to be used as polymorphic base class.
		struct Node {
			Context context;
		};

		// The language limits the allowed size of indicies, but the parser
		// just grabs a sequence of decimal digits, so bounds are checked by
		// the elaboration code.
		struct Index : Node {
			std::string digits;
		};

		// A range consists of either a single index, or a pair of indices
		// indicating the start and stop index in normal left to right order.
		struct Range : Node {
			boost::variant<
				Index,
				std::pair<Index,Index>
			> index;
		};

		// A slice just is a collection of ranges. The language requires that a
		// slice has at least one range element. (This fact is enforced in both
		// parsing and elaboration, but this object doesn't care.)
		struct Slice : Node {
			std::vector<Range> ranges;
		};

		// Although certain types of names (e.g. hierarchical vs. simple,
		// sliced vs. plain, normal vs. attribute) are allowed in different
		// places in the language, the parser encodes all names encountered
		// while into the same type of node for simplicity. Many constraints
		// are validated during parsing (basically just where its convenient to
		// do so); the rest of are checked during elaboration.
		struct Name : Node {
			enum class Type { Normal, Parameter, Attribute };
			Type type;
			std::vector<std::string> parts;
			boost::optional<Slice> slice;
		};

		// Forward declaration of blocks so they can be used in assignments.
		struct Block;

		// Almost every line in the language is fundamentally some type of
		// assignment. The parser follows suit and encodes anything
		// assignment-like into this node type. The decoded target and source
		// names are stored here, but validation of the compatibility between
		// target and source width and type is left alone until elaboration.
		// Likewise with the semantics of the associated block.
		struct Assignment : Node {
			std::vector<Name> targets;
			std::vector<Name> sources;
			boost::optional<boost::recursive_wrapper<Block>> block;
		};
		
		// All types of declarations are coalesced into a single type by the
		// parser. Many constraints are checked during parsing; the rest are
		// handled during elaboration. Almost all information is stored in the
		// nested Assignment object since declarations are effectively just
		// initial assignments.
		struct Declaration : Node {
			enum class Type {
				Attribute ,
				Board     ,
				Design    ,
				Device    ,
				Instance  ,
				Net       ,
				Package   ,
				Parameter ,
				Pin       ,
				Port      ,
			};
			Type type;
			Assignment assignment;
		};

		// All language block sections are unified into this single collection
		// of statements. Largely, the parser only validates allowed statement
		// insofar as it is convenient to do so. Further detailed analysis
		// happens during elaboration.
		struct Block : Node {
			std::vector<boost::variant<
				Assignment,
				Declaration
			>> statements;
		};

	}

	// This is the top-level parse function that parses a PHDL file and returns
	// the resulting AST starting with the implied top-level Block. A parse
	// error is thrown if any error is encountered.
	ast::Block parse_phdl(Context &);

	// Everything in the grammar namespace are the individual parsers for
	// pieces of the language. They are declared publicly here primarily so
	// that they can be more easily unit-tested. In general, these do not need
	// to ever be called individually.
	namespace grammar {

		// We often use boost::optional when matching against parsers. Since it
		// doesn't work with the void type, we make our own type to use to mark
		// the fact that a parser returns results that are simply ignored.
		struct ignored {};

		// Create a parser to match the given literal value.
		std::function<ignored(Context &)> literal(std::string value);

		// Match the end of file.
		ignored end_of_file(Context &context);

		// Match actual whitespace (not including comments).
		ignored actual_whitespace(Context &context);

		// Match a single-line comment.
		ignored single_line_comment(Context &context);

		// Match a multi-line comment.
		ignored multi_line_comment(Context &context);

		// Match any type of comment.
		ignored comment(Context &context);

		// Match any effective whitespace (actual whitespace or comments).
		ignored whitespace(Context &context);

		// Parse an index.
		ast::Index index(Context &context);

		// Parse a range.
		ast::Range range(Context &context);

		// Parse a slice.
		ast::Slice slice(Context &context);

		// Parse an simple unquoted name part, with no hierarchy or slice part.
		std::string unquoted_name(Context &context);

		// Parse a simple quoted name part, with no hierarchy or slice part.
		std::string quoted_name(Context &context);

		// Parse a fully-qualified, optionally sliced name of any kind.
		ast::Name name(Context &context);
	}

}}

#endif
