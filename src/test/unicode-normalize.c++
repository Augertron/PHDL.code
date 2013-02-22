#include "test.h++"

#include "phdl/private.h++"

using namespace phdl::unicode;

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

TEST(replacing_replacement_character) {
	EXPECT(normalize(u8"wx\ufffdyz") == u8"wx\ufffdyz"); // D+FFFD
}

