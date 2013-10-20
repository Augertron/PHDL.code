#include "test.h++"

#include <phdl/unicode.h++>

#include <iostream>

#include <boost/format.hpp>

void print_string_bytes(const std::string &s) {
	std::cerr << "[";
	for (char c : s) {
		std::cerr << boost::format(" %02x") % (int(c)& 0xff);
	}
	std::cerr << " ]\n";
}

using phdl::unicode::normalize;

TEST(basic_normalization_works) {
	EXPECT(normalize(u8"ABCDabcd1234¹²³⁴₁₂₃₄") == u8"ABCDabcd1234¹²³⁴₁₂₃₄"); // already NFC
	EXPECT(normalize(u8"\u00e9xpose\u0301") == u8"\u00e9xpos\u00e9"); // éxposé
}

TEST(misc_BMP_and_SMP_codepoints) {
	EXPECT(
		normalize("\xce\xb2\xe3\x81\xa7\xc3\x98\xe2\x88\x9d\xe1\x9a\xa0")
		==        "\xce\xb2\xe3\x81\xa7\xc3\x98\xe2\x88\x9d\xe1\x9a\xa0"
	); // βでØ∝ᚠ
	EXPECT(
		normalize("\xf0\x90\x84\xbb\xf0\x9d\x80\x89""\xf0\xaf\xa1\x8b""\xf0\x92\x80\xbc\xf0\x90\xa0\x9c")
		==        "\xf0\x90\x84\xbb\xf0\x9d\x80\x89""\xe5\x9c\x96"    "\xf0\x92\x80\xbc\xf0\x90\xa0\x9c"
	); // 𐄻𝀉圖𒀼𐠜
}

TEST(examples_from_tr15) {
	EXPECT(normalize(u8"wx\u212byz"            ) == u8"wx\u00c5yz"            );
	EXPECT(normalize(u8"wx\u0041\u030ayz"      ) == u8"wx\u00c5yz"            );
	EXPECT(normalize(u8"wx\u2126yz"            ) == u8"wx\u03a9yz"            );
	EXPECT(normalize(u8"wx\u006f\u0302yz"      ) == u8"wx\u00f4yz"            );
	EXPECT(normalize(u8"wx\u0073\u0323\u0307yz") == u8"wx\u1e69yz"            );
	EXPECT(normalize(u8"wx\u0073\u0307\u0323yz") == u8"wx\u1e69yz"            );
	EXPECT(normalize(u8"wx\u1e0b\u0323yz"      ) == u8"wx\u1e0d\u0307yz"      );
	EXPECT(normalize(u8"wx\u0064\u0323\u0307yz") == u8"wx\u1e0d\u0307yz"      );
	EXPECT(normalize(u8"wx\u0064\u0307\u0323yz") == u8"wx\u1e0d\u0307yz"      );
	EXPECT(normalize(u8"wx\u0064\u0307\u0323yz") == u8"wx\u1e0d\u0307yz"      );
	EXPECT(normalize(u8"wx\u0071\u0307\u0323yz") == u8"wx\u0071\u0323\u0307yz");
	EXPECT(normalize(u8"wx\u0071\u0323\u0307yz") == u8"wx\u0071\u0323\u0307yz");
}

TEST(invalid_sequences_replaced) {
	EXPECT(normalize("\xffwxyz") == u8"\ufffdwxyz");
	EXPECT(normalize("w\xffxyz") == u8"w\ufffdxyz");
	EXPECT(normalize("wx\xffyz") == u8"wx\ufffdyz");
	EXPECT(normalize("wxy\xffz") == u8"wxy\ufffdz");
	EXPECT(normalize("wxyz\xff") == u8"wxyz\ufffd");
}

TEST(standard_recommended_replacement_conversion_policy) {
	std::string before =   "\x61""\xf1\x80\x80""\xe1\x80""\xc2"  "\x62""\x80"  "\x63""\x80"  "\xbf"  "\x64";
	std::string after  = u8"\x61""\ufffd"      "\ufffd"  "\ufffd""\x62""\ufffd""\x63""\ufffd""\ufffd""\x64";
	EXPECT(normalize(before) == after);
}

