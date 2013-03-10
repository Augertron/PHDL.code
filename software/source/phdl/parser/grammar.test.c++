#include "test.h++"
#include <phdl/parser.h++>
#include <phdl/unicode.h++>

using phdl::User_Visible_Error;
using phdl::parser::Context;
using phdl::unicode::Characters;
using namespace phdl::parser::grammar;

Context make_context(const std::string &filename, const std::string &text) {
	return Context (
		filename,
		std::shared_ptr<Characters>(new Characters(
			phdl::unicode::split_characters(phdl::unicode::normalize(text))
		))
	);
}

#define TEST_MATCH(rule, text, end_position)\
	TEST(rule) {\
		auto context = make_context(#rule ".phdl", text);\
		bool matched = false;\
		try {\
			rule(context);\
			matched = true;\
		} catch (const User_Visible_Error &) {}\
		EXPECT(matched);\
		EXPECT(context.position() == end_position);\
	}

#define TEST_NO_MATCH(rule, text)\
	TEST(rule) {\
		auto context = make_context(#rule ".phdl", text);\
		bool matched = false;\
		try {\
			rule(context);\
			matched = true;\
		} catch (const User_Visible_Error &) {}\
		EXPECT(!matched);\
		EXPECT(context.position() == 0);\
	}

TEST_MATCH   (literal("abcd"), "abcd", 4);
TEST_MATCH   (literal("abcd"), "abcdefg", 4);
TEST_NO_MATCH(literal("abcd"), "abce");
TEST_NO_MATCH(literal("abcd"), "ax");
TEST_NO_MATCH(literal("abcd"), "x");
TEST_NO_MATCH(literal("abcd"), "");

TEST_MATCH   (end_of_file, "", 1);
TEST_NO_MATCH(end_of_file, "x");

TEST_MATCH   (actual_whitespace, "   ", 3);
TEST_MATCH   (actual_whitespace, " \t  \r", 5);
TEST_MATCH   (actual_whitespace, "\r\n",1);
TEST_MATCH   (actual_whitespace, "\r\nx",1);
TEST_NO_MATCH(actual_whitespace, "x");

TEST_MATCH   (single_line_comment, "//", 2);
TEST_MATCH   (single_line_comment, "//\n", 3);
TEST_MATCH   (single_line_comment, "// this is a comment", 20);
TEST_MATCH   (single_line_comment, "//comment\n", 10);
TEST_MATCH   (single_line_comment, "//comment\nxyz", 10);
TEST_MATCH   (single_line_comment, "//comm/*en*/t/*\nxyz", 16);
TEST_NO_MATCH(single_line_comment, "/not a comment\nxyz");
TEST_NO_MATCH(single_line_comment, "not a comment\nxyz");

TEST_MATCH   (multi_line_comment, "/**/1", 4);
TEST_MATCH   (multi_line_comment, "/* a\nb\nc */", 11);
TEST_MATCH   (multi_line_comment, "/*comment*/", 11);
TEST_MATCH   (multi_line_comment, "/*comment*//", 11);
TEST_MATCH   (multi_line_comment, "/*comment**/", 12);
TEST_MATCH   (multi_line_comment, "/*com//ment*/", 13);
TEST_MATCH   (multi_line_comment, "/*comm\nent*/xyz", 12);
TEST_NO_MATCH(multi_line_comment, "/not a comment\nxyz");
TEST_NO_MATCH(multi_line_comment, "*/not a comment\nxyz");
TEST_NO_MATCH(multi_line_comment, "not a comment\nxyz");

TEST_MATCH   (comment, "// single line comment", 22);
TEST_MATCH   (comment, "/* multi\n line\r comment */", 26);
TEST_MATCH   (comment, "///*single \n  */", 12);
TEST_MATCH   (comment, "/*//single \n  */", 16);
TEST_NO_MATCH(comment, "*/ not a comment");
TEST_NO_MATCH(comment, "not a comment");

TEST_MATCH   (whitespace, " /**/", 5);
TEST_MATCH   (whitespace, "/**/ ", 5);
TEST_MATCH   (whitespace, "//\n/**/", 7);
TEST_MATCH   (whitespace, "\n // \n/*\n*/\n  \n", 15);
TEST_MATCH   (whitespace, "// single line comment", 22);
TEST_MATCH   (whitespace, "// single line comment", 22);
TEST_MATCH   (whitespace, "/* multi\n line\r comment */", 26);
TEST_MATCH   (whitespace, "///*single \n  */", 14);
TEST_MATCH   (whitespace, "/*//single \n  */", 16);
TEST_MATCH   (whitespace, "  //abc\n\t\t/*xyz*/   abc", 20);
TEST_NO_MATCH(whitespace, "*/ not a comment or whitespace");
TEST_NO_MATCH(whitespace, "not a comment or whitespace");
