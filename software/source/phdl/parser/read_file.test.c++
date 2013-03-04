#include "test.h++"
#include <phdl/parser.h++>

#include <iostream>

using phdl::User_Visible_Error;

TEST(read_file_error) {
	try {
		phdl::parser::read_file("nonexistant_file.phdl");
		REQUIRE(false);
	} catch (const User_Visible_Error &error) {
		std::ostringstream ss;
		ss << error;
		EXPECT(ss.str() == "nonexistant_file.phdl:1:1: error: could not open file\n");
	}
}
