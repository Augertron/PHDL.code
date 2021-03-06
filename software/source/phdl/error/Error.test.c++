#include "test.h++"
#include <phdl/error.h++>

#include <iostream>

using phdl::error::Error;

struct My_Error : Error {};

TEST(add_error_info) {
	using error_message = boost::error_info<struct tag_error_message, std::string>;
	try {
		throw My_Error() << error_message("this is the message");
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
		BOOST_THROW_EXCEPTION(My_Error() << position(88));
		REQUIRE(false);
	} catch (const Error &e) {
		std::cout << boost::diagnostic_information(e);
		EXPECT(*boost::get_error_info<position>(e) == 88);
	}
}

TEST(inline_defined_error_infos) {
	try {
		throw My_Error()
			<< boost::error_info<struct tag_c,char>('x')
			<< boost::error_info<struct tag_d,int >(8)
		;
		REQUIRE(false);
	} catch (const Error &e) {
		std::cout << boost::diagnostic_information(e);
		REQUIRE(true);
	}
}
