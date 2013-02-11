#include "Error.h++"

#include <sstream>
#include <typeinfo>

namespace phdl {

	const char *Error::what() const noexcept {
		return typeid(*this).name();
	}

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

	static std::string severity_to_string(Severity severity) {
		switch (severity) {
			case Severity::Error:   return "error";
			case Severity::Warning: return "warning";
			case Severity::Debug:   return "debug";

			default:
				assert(false && PHDL_BUG "severity level");
		}
	}

	std::ostream &operator<<(std::ostream &os, const User_Visible_Error &error) {
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
