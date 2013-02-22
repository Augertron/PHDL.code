#include "phdl/error.h++"

#include <sstream>

namespace phdl {

	User_Visible_Error::User_Visible_Error (
		Severity severity,
		const std::string &file_name,
		size_t line_number,
		size_t column_number,
		const std::string &message
	) :
		_severity(severity),
		_file_name(file_name),
		_line_number(line_number),
		_column_number(column_number),
		_message(message)
	{}

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
				default: phdl_assert(false, "invalid severity level");
			}
		};

		std::stringstream ss;
		ss
			<< error._file_name     << ":"
			<< error._line_number   << ":"
			<< error._column_number << ":"
			<< " " << severity_to_string(error._severity) << ": "
			<< error._message       << "\n"
		;
		os << ss.str();
		return os;

	}

}
