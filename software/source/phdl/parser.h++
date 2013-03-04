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
	// a Parse_Error exception. Parsers can be called directly, but for
	// convenience will usually be called through these helper methods in order
	// to promote uniformity and avoid boilerplate.
	struct Context {
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

		// Optionally match the current context with the given parser. If there
		// is a match, the result is returned and the position is advanced.
		// Otherwise, no result is returned, and the context stays unchanged.
		template <typename Result, typename Parser>
		boost::optional<Result> match (Parser &parser);

		// Require the current context to match with the given parser and
		// return the result. This is really no different from calling a parser
		// directly with the current context, but is provided to allow a
		// similar calling convention to the match function.
		template <typename Result, typename Parser>
		Result expect (Parser &parser);

		// Parse errors should be be thrown via this interface, which will
		// generate a chain of User_Visible_Errors. Errors thrown by failing
		// parsers are not always reported to the user (e.g. during normal
		// parser backtracking), but we always track all the context needed to
		// generate good error messages when necessary.
		void parse_error (
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

		// We strictly limit allowed indices to [-2**15,2**15), under the
		// assumption that a 64 Kibit bus should be wide enough for anybody.
		using Index = int16_t;

		// A range consists of either a single index, or a pair of indices
		// indicating the start and stop index in normal left to right order.
		struct Range {
			boost::variant<
				Index,
				std::pair<Index,Index>
			> index;
		};

		// A slice just is a collection of ranges. The language requires that a
		// slice has at least one range element. (This fact is enforced in both
		// parsing and elaboration, but this object doesn't care.)
		struct Slice {
			std::vector<Range> ranges;
		};

		// Although certain types of names (e.g. hierarchical vs. simple,
		// sliced vs. plain, normal vs. attribute) are allowed in different
		// places in the language, the parser encodes all names encountered
		// while into the same type of node for simplicity. Many constraints
		// are validated during parsing (basically just where its convenient to
		// do so); the rest of are checked during elaboration.
		struct Name {
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
		struct Assignment {
			std::vector<Name> targets;
			std::vector<Name> sources;
			boost::optional<boost::recursive_wrapper<Block>> block;
		};
		
		// All types of declarations are coalesced into a single type by the
		// parser. Many constraints are checked during parsing; the rest are
		// handled during elaboration. Almost all information is stored in the
		// nested Assignment object since declarations are effectively just
		// initial assignments.
		struct Declaration {
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
		struct Block {
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

}}

#include "parser/Context.template.h++"

#endif
