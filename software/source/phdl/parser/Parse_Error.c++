#include <phdl/error.h++>
#include <phdl/parser.h++>
#include <phdl/position.h++>

namespace phdl { namespace parser {

	Parse_Error::Parse_Error (
		const Context &context,
		const std::string &expected_syntax,
		boost::optional<const User_Visible_Error> wrapped
	) :
		User_Visible_Error(
			error::Severity::Error,
			context.filename(),
			position::line_number(context.text(), context.position()),
			position::column_number(context.text(), context.position()),
			expected_syntax,
			boost::optional<const User_Visible_Error &>(wrapped)
		)
	{}

}}
