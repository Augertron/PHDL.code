#include "test.h++"

using namespace phdl::unicode;

#define SV std::vector<std::string>

TEST(basic_splitting_works) {
	EXPECT(split_characters(u8"abcd") == SV({ "a", "b", "c", "d" }));
	EXPECT(split_characters(u8"\u00e9xpose\u0301") == SV({
		"\u00e9", "x", "p", "o", "s", "e\u0301"
	}));
}

TEST(newline_characters) {
	EXPECT(split_characters(u8"\n\n\r\r\n\r") == SV({ "\n", "\n", "\r", "\r\n", "\r" }));
}

TEST(examples_from_tr15) {
	EXPECT(split_characters(u8"\u212b"            ) == SV({ u8"\u212b"             }));
	EXPECT(split_characters(u8"\u0041\u030a"      ) == SV({ u8"\u0041\u030a"       }));
	EXPECT(split_characters(u8"\u2126"            ) == SV({ u8"\u2126"             }));
	EXPECT(split_characters(u8"\u006f\u0302"      ) == SV({ u8"\u006f\u0302"       }));
	EXPECT(split_characters(u8"\u0073\u0323\u0307") == SV({ u8"\u0073\u0323\u0307" }));
	EXPECT(split_characters(u8"\u0073\u0307\u0323") == SV({ u8"\u0073\u0307\u0323" }));
	EXPECT(split_characters(u8"\u1e0b\u0323"      ) == SV({ u8"\u1e0b\u0323"       }));
	EXPECT(split_characters(u8"\u0064\u0323\u0307") == SV({ u8"\u0064\u0323\u0307" }));
	EXPECT(split_characters(u8"\u0064\u0307\u0323") == SV({ u8"\u0064\u0307\u0323" }));
	EXPECT(split_characters(u8"\u0064\u0307\u0323") == SV({ u8"\u0064\u0307\u0323" }));
	EXPECT(split_characters(u8"\u0071\u0307\u0323") == SV({ u8"\u0071\u0307\u0323" }));
	EXPECT(split_characters(u8"\u0071\u0323\u0307") == SV({ u8"\u0071\u0323\u0307" }));
}

TEST(examples_from_tr15_normalized) {
	auto norm_split = [](const std::string &string) {
		return split_characters(normalize(string));
	};
	EXPECT(norm_split(u8"x\u212by"            ) == SV({ "x", u8"\u00c5", "y"             }));
	EXPECT(norm_split(u8"x\u0041\u030ay"      ) == SV({ "x", u8"\u00c5", "y"             }));
	EXPECT(norm_split(u8"x\u2126y"            ) == SV({ "x", u8"\u03a9", "y"             }));
	EXPECT(norm_split(u8"x\u006f\u0302y"      ) == SV({ "x", u8"\u00f4", "y"             }));
	EXPECT(norm_split(u8"x\u0073\u0323\u0307y") == SV({ "x", u8"\u1e69", "y"             }));
	EXPECT(norm_split(u8"x\u0073\u0307\u0323y") == SV({ "x", u8"\u1e69", "y"             }));
	EXPECT(norm_split(u8"x\u1e0b\u0323y"      ) == SV({ "x", u8"\u1e0d\u0307", "y"       }));
	EXPECT(norm_split(u8"x\u0064\u0323\u0307y") == SV({ "x", u8"\u1e0d\u0307", "y"       }));
	EXPECT(norm_split(u8"x\u0064\u0307\u0323y") == SV({ "x", u8"\u1e0d\u0307", "y"       }));
	EXPECT(norm_split(u8"x\u0064\u0307\u0323y") == SV({ "x", u8"\u1e0d\u0307", "y"       }));
	EXPECT(norm_split(u8"x\u0071\u0307\u0323y") == SV({ "x", u8"\u0071\u0323\u0307", "y" }));
	EXPECT(norm_split(u8"x\u0071\u0323\u0307y") == SV({ "x", u8"\u0071\u0323\u0307", "y" }));
}
