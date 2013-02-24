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
