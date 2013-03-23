#include <phdl/error.h++>
#include <phdl/position.h++>

#include <sstream>

namespace phdl { namespace error {

	User_Visible_Error::User_Visible_Error (
		Severity severity,
		const std::string &filename,
		std::shared_ptr<Characters> text,
		size_t position,
		const std::string &message,
		boost::optional<const User_Visible_Error &> wrapped
	) :
		_severity(severity),
		_filename(filename),
		_text(text),
		_position(position),
		_message(message),
		_wrapped(wrapped)
	{}

	User_Visible_Error::~User_Visible_Error() throw() {}

	// Output user error messages into a GCC-like format, which is kind of the
	// de-facto standard for compiler error messages.
	//
	// For example:
	//
	//   User_Visible_Error err(
	//     Severity::Error, "example.phdl", text, 134,
	//     "an error occurred, please fix it"
	//   );
	//   os << err;
	//
	// Might yield:
	//
	//   example.phdl:5:19: error: an error occurred, please fix it
	//   example.phdl:5:19: context: net abcd = x;yz
	//   example.phdl:5:19: context:             ^

	static std::string severity_to_string(Severity severity) {
		switch (severity) {
			case Severity::Error:   return "error";
			case Severity::Warning: return "warning";
			case Severity::Debug:   return "debug";
			case Severity::Context: return "context";
			default: dynamic_assert(false, "invalid severity level");
		}
	}

	std::ostream &operator<<(std::ostream &os, const User_Visible_Error &error) {

		if (error._wrapped) os << (*error._wrapped).get();

		size_t line_number   = 1;
		size_t column_number = 1;

		if (error._text) {
			line_number   = phdl::position::line_number  (*error._text, error._position);
			column_number = phdl::position::column_number(*error._text, error._position);
		}

		std::stringstream ss;
		ss
			<< error._filename << ":"
			<< line_number     << ":"
			<< column_number   << ":"
			<< " " << severity_to_string(error._severity) << ": "
			<< error._message  << "\n"
		;
		os << ss.str();

		// If we're inside valid text, and the severity isn't already context,
		// print extra context information.
		if (
			(error._text) &&
			(error._position < error._text->size()) &&
			(error._severity != Severity::Context)
		) {

			os << User_Visible_Error (
				phdl::error::Severity::Context,
				error._filename, error._text, error._position,
				phdl::position::line_content(*error._text, error._position)
			);

			os << User_Visible_Error (
				phdl::error::Severity::Context,
				error._filename, error._text, error._position,
				phdl::position::line_pointer(*error._text, error._position)
			);
		}

		return os;
	}

}}
