#include <phdl/error.h++>
#include <phdl/parser.h++>
#include <phdl/position.h++>

namespace phdl { namespace parser {

	// FIXME: how user-visible and parse errors work needs to change a lot
	Parse_Error::Parse_Error (
		const Context &context,
		const std::string &expected_syntax,
		const optional<Parse_Error>
	) :
		User_Visible_Error(
			error::Severity::Error,
			context.filename(),
			position::line_number(context.text(), context.position()),
			position::column_number(context.text(), context.position()),
			expected_syntax
		)
	{}

}}
