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
	//     Severity::Error, "example.phdl", 5, 19,
	//     "an error occurred, please fix it"
	//   );
	//   os << err;
	//
	// Yields:
	//
	//   example.phdl:5:19: error: an error occurred, please fix it
	//
	std::ostream &operator<<(std::ostream &os, const User_Visible_Error &error) {

		auto severity_to_string = [](Severity severity) {
			switch (severity) {
				case Severity::Error:   return "error";
				case Severity::Warning: return "warning";
				case Severity::Debug:   return "debug";
				case Severity::Context: return "context";
				default: phdl_assert(false, "invalid severity level");
			}
		};

		std::stringstream ss;
		if (error._wrapped) ss << (*error._wrapped).get();

		size_t line_number   = 1;
		size_t column_number = 1;

		if (error._text) {
			line_number   = phdl::position::line_number  (*error._text, error._position); 
			column_number = phdl::position::column_number(*error._text, error._position); 
		}

		ss
			<< error._filename << ":"
			<< line_number     << ":"
			<< column_number   << ":"
			<< " " << severity_to_string(error._severity) << ": "
			<< error._message       << "\n"
		;
		os << ss.str();
		return os;

	}

}}
