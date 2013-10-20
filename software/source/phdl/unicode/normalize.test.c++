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
	EXPECT(normalize(u8"abcd1234")          == u8"abcd1234"        );
	EXPECT(normalize(u8"\u00e9xpose\u0301") == u8"\u00e9xpos\u00e9"); // éxposé -> éxposé
}

TEST(misc_BMP_and_SMP_characters) {
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

TEST(long_sequences_replaced_once) {
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

TEST(standard_recommended_replacement_conversion_policy) {
	std::string before =   "\x61""\xf1\x80\x80""\xe1\x80""\xc2"  "\x62""\x80"  "\x63""\x80"  "\xbf"  "\x64";
	std::string after  = u8"\x61""\ufffd"      "\ufffd"  "\ufffd""\x62""\ufffd""\x63""\ufffd""\ufffd""\x64";
	EXPECT(normalize(before) == after);
}

TEST(surrogates_replaced) {
	EXPECT(normalize("wx\xed\xa0\x80yz") == u8"wx\ufffdyz"); // D+D800
	EXPECT(normalize("wx\xed\xb0\x80yz") == u8"wx\ufffdyz"); // D+DC00
	EXPECT(normalize("wx\xed\xae\x80yz") == u8"wx\ufffdyz"); // D+DB80
}

TEST(replacing_replacement_characters) {
	EXPECT(normalize(u8"wx\ufffdyz") == u8"wx\ufffdyz"); // D+FFFD
}

TEST(unassigned_characters_are_untouched) {
	EXPECT(normalize(u8"wx\ufffeyz")     == u8"wx\ufffeyz"    ); // D+FFFE
	EXPECT(normalize(u8"wx\uffffyz")     == u8"wx\uffffyz"    ); // D+FFFF
	EXPECT(normalize(u8"wx\U0010fffeyz") == u8"wx\U0010fffeyz"); // D+10FFFE
	EXPECT(normalize(u8"wx\U0010ffffyz") == u8"wx\U0010ffffyz"); // D+10FFFF
}

TEST(private_use_characters_are_untouched) {
	EXPECT(normalize(u8"wx\uf51ayz")     == u8"wx\uf51ayz"    ); // D+F51A
	EXPECT(normalize(u8"wx\U000f2a82yz") == u8"wx\U000f2a82yz"); // D+F2A82
}

