#include <phdl/parser.h++>
#include <phdl/unicode.h++>

// Optionally match the current context with the given parser. If there is a
// match, the result is returned and the position is advanced. Otherwise, no
// result is returned, and the context stays unchanged.
#define MATCH(context, parser)\
	[&]() -> boost::optional<decltype(parser(context))> {\
		try {\
			Context subcontext(context);\
			auto result = parser(subcontext);\
			context = std::move(subcontext);\
			return result;\
		} catch (const phdl::User_Visible_Error &) {\
			return boost::none;\
		}\
	}()

// Require the current context to match the given parser and return the result.
// This is different from calling a parser directly with the current context
// because it guarantees that the context is rewound if an exception occurs.
#define REQUIRE(context, parser)\
	[&]() {\
		Context subcontext(context);\
		auto result = parser(subcontext);\
		context = std::move(subcontext);\
		return result;\
	}()

namespace phdl { namespace parser { namespace grammar {

	std::function<ignored(Context &)> literal(std::string value) {
		Characters characters = unicode::split_characters(unicode::normalize(value));
		return [=](Context &context) -> ignored {
			Context start(context);
			for (auto c : characters) {
				if (*context != c) start.throw_error("expected literal " + value);
				++context;
			}
			return {};
		};
	}
	
	ignored end_of_file(Context &context) {
		if (context->size() != 0) context.throw_error("expected end-of-file");
		return {};
	}

	ignored actual_whitespace(Context &context) {
		bool success = false;
		while (unicode::is_whitespace(*context)) {
			success = true;
			++context;
		}
		if (!success) context.throw_error("expected actual whitespace");
		return {};
	}

	ignored single_line_comment(Context &context) {
		try {
			REQUIRE(context,literal("//"));
			while (context->size() != 0) {
				bool end = false;
				if (unicode::is_newline(*context) || *context == "") end = true;
				++context;
				if (end) break;
			}
			return {};
		} catch (const User_Visible_Error &e) {
			context.throw_error("expected single-line comment", e);
		}
		return {};
	}

	ignored multi_line_comment(Context &context) {
		try {
			REQUIRE(context,literal("/*"));
			while (context->size() != 0) {
				if (MATCH(context,literal("*/"))) return {};
				++context;
			}
			context.throw_error("expected end of multi-line comment");
		} catch (const User_Visible_Error &e) {
			context.throw_error("expected multi-line comment", e);
		}
		return {};
	}

	ignored comment(Context &context) {
		auto m1 = MATCH(context,single_line_comment);
		auto m2 = MATCH(context,multi_line_comment);
		if (!(m1 || m2)) context.throw_error("expected comment");
		return {};
	}

	ignored whitespace(Context &context) {
		bool success = false;
		while (true) {
			if (!(
				   MATCH(context,actual_whitespace)
				|| MATCH(context,comment)
			)) {
				if (!success) context.throw_error("expected whitespace (or comment)");
				else return {};
			}
		}
	}

	//static std::string parse_identifier(const Characters &characters) {
	//	return "";
	//}

	//static std::string parse_string(const Characters &characters) {
	//	return "";
	//}

	//static uint32_t parse_number(const Characters &characters) {
	//	return 0;
	//}

	//static std::pair<uint32_t, uint32_t> parse_range(const Characters &characters) {
	//	return std::pair<uint32_t, uint32_t>(0, 0);
	//}

}}}