TEST(long_sequences_replaced) {
	EXPECT(normalize("wx\xc0\x8ayz"                ) == u8"wx\ufffdyz"); // U+A (long)
	EXPECT(normalize("wx\xe0\x80\x8ayz"            ) == u8"wx\ufffdyz"); // U+A (long)
	EXPECT(normalize("wx\xf0\x80\x80\x8ayz"        ) == u8"wx\ufffdyz"); // U+A (long)
	EXPECT(normalize("wx\xf8\x80\x80\x80\x8ayz"    ) == u8"wx\ufffdyz"); // U+A (long)
	EXPECT(normalize("wx\xfc\x80\x80\x80\x80\x8ayz") == u8"wx\ufffdyz"); // U+A (long)
	EXPECT(normalize("wx\xf8\x86\xb0\xb0\xb0yz"    ) == u8"wx\ufffdyz"); // U+110000
	EXPECT(normalize("wx\xfc\x83\xbf\xbf\xbf\xbfyz") == u8"wx\ufffdyz"); // U+3FFFFFF
}

TEST(multiple_invalid_sequences) {
	EXPECT(normalize("\xc0wx\xfc\xf9yz\x8a")     == u8"\ufffdwx\ufffd\ufffdyz\ufffd");
	EXPECT(normalize("\xfc\x83wx\x86\xf8yz\x80") == u8"\ufffdwx\ufffd\ufffdyz\ufffd");
}

TEST(surrogates_replaced) {
	EXPECT(normalize("wx\xed\xa0\x80yz") == u8"wx\ufffdyz"); // U+D800
	EXPECT(normalize("wx\xed\xb0\x80yz") == u8"wx\ufffdyz"); // U+DC00
	EXPECT(normalize("wx\xed\xae\x80yz") == u8"wx\ufffdyz"); // U+DB80
}

TEST(replacing_replacement_codepoints) {
	EXPECT(normalize(u8"wx\ufffdyz")       == u8"wx\ufffdyz"      ); // U+FFFD
	EXPECT(normalize(u8"wx\ufffd\ufffdyz") == u8"wx\ufffd\ufffdyz"); // U+FFFD
}

TEST(reserved_codepoints_are_untouched) {
	// These codepoints are reserved as of Unicode 6.2 and are unlikely to be
	// assigned in the forseeable future.
	EXPECT(normalize(u8"wx\U000A7778yz") == u8"wx\U000A7778yz"); // U+A7778
	EXPECT(normalize(u8"wx\U000BABCDyz") == u8"wx\U000BABCDyz"); // U+BABCD
	EXPECT(normalize(u8"wx\U000C1234yz") == u8"wx\U000C1234yz"); // U+C1234
}

TEST(noncharacters_are_untouched) {
	EXPECT(normalize(u8"wx\ufdd2yz")     == u8"wx\ufdd2yz"    ); // U+FDD2
	EXPECT(normalize(u8"wx\ufffeyz")     == u8"wx\ufffeyz"    ); // U+FFFE
	EXPECT(normalize(u8"wx\uffffyz")     == u8"wx\uffffyz"    ); // U+FFFF
	EXPECT(normalize(u8"wx\U0001fffeyz") == u8"wx\U0001fffeyz"); // U+1FFFE
	EXPECT(normalize(u8"wx\U0006ffffyz") == u8"wx\U0006ffffyz"); // U+6FFFF
	EXPECT(normalize(u8"wx\U0010fffeyz") == u8"wx\U0010fffeyz"); // U+10FFFE
	EXPECT(normalize(u8"wx\U0010ffffyz") == u8"wx\U0010ffffyz"); // U+10FFFF
}

TEST(private_use_codepoints_are_untouched) {
	EXPECT(normalize(u8"wx\uf51ayz")     == u8"wx\uf51ayz"    ); // U+F51A
	EXPECT(normalize(u8"wx\U000f2a82yz") == u8"wx\U000f2a82yz"); // U+F2A82
}

