#include <phdl/unicode.h++>

namespace phdl { namespace unicode {

	std::string combine_characters(const Characters &characters) {
		// Much different from the complexity needed for splitting, combining
		// is just a matter of pasting the characters back together.
		std::string string;
		for (auto c : characters) string.append(c);
		return string;
	}

}}
