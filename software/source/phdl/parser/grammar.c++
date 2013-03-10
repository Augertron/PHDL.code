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
				if (*context != c) {
					start.throw_error("expected literal " + value);
				}
				++context;
			}
			return {};
		};
	}
	
	ignored end_of_file(Context &context) {
		if (context->size() != 0) context.throw_error("expected end-of-file");
		++context;
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
			auto m1 = MATCH(context,actual_whitespace);
			auto m2 = MATCH(context,comment);
			if (m1 || m2) success = true;
			else {
				if (success) break;
				context.throw_error("expected whitespace (or comment)");
			}
		}
		return {};
	}

	ast::Index index(Context &context) {
		MATCH(context, whitespace);
		ast::Index index;
		index.context = context;
		while (true) {
			if (
				   (context->size() == 1)
				&& (("0" <= *context) && (*context <= "9"))
			) {
				index.digits += *context;
				++context;
			} else if (index.digits.size() == 0) {
				index.context.throw_error("expected index");
			} else {
				break;
			}
		}
		return index;
	}

	ast::Range range(Context &context) {
		MATCH(context, whitespace);
		ast::Range range;
		range.context = context;
		auto index1 = REQUIRE(context, index);

		MATCH(context, whitespace);
		if (!MATCH(context,literal(":"))) {
			range.index = index1;
			return range;
		}

		MATCH(context, whitespace);
		auto index2 = REQUIRE(context, index);
		range.index = std::make_pair(index1, index2);
		return range;
	}

	ast::Slice slice(Context &context) {
		MATCH(context, whitespace);
		ast::Slice slice;
		slice.context = context;
		if (!MATCH(context,literal("["))) {
			slice.context.throw_error("expected slice");
		}

		MATCH(context, whitespace);
		auto range1 = REQUIRE(context, range);
		slice.ranges.push_back(range1);

		while (true) {
			MATCH(context, whitespace);
			if (MATCH(context, literal("]"))) {
				return slice;
			} else if (!MATCH(context, literal(","))) {
				context.throw_error("expected , for next range or ] for end-of-slice");
			}

			MATCH(context, whitespace);
			auto rangeN = REQUIRE(context, range);
			slice.ranges.push_back(rangeN);
		}
		return slice;
	}

	std::string unquoted_name(Context &) {
		std::string name;
		return name;
	}

	std::string quoted_name(Context &context) {
		std::string name;

		REQUIRE(context, literal("\""));
		while (true) {
			if (MATCH(context, literal("\""))) {
				if (name.size() == 0) context.throw_error("name may not be empty");
				else return name;
			} else if (MATCH(context, literal("\\"))) {
				if      (MATCH(context, literal("\\"))) name += "\\";
				else if (MATCH(context, literal("\""))) name += "\"";
				else if (MATCH(context, literal("x{"))) {
					std::string digits;
					while (true) {
						if (MATCH(context, literal("}"))) {
							if (digits.size() == 0) context.throw_error("expected Unicode codepoint");
							// FIXME: compute Unicode codepoint from digits correctly
							// FIXME: append computed codepoint to name
							break;
						}
						digits += *context;
						++context;
					}
				} else {
					context.throw_error("invalid escape in quoted name");
				}
			}
			name += *context;
			++context;
		}

		return name;
	}

	ast::Name name(Context &context) {
		ast::Name name;
		name.context = context;
		return name;
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
