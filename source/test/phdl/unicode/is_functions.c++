#include "test.h++"

#include <phdl/unicode.h++>

using phdl::unicode::is_whitespace;
using phdl::unicode::is_newline;

TEST(some_valid_whitespace) {
	EXPECT(is_whitespace(u8" "      ));
	EXPECT(is_whitespace(u8"\t"     ));
	EXPECT(is_whitespace(u8"\n"     ));
	EXPECT(is_whitespace(u8"\u0085" ));
	EXPECT(is_whitespace(u8"\v"     ));
	EXPECT(is_whitespace(u8"\r"     ));
	EXPECT(is_whitespace(u8"\r\n"   ));
}

TEST(some_invalid_whitespace) {
	EXPECT(!is_whitespace(u8"a"         ));
	EXPECT(!is_whitespace(u8"\0"        ));
	EXPECT(!is_whitespace(u8"\x10"      ));
	EXPECT(!is_whitespace(u8"\x7f"      ));
	EXPECT(!is_whitespace(u8"\u210D"    ));
	EXPECT(!is_whitespace(u8"\u29bf"    ));
	EXPECT(!is_whitespace(u8"\U00011680"));
	EXPECT(!is_whitespace(u8"\U000E0100"));
}

TEST(all_valid_newlines) {
	EXPECT(is_newline(u8"\u000a"      )); // LF
	EXPECT(is_newline(u8"\u000b"      )); // VT
	EXPECT(is_newline(u8"\u000c"      )); // FF
	EXPECT(is_newline(u8"\u000d"      )); // CR
	EXPECT(is_newline(u8"\u0085"      )); // NEL
	EXPECT(is_newline(u8"\u2028"      )); // LS
	EXPECT(is_newline(u8"\u2029"      )); // PS
	EXPECT(is_newline(u8"\u000d\u000a")); // CR+LF
}

TEST(some_invalid_newlines) {
	EXPECT(!is_newline(u8" "         ));
	EXPECT(!is_newline(u8"\t"        ));
	EXPECT(!is_newline(u8"\u0009"    ));
	EXPECT(!is_newline(u8"\u001b"    ));
	EXPECT(!is_newline(u8"\u341c"    ));
	EXPECT(!is_newline(u8"\u000e"    ));
	EXPECT(!is_newline(u8"\u0084"    ));
	EXPECT(!is_newline(u8"\u0086"    ));
	EXPECT(!is_newline(u8"\u2027"    ));
	EXPECT(!is_newline(u8"\u2030"    ));
	EXPECT(!is_newline(u8"\U0010e743"));
}
