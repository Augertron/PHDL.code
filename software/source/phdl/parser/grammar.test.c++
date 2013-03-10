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

TEST_MATCH   (single_line_comment, "// this is a comment", 20);
TEST_MATCH   (single_line_comment, "//comment\n", 10);
TEST_MATCH   (single_line_comment, "//comment\nxyz", 10);
