#include "Error.h++"

#include <sstream>
#include <typeinfo>

namespace phdl {

	// In normal circumstances, this function will never be called. Thus, we
	// don't need anything special here, just enough to identify the exception
	// in a debugging scenerio. This means we can just use the output from
	// std::type_info::name with no worries about its human-readability.
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

	// Output user error messages into a GCC-like format, which is kind of the
	// de-facto standard for compiler error messages:
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
