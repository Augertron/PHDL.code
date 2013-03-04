#include <phdl/error.h++>
#include <phdl/parser.h++>
#include <phdl/unicode.h++>

namespace phdl { namespace parser {

	std::shared_ptr<Characters> read_file(const std::string &filename) {
		// Read the content of the given file.
		std::ifstream ifs(
			filename.c_str(),
			std::ios_base::in | std::ios_base::binary
		);

		if (!ifs) {
			Context context(filename, std::shared_ptr<Characters>());
			context.parse_error("could not open file");
		}
		std::ostringstream ss;
		ss << ifs.rdbuf();

		// Convert to characters and store as a shared pointer so that
		// multiple context objects can efficiently share the same content.
		return std::shared_ptr<Characters>(new unicode::Characters (
			unicode::split_characters(unicode::normalize(ss.str()))
		));
	}

}}
