#include "test.h++"

using namespace phdl::parser::position;
using namespace phdl::unicode;

Characters text(const std::string &text) {
	return split_characters(text);
}

TEST(basic_line_numbers) {
	EXPECT(line_number(text("abcd\nwxyz\n1234\n"),  0) == 1);
	EXPECT(line_number(text("abcd\nwxyz\n1234\n"),  1) == 1);
	EXPECT(line_number(text("abcd\nwxyz\n1234\n"),  2) == 1);
	EXPECT(line_number(text("abcd\nwxyz\n1234\n"),  3) == 1);
	EXPECT(line_number(text("abcd\nwxyz\n1234\n"),  4) == 1);
	EXPECT(line_number(text("abcd\nwxyz\n1234\n"),  5) == 2);
	EXPECT(line_number(text("abcd\nwxyz\n1234\n"),  6) == 2);
	EXPECT(line_number(text("abcd\nwxyz\n1234\n"),  7) == 2);
	EXPECT(line_number(text("abcd\nwxyz\n1234\n"),  8) == 2);
	EXPECT(line_number(text("abcd\nwxyz\n1234\n"),  9) == 2);
	EXPECT(line_number(text("abcd\nwxyz\n1234\n"), 10) == 3);
	EXPECT(line_number(text("abcd\nwxyz\n1234\n"), 11) == 3);
	EXPECT(line_number(text("abcd\nwxyz\n1234\n"), 12) == 3);
	EXPECT(line_number(text("abcd\nwxyz\n1234\n"), 13) == 3);
	EXPECT(line_number(text("abcd\nwxyz\n1234\n"), 14) == 3);
	EXPECT(line_number(text("abcd\nwxyz\n1234\n"), 15) == 4);
	EXPECT(line_number(text("abcd\nwxyz\n1234\n"), 16) == 4);
}

TEST(basic_line_numbers_with_CRLF) {
	EXPECT(line_number(text("abcd\r\nwxyz\r\n1234\r\n"),  0) == 1);
	EXPECT(line_number(text("abcd\r\nwxyz\r\n1234\r\n"),  1) == 1);
	EXPECT(line_number(text("abcd\r\nwxyz\r\n1234\r\n"),  2) == 1);
	EXPECT(line_number(text("abcd\r\nwxyz\r\n1234\r\n"),  3) == 1);
	EXPECT(line_number(text("abcd\r\nwxyz\r\n1234\r\n"),  4) == 1);
	EXPECT(line_number(text("abcd\r\nwxyz\r\n1234\r\n"),  5) == 2);
	EXPECT(line_number(text("abcd\r\nwxyz\r\n1234\r\n"),  6) == 2);
	EXPECT(line_number(text("abcd\r\nwxyz\r\n1234\r\n"),  7) == 2);
	EXPECT(line_number(text("abcd\r\nwxyz\r\n1234\r\n"),  8) == 2);
	EXPECT(line_number(text("abcd\r\nwxyz\r\n1234\r\n"),  9) == 2);
	EXPECT(line_number(text("abcd\r\nwxyz\r\n1234\r\n"), 10) == 3);
	EXPECT(line_number(text("abcd\r\nwxyz\r\n1234\r\n"), 11) == 3);
	EXPECT(line_number(text("abcd\r\nwxyz\r\n1234\r\n"), 12) == 3);
	EXPECT(line_number(text("abcd\r\nwxyz\r\n1234\r\n"), 13) == 3);
	EXPECT(line_number(text("abcd\r\nwxyz\r\n1234\r\n"), 14) == 3);
	EXPECT(line_number(text("abcd\r\nwxyz\r\n1234\r\n"), 15) == 4);
	EXPECT(line_number(text("abcd\r\nwxyz\r\n1234\r\n"), 16) == 4);
}

TEST(line_numbers_with_no_newline_at_end) {
	EXPECT(line_number(text("abcd\nwxyz\r\n1234"), 14) == 3);
	EXPECT(line_number(text("abcd\nwxyz\r\n1234"), 15) == 3);
	EXPECT(line_number(text("abcd\nwxyz\r\n1234"), 16) == 3);
}

TEST(basic_column_numbers) {
	EXPECT(column_number(text("abcd\nwxyz\n1234\n"),  0) == 1);
	EXPECT(column_number(text("abcd\nwxyz\n1234\n"),  1) == 2);
	EXPECT(column_number(text("abcd\nwxyz\n1234\n"),  2) == 3);
	EXPECT(column_number(text("abcd\nwxyz\n1234\n"),  3) == 4);
	EXPECT(column_number(text("abcd\nwxyz\n1234\n"),  4) == 5);
	EXPECT(column_number(text("abcd\nwxyz\n1234\n"),  5) == 1);
	EXPECT(column_number(text("abcd\nwxyz\n1234\n"),  6) == 2);
	EXPECT(column_number(text("abcd\nwxyz\n1234\n"),  7) == 3);
	EXPECT(column_number(text("abcd\nwxyz\n1234\n"),  8) == 4);
	EXPECT(column_number(text("abcd\nwxyz\n1234\n"),  9) == 5);
	EXPECT(column_number(text("abcd\nwxyz\n1234\n"), 10) == 1);
	EXPECT(column_number(text("abcd\nwxyz\n1234\n"), 11) == 2);
	EXPECT(column_number(text("abcd\nwxyz\n1234\n"), 12) == 3);
	EXPECT(column_number(text("abcd\nwxyz\n1234\n"), 13) == 4);
	EXPECT(column_number(text("abcd\nwxyz\n1234\n"), 14) == 5);
	EXPECT(column_number(text("abcd\nwxyz\n1234\n"), 15) == 1);
	EXPECT(column_number(text("abcd\nwxyz\n1234\n"), 16) == 1);
}

