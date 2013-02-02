#include "test.h++"

using namespace phdl::unicode;

TEST(basic_normalization_works) {
	REQUIRE(normalize(u8"abcd1234") == u8"abcd1234");
	REQUIRE(normalize(u8"éxpose\u0301") == u8"éxposé");
}

TEST(examples_from_tr15) {
	EXPECT(normalize(u8"wx\u212byz") == u8"wx\u00c5yz");
	EXPECT(normalize(u8"wx\u0041\u030ayz") == u8"wx\u00c5yz");
	EXPECT(normalize(u8"wx\u2126yz") == u8"wx\u03a9yz");
	EXPECT(normalize(u8"wx\u006f\u0302yz") == u8"wx\u00f4yz");
	EXPECT(normalize(u8"wx\u0073\u0323\u0307yz") == u8"wx\u1e69yz");
	EXPECT(normalize(u8"wx\u0073\u0307\u0323yz") == u8"wx\u1e69yz");
	EXPECT(normalize(u8"wx\u1e0b\u0323yz") == u8"wx\u1e0d\u0307yz");
	EXPECT(normalize(u8"wx\u0064\u0323\u0307yz") == u8"wx\u1e0d\u0307yz");
	EXPECT(normalize(u8"wx\u0064\u0307\u0323yz") == u8"wx\u1e0d\u0307yz");
	EXPECT(normalize(u8"wx\u0064\u0307\u0323yz") == u8"wx\u1e0d\u0307yz");
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

TEST(long_sequences_replaced) {
	EXPECT(normalize("wx\xc0\x8ayz"                ) == u8"wx\ufffdyz"); // U+A (long)
	EXPECT(normalize("wx\xe0\x80\x8ayz"            ) == u8"wx\ufffdyz"); // U+A (long)
	EXPECT(normalize("wx\xf0\x80\x80\x8ayz"        ) == u8"wx\ufffdyz"); // U+A (long)
	EXPECT(normalize("wx\xf8\x80\x80\x80\x8ayz"    ) == u8"wx\ufffdyz"); // U+A (long)
	EXPECT(normalize("wx\xfc\x80\x80\x80\x80\x8ayz") == u8"wx\ufffdyz"); // U+A (long)
	EXPECT(normalize("wx\xf8\x86\xb0\xb0\xb0yz"    ) == u8"wx\ufffdyz"); // U+110000
	EXPECT(normalize("wx\xfc\x83\xbf\xbf\xbf\xbfyz") == u8"wx\ufffdyz"); // U+3FFFFFF
}

TEST(surrogates_replaced) {
	EXPECT(normalize("wx\xed\xa0\x80yz") == u8"wx\ufffdyz"); // D+D800
	EXPECT(normalize("wx\xed\xb0\x80yz") == u8"wx\ufffdyz"); // D+DC00
	EXPECT(normalize("wx\xed\xae\x80yz") == u8"wx\ufffdyz"); // D+DB80
}

