#include "test.h++"

using namespace phdl::unicode;

TEST(valid_sequences_work) {
	validate(u8"abcd1234");
	validate(u8"  composed > é <");
	validate(u8"decomposed > é <");
	validate(u8"BMP > βでØ∝ᚠ <");
	validate(u8"SMP > 𐄻𝀉圖𒀼𐠜 <");
}

void expect_error(const std::string &input, size_t position) {
	try {
		validate(input);
		EXPECT(false);
	} catch (const Validation_Error &e) {
		EXPECT(e.position == position);
	}
}

TEST(invalid_sequences_rejected) {
	expect_error("\xffwxyz", 0);
	expect_error("w\xffxyz", 1);
	expect_error("wx\xffyz", 2);
	expect_error("wxy\xffz", 3);
	expect_error("wxyz\xff", 4);
}

TEST(long_sequences_rejected) {
	validate    ("wx\x0ayz");                        // U+A
	expect_error("wx\xc0\x8ayz", 2);                 // U+A (long)
	expect_error("wx\xe0\x80\x8ayz", 2);             // U+A (long)
	expect_error("wx\xf0\x80\x80\x8ayz", 2);         // U+A (long)
	expect_error("wx\xf8\x80\x80\x80\x8ayz", 2);     // U+A (long)
	expect_error("wx\xfc\x80\x80\x80\x80\x8ayz", 2); // U+A (long)
	expect_error("wx\xf8\x86\xb0\xb0\xb0yz", 2);     // U+110000
	expect_error("wx\xfc\x83\xbf\xbf\xbf\xbfyz", 2); // U+3FFFFFF
}

TEST(surrogates_rejected) {
	validate    ("wx\xea\xa0\x80yz");    // U+A800
	expect_error("wx\xed\xa0\x80yz", 2); // D+D800
	validate    ("wx\xea\xb0\x80yz");    // U+AC00
	expect_error("wx\xed\xb0\x80yz", 2); // D+DC00
	validate    ("wx\xea\xae\x80yz");    // U+AB80
	expect_error("wx\xed\xae\x80yz", 2); // D+DB80
}

