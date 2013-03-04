#include "test.h++"
#include <phdl/error.h++>
#include <phdl/unicode.h++>

#include <iostream>

using phdl::Characters;
using phdl::error::Severity;
using phdl::error::User_Visible_Error;

struct Fixture {
	std::shared_ptr<Characters> text;
	Fixture() {
		text.reset(new Characters(
			phdl::unicode::split_characters("abc\n123\r\nxyz\n")
		));
	}
};

TEST_METHOD(Fixture, error_message) {
	User_Visible_Error error(Severity::Error, "the_file_name", text, 1, "the error message");
	std::ostringstream ss;
	ss << error;
	EXPECT(ss.str() == "the_file_name:1:2: error: the error message\n");
}

TEST_METHOD(Fixture, warning_message) {
	User_Visible_Error error(Severity::Warning, "the_file_name", text, 6, "the error message");
	std::ostringstream ss;
	ss << error;
	EXPECT(ss.str() == "the_file_name:2:3: warning: the error message\n");
}

TEST_METHOD(Fixture, debug_message) {
	User_Visible_Error error(Severity::Debug, "the_file_name", text, 7, "the error message");
	std::ostringstream ss;
	ss << error;
	EXPECT(ss.str() == "the_file_name:2:4: debug: the error message\n");
}

TEST_METHOD(Fixture, context_message) {
	User_Visible_Error error(Severity::Context, "the_file_name", text, 8, "the error message");
	std::ostringstream ss;
	ss << error;
	EXPECT(ss.str() == "the_file_name:3:1: context: the error message\n");
}

TEST_METHOD(Fixture, basic_error_chaining_works) {
	User_Visible_Error error  (Severity::Error,   "error_file_name",   text,  2, "the error message");
	User_Visible_Error warning(Severity::Warning, "warning_file_name", text,  9, "the warning message", error);
	User_Visible_Error debug  (Severity::Debug,   "debug_file_name",   text, 20, "the debug message", warning);
	std::ostringstream ss;
	ss << debug;
	EXPECT(ss.str() ==
		"error_file_name:1:3: error: the error message\n"
		"warning_file_name:3:2: warning: the warning message\n"
		"debug_file_name:4:1: debug: the debug message\n"
	);
}
