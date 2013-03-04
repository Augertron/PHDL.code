#include "test.h++"
#include <phdl/error.h++>

#include <iostream>

using phdl::error::Severity;
using phdl::error::User_Visible_Error;

TEST(error_message) {
	User_Visible_Error error(Severity::Error, "the_file_name", 39, 13, "the error message");
	std::ostringstream ss;
	ss << error;
	EXPECT(ss.str() == "the_file_name:39:13: error: the error message\n");
}

TEST(warning_message) {
	User_Visible_Error error(Severity::Warning, "the_file_name", 39, 13, "the error message");
	std::ostringstream ss;
	ss << error;
	EXPECT(ss.str() == "the_file_name:39:13: warning: the error message\n");
}

TEST(debug_message) {
	User_Visible_Error error(Severity::Debug, "the_file_name", 39, 13, "the error message");
	std::ostringstream ss;
	ss << error;
	EXPECT(ss.str() == "the_file_name:39:13: debug: the error message\n");
}

TEST(basic_error_chaining_works) {
	User_Visible_Error error  (Severity::Error,   "error_file_name",    19, 58, "the error message");
	User_Visible_Error warning(Severity::Warning, "warning_file_name",  99,  3, "the warning message", error);
	User_Visible_Error debug  (Severity::Debug,   "debug_file_name",   179, 73, "the debug message", warning);
	std::ostringstream ss;
	ss << debug;
	EXPECT(ss.str() ==
		"error_file_name:19:58: error: the error message\n"
		"warning_file_name:99:3: warning: the warning message\n"
		"debug_file_name:179:73: debug: the debug message\n"
	);
}
