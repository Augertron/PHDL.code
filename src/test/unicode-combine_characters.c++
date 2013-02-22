#include "test.h++"

#include "phdl/private.h++"

using namespace phdl::unicode;

TEST(basic_combining_works) {
	EXPECT(combine_characters({ "a", "b", "c", "d" }) == "abcd");
	EXPECT(combine_characters({"\u00e9", "x", "p", "o", "s", "e\u0301"}) == u8"\u00e9xpose\u0301");
}