TEST(column_numbers_with_no_newline_at_end) {
	EXPECT(column_number(text("abcd\nwxyz\r\n1234"), 14) == 5);
	EXPECT(column_number(text("abcd\nwxyz\r\n1234"), 15) == 5);
	EXPECT(column_number(text("abcd\nwxyz\r\n1234"), 16) == 5);
}

TEST(line_content) {
	EXPECT(line_content(text("abcd\nwxyz\n1234\n"),  0) == "abcd");
	EXPECT(line_content(text("abcd\nwxyz\n1234\n"),  1) == "abcd");
	EXPECT(line_content(text("abcd\nwxyz\n1234\n"),  2) == "abcd");
	EXPECT(line_content(text("abcd\nwxyz\n1234\n"),  3) == "abcd");
	EXPECT(line_content(text("abcd\nwxyz\n1234\n"),  4) == "abcd");
	EXPECT(line_content(text("abcd\nwxyz\n1234\n"),  5) == "wxyz");
	EXPECT(line_content(text("abcd\nwxyz\n1234\n"),  6) == "wxyz");
	EXPECT(line_content(text("abcd\nwxyz\n1234\n"),  7) == "wxyz");
	EXPECT(line_content(text("abcd\nwxyz\n1234\n"),  8) == "wxyz");
	EXPECT(line_content(text("abcd\nwxyz\n1234\n"),  9) == "wxyz");
	EXPECT(line_content(text("abcd\nwxyz\n1234\n"), 10) == "1234");
	EXPECT(line_content(text("abcd\nwxyz\n1234\n"), 11) == "1234");
	EXPECT(line_content(text("abcd\nwxyz\n1234\n"), 12) == "1234");
	EXPECT(line_content(text("abcd\nwxyz\n1234\n"), 13) == "1234");
	EXPECT(line_content(text("abcd\nwxyz\n1234\n"), 14) == "1234");
	EXPECT(line_content(text("abcd\nwxyz\n1234\n"), 15) == "");
	EXPECT(line_content(text("abcd\nwxyz\n1234\n"), 16) == "");
}

TEST(line_content_no_newline_at_end) {
	EXPECT(line_content(text("abcd\nwxyz\r\n1234"), 14) == "1234");
	EXPECT(line_content(text("abcd\nwxyz\r\n1234"), 15) == "1234");
	EXPECT(line_content(text("abcd\nwxyz\r\n1234"), 16) == "1234");
}

TEST(line_pointer) {
	EXPECT(line_pointer(text("abcd\nwxyz\n1234\n"),  0) == "^");
	EXPECT(line_pointer(text("abcd\nwxyz\n1234\n"),  1) == " ^");
	EXPECT(line_pointer(text("abcd\nwxyz\n1234\n"),  2) == "  ^");
	EXPECT(line_pointer(text("abcd\nwxyz\n1234\n"),  3) == "   ^");
	EXPECT(line_pointer(text("abcd\nwxyz\n1234\n"),  4) == "    ^");
	EXPECT(line_pointer(text("abcd\nwxyz\n1234\n"),  5) == "^");
	EXPECT(line_pointer(text("abcd\nwxyz\n1234\n"),  6) == " ^");
	EXPECT(line_pointer(text("abcd\nwxyz\n1234\n"),  7) == "  ^");
	EXPECT(line_pointer(text("abcd\nwxyz\n1234\n"),  8) == "   ^");
	EXPECT(line_pointer(text("abcd\nwxyz\n1234\n"),  9) == "    ^");
	EXPECT(line_pointer(text("abcd\nwxyz\n1234\n"), 10) == "^");
	EXPECT(line_pointer(text("abcd\nwxyz\n1234\n"), 11) == " ^");
	EXPECT(line_pointer(text("abcd\nwxyz\n1234\n"), 12) == "  ^");
	EXPECT(line_pointer(text("abcd\nwxyz\n1234\n"), 13) == "   ^");
	EXPECT(line_pointer(text("abcd\nwxyz\n1234\n"), 14) == "    ^");
	EXPECT(line_pointer(text("abcd\nwxyz\n1234\n"), 15) == "^");
	EXPECT(line_pointer(text("abcd\nwxyz\n1234\n"), 16) == "^");
}

TEST(line_pointer_no_newline_at_end) {
	EXPECT(line_pointer(text("abcd\nwxyz\r\n1234"), 14) == "    ^");
	EXPECT(line_pointer(text("abcd\nwxyz\r\n1234"), 15) == "    ^");
	EXPECT(line_pointer(text("abcd\nwxyz\r\n1234"), 16) == "    ^");
}
