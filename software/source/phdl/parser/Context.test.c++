#include "test.h++"

#include <phdl/parser.h++>
#include <phdl/error.h++>
#include <phdl/unicode.h++>

using phdl::User_Visible_Error;
using phdl::parser::Context;
using phdl::parser::read_file;
using phdl::unicode::Characters;
using phdl::unicode::split_characters;
using phdl::unicode::combine_characters;

TEST(default_constructor_leaves_context_in_valid_state) {
	Context context;
	REQUIRE(context.filename() == "");
	REQUIRE(context.text()->size() == 0);
	REQUIRE(context.position() == 0);
	REQUIRE(*context == "");
	REQUIRE(context->size() == 0);
	REQUIRE(combine_characters(*context.text()) == "");
}

struct Make_Test_Files {
	Make_Test_Files() {
		{
			std::ofstream ofs("tmp-parser-context-001.tmp");
			ofs << "abc 123\r\nxyz 789\n";
		}
		{
			std::ofstream ofs("tmp-parser-context-002.tmp");
			ofs << "abc 123\r\nxyz 789\n";
		}
	}
} _make_test_files;

struct Fixture {
	Context context;
	Fixture() :
		context("tmp-parser-context-001.tmp", read_file("tmp-parser-context-001.tmp"))
	{}
};

TEST_METHOD(Fixture, filename_is_correct) {
	EXPECT(context.filename() == "tmp-parser-context-001.tmp");
}

TEST_METHOD(Fixture, text_is_correct) {
	REQUIRE(context.text());
	EXPECT(*context.text() == split_characters("abc 123\r\nxyz 789\n"));
}

TEST_METHOD(Fixture, basic_positioning) {
	EXPECT(context.position() == 0);
	context.set_position(5);
	EXPECT(context.position() == 5);
	context.set_position(103);
	EXPECT(context.position() == 103);
}

TEST_METHOD(Fixture, reading_characters_with_offset) {
	context.set_position(5);
	EXPECT(context[-7] == "");
	EXPECT(context[-6] == "");
	EXPECT(context[-5] == "a");
	EXPECT(context[-4] == "b");
	EXPECT(context[-3] == "c");
	EXPECT(context[-2] == " ");
	EXPECT(context[-1] == "1");
	EXPECT(context[ 0] == "2");
	EXPECT(context[ 1] == "3");
	EXPECT(context[ 2] == "\r\n");
	EXPECT(context[ 3] == "x");
	EXPECT(context[ 4] == "y");
	EXPECT(context[ 5] == "z");
	EXPECT(context[ 6] == " ");
	EXPECT(context[ 7] == "7");
	EXPECT(context[ 8] == "8");
	EXPECT(context[ 9] == "9");
	EXPECT(context[10] == "\n");
	EXPECT(context[11] == "");
	EXPECT(context[12] == "");
}

TEST_METHOD(Fixture, reading_characters_at_position) {
	EXPECT(context[0] == "a");
	EXPECT(*context   == "a");
	EXPECT(context->front() == 'a');
	context.set_position(9);
	EXPECT(context[0] == "y");
	EXPECT(*context   == "y");
	EXPECT(context->front() == 'y');
}

TEST_METHOD(Fixture, advancing_position) {
	EXPECT(*context   == "a");
	EXPECT(*++context == "b");
	EXPECT(*context   == "b");
	EXPECT(*context++ == "b");
	EXPECT(*context   == "c");

	EXPECT(*context   == "c");
	EXPECT(*--context == "b");
	EXPECT(*context   == "b");
	EXPECT(*context-- == "b");
	EXPECT(*context   == "a");

	EXPECT(*(context += 10) == "z");
	EXPECT(*context         == "z");
	EXPECT(*(context -=  3) == "\r\n");
	EXPECT(*context         == "\r\n");
}

TEST_METHOD(Fixture, copying_a_context) {
	context.set_position(7);
	EXPECT(context.position() == 7);

	Context context2(context);
	EXPECT(context2.filename() == context.filename());
	EXPECT(context2.position() == context.position());

	context2.set_position(2);
	EXPECT(context2.filename() == context.filename());
	EXPECT(context .position() == 7);
	EXPECT(context2.position() == 2);
}

TEST_METHOD(Fixture, assigning_and_comparing) {
	Context context2(context);
	EXPECT(context   ==   context2);
	EXPECT(context++ ==   context2);
	EXPECT(context   == ++context2);
	EXPECT(++context !=   context2);
	EXPECT(context   !=   context2);

	context = context2;
	EXPECT(context   ==   context2);
}

TEST_METHOD(Fixture, works_without_reading_file) {
	Context context2("tmp-parser-context-001.tmp", std::make_shared<Characters>(split_characters("abc 123\r\nxyz 789\n")));
	Context context3("tmp-parser-context-002.tmp", std::make_shared<Characters>(split_characters("abc 123\r\nxyz 789\n")));
	EXPECT(context2 == context);
	EXPECT(context3 != context);
}

TEST_METHOD(Fixture, not_equal_if_files_differ) {
	Context context2("tmp-parser-context-002.tmp", read_file("tmp-parser-context-002.tmp"));
	EXPECT(context != context2);
}

TEST_METHOD(Fixture, fail_if_file_not_found) {
	try {
		Context context("tmp-parser-context-999.tmp", read_file("tmp-parser-context-999.tmp"));
		EXPECT(false);
	} catch (const User_Visible_Error &error) {
		std::cout << error;
		EXPECT(true);
	}
}

TEST(basic_error_messages) {
	Context context("file.phdl", std::make_shared<Characters>(split_characters("abc\r\nωéヽђ\n")));

	std::string messages[] = {
		"file.phdl:1:1: error: some error message\n"
		"file.phdl:1:1: context: abc\n"
		"file.phdl:1:1: context: ^\n",

		"file.phdl:1:2: error: some error message\n"
		"file.phdl:1:2: context: abc\n"
		"file.phdl:1:2: context:  ^\n",

		"file.phdl:1:3: error: some error message\n"
		"file.phdl:1:3: context: abc\n"
		"file.phdl:1:3: context:   ^\n",

		"file.phdl:1:4: error: some error message\n"
		"file.phdl:1:4: context: abc\n"
		"file.phdl:1:4: context:    ^\n",

		"file.phdl:2:1: error: some error message\n"
		"file.phdl:2:1: context: ωéヽђ\n"
		"file.phdl:2:1: context: ^\n",

		"file.phdl:2:2: error: some error message\n"
		"file.phdl:2:2: context: ωéヽђ\n"
		"file.phdl:2:2: context:  ^\n",

		"file.phdl:2:3: error: some error message\n"
		"file.phdl:2:3: context: ωéヽђ\n"
		"file.phdl:2:3: context:   ^\n",

		"file.phdl:2:4: error: some error message\n"
		"file.phdl:2:4: context: ωéヽђ\n"
		"file.phdl:2:4: context:    ^\n",

		"file.phdl:2:5: error: some error message\n"
		"file.phdl:2:5: context: ωéヽђ\n"
		"file.phdl:2:5: context:     ^\n",

		"file.phdl:3:1: error: some error message\n",

		"file.phdl:3:1: error: some error message\n",
	};

	for (auto msg : messages) {
		try {
			context.throw_error("some error message");
			REQUIRE(false);
		} catch (const User_Visible_Error &error) {
			std::ostringstream ss;
			ss << error;
			EXPECT(ss.str() == msg);
			++context;
		}
	}
}

