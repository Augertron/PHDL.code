#include "phdl/private.h++"

namespace phdl { namespace parser {

	struct Context::Detail {
		std::string filename;
		std::shared_ptr<Characters> text;
		size_t position;
	};

	Context::~Context() {}

	Context::Context(const std::string &filename) :
		detail(new Detail)
	{
		// Keep track of the filename
		detail->filename = filename;

		// Read the content of the given file.
		std::ifstream ifs(
			filename.c_str(),
			std::ios_base::in | std::ios_base::binary
		);
		// FIXME: throw exception here to generate the right error
		if (!ifs) throw std::runtime_error("file opening failed");
		std::ostringstream ss;
		ss << ifs.rdbuf();

		// Convert to characters and store as a shared pointer so that
		// multiple context objects can efficiently share the same content.
		detail->text.reset(new Characters(
			unicode::split_characters(unicode::normalize(ss.str()))
		));

		// Start at position 0
		detail->position = 0;
	}

	Context::Context(const Context &other) 
		: detail(new Detail(*other.detail))
	{}

	Context &Context::operator=(const Context &other) {
		*detail = *other.detail;
		return *this;
	}

	bool Context::operator==(const Context &other) {
		const Detail &detail1 = *detail;
		const Detail &detail2 = *other.detail;
		return
			(detail1.filename == detail2.filename) &&
			(detail1.position == detail2.position);
	}

	bool Context::operator!=(const Context &other) {
		return !(*this == other);
	}

	// Get the filename.
	std::string Context::filename() const {
		return detail->filename;
	}

	size_t Context::position() const {
		return detail->position;
	}

	void Context::set_position(size_t new_position) {
		detail->position = new_position;
	}

	const Character &Context::operator[](int offset) const {
		// Access the character from the text. If we're out of bounds, return
		// an placeholder empty character instead.
		try {
			return detail->text->at(position() + offset);
		} catch (const std::out_of_range &) {
			static const std::string empty_character;
			return empty_character;
		}
	}

	const Character &Context::operator*() const {
		return (*this)[0];
	}

	const Character *Context::operator->() const {
		return &(*this)[0];
	}

	Context &Context::operator++() { ++detail->position; return *this; };
	Context &Context::operator--() { --detail->position; return *this; };

	Context  Context::operator++(int) { Context t(*this); ++(*this); return t; }
	Context  Context::operator--(int) { Context t(*this); --(*this); return t; }

	Context &Context::operator+=(int offset) { detail->position += offset; return *this; }
	Context &Context::operator-=(int offset) { detail->position -= offset; return *this; }

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
