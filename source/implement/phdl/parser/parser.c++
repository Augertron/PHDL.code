#include <phdl/parser.h++>

namespace phdl { namespace parser {

	bool parse_whitespace(Context &context) {
		bool success = false;
		while (unicode::is_whitespace(*context)) {
			success = true;
			++context;
		}
		return success;
	}

	//static std::string parse_multi_line_comment(Context &context) {
	//	parse_whitespace(context);

	//return "";
	//}

	//static std::string parse_comment(const Characters &characters) {
	//	return "";
	//}

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

}}
