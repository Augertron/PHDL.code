#include "test.h++"
#include <phdl/parser.h++>

#include <iostream>

using phdl::parser::Parse_Error;

TEST(read_file_error) {
	try {
		phdl::parser::read_file("nonexistant_file.phdl");
		REQUIRE(false);
	} catch (const Parse_Error &error) {
		std::ostringstream ss;
		ss << error;
		EXPECT(ss.str() == "nonexistant_file.phdl:1:1: error: could not open file\n");
	}
}
