#include "test.h++"

#include <phdl/unicode.h++>

using phdl::unicode::normalize;
using phdl::unicode::split_characters;
using C = phdl::unicode::Characters;

TEST(basic_splitting_works) {
	EXPECT(split_characters(u8"abcd") == C({ "a", "b", "c", "d" }));
	EXPECT(split_characters(u8"\u00e9xpose\u0301") == C({
		"\u00e9", "x", "p", "o", "s", "e\u0301"
	}));
}

TEST(newline_characters) {
	EXPECT(split_characters(u8"\n\n\r\r\n\r") == C({ "\n", "\n", "\r", "\r\n", "\r" }));
}

TEST(examples_from_uax29) {
	EXPECT(split_characters(u8"\u0067\u0308"      ) == C({ u8"\u0067\u0308"       }));
	EXPECT(split_characters(u8"\u1100\u1161\u11a8") == C({ u8"\u1100\u1161\u11a8" }));
	EXPECT(split_characters(u8"\u0ba8\u0bbf"      ) == C({ u8"\u0ba8\u0bbf"       }));
	EXPECT(split_characters(u8"\u0e01\u0e33"      ) == C({ u8"\u0e01\u0e33"       }));
	EXPECT(split_characters(u8"\u0937\u093f"      ) == C({ u8"\u0937\u093f"       }));
}

TEST(examples_from_tr15) {
	EXPECT(split_characters(u8"\u212b"            ) == C({ u8"\u212b"             }));
	EXPECT(split_characters(u8"\u0041\u030a"      ) == C({ u8"\u0041\u030a"       }));
	EXPECT(split_characters(u8"\u2126"            ) == C({ u8"\u2126"             }));
	EXPECT(split_characters(u8"\u006f\u0302"      ) == C({ u8"\u006f\u0302"       }));
	EXPECT(split_characters(u8"\u0073\u0323\u0307") == C({ u8"\u0073\u0323\u0307" }));
	EXPECT(split_characters(u8"\u0073\u0307\u0323") == C({ u8"\u0073\u0307\u0323" }));
	EXPECT(split_characters(u8"\u1e0b\u0323"      ) == C({ u8"\u1e0b\u0323"       }));
	EXPECT(split_characters(u8"\u0064\u0323\u0307") == C({ u8"\u0064\u0323\u0307" }));
	EXPECT(split_characters(u8"\u0064\u0307\u0323") == C({ u8"\u0064\u0307\u0323" }));
	EXPECT(split_characters(u8"\u0064\u0307\u0323") == C({ u8"\u0064\u0307\u0323" }));
	EXPECT(split_characters(u8"\u0071\u0307\u0323") == C({ u8"\u0071\u0307\u0323" }));
	EXPECT(split_characters(u8"\u0071\u0323\u0307") == C({ u8"\u0071\u0323\u0307" }));
}

TEST(examples_from_tr15_normalized) {
	auto norm_split = [](const std::string &string) {
		return split_characters(normalize(string));
	};
	EXPECT(norm_split(u8"x\u212by"            ) == C({ "x", u8"\u00c5", "y"             }));
	EXPECT(norm_split(u8"x\u0041\u030ay"      ) == C({ "x", u8"\u00c5", "y"             }));
	EXPECT(norm_split(u8"x\u2126y"            ) == C({ "x", u8"\u03a9", "y"             }));
	EXPECT(norm_split(u8"x\u006f\u0302y"      ) == C({ "x", u8"\u00f4", "y"             }));
	EXPECT(norm_split(u8"x\u0073\u0323\u0307y") == C({ "x", u8"\u1e69", "y"             }));
	EXPECT(norm_split(u8"x\u0073\u0307\u0323y") == C({ "x", u8"\u1e69", "y"             }));
	EXPECT(norm_split(u8"x\u1e0b\u0323y"      ) == C({ "x", u8"\u1e0d\u0307", "y"       }));
	EXPECT(norm_split(u8"x\u0064\u0323\u0307y") == C({ "x", u8"\u1e0d\u0307", "y"       }));
	EXPECT(norm_split(u8"x\u0064\u0307\u0323y") == C({ "x", u8"\u1e0d\u0307", "y"       }));
	EXPECT(norm_split(u8"x\u0064\u0307\u0323y") == C({ "x", u8"\u1e0d\u0307", "y"       }));
	EXPECT(norm_split(u8"x\u0071\u0307\u0323y") == C({ "x", u8"\u0071\u0323\u0307", "y" }));
	EXPECT(norm_split(u8"x\u0071\u0323\u0307y") == C({ "x", u8"\u0071\u0323\u0307", "y" }));
}
