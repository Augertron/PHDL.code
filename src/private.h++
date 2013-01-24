// standard headers
#include <cassert>
#include <string>

// libunistring headers
#include <uninorm.h>
#include <unistr.h>

namespace phdl {
	namespace unicode {
		void validate(const std::string &input);
		std::u32string to_UTF32(const std::   string &input);
		std::   string to_UTF8 (const std::u32string &input);
		std::u32string normalize(const std::u32string &input);
	} // namespace phdl::unicode
} // namespace phdl
