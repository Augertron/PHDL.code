#include "test.h++"
#include "../Error.h++"

#include <iostream>

using phdl::Error;
using phdl::User_Visible_Error;
using phdl::Severity;

TEST(add_error_info) {
	using error_message = boost::error_info<struct tag_error_message, std::string>;
	try {
		throw Error() << error_message("this is the message");
		REQUIRE(false);
	} catch (const Error &e) {
		if (auto *message = boost::get_error_info<error_message>(e)) {
			EXPECT(*message == "this is the message");
		} else {
			REQUIRE(false);
		}
	}
}

TEST(BOOST_THROW_EXCEPTION_and_diagnostics) {
	using position = boost::error_info<struct tag_position, size_t>;
	try {
		BOOST_THROW_EXCEPTION(Error() << position(88));
		REQUIRE(false);
	} catch (const Error &e) {
		std::cout << boost::diagnostic_information(e);
		EXPECT(*boost::get_error_info<position>(e) == 88);
	}
}

TEST(inline_defined_error_infos) {
	try {
		throw Error()
			<< boost::error_info<struct tag_c,char>('x')
			<< boost::error_info<struct tag_d,int >(8)
		;
		REQUIRE(false);
	} catch (const Error &e) {
		std::cout << boost::diagnostic_information(e);
		REQUIRE(true);
	}
}

TEST(user_visible_error_message) {
	User_Visible_Error error(Severity::Warning, "the_file_name", 39, 13, "the error message");
	std::ostringstream ss;
	ss << error;
	EXPECT(ss.str() == "the_file_name:39:13: warning: the error message\n");
}
