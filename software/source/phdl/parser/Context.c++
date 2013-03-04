#include <phdl/error.h++>
#include <phdl/parser.h++>
#include <phdl/unicode.h++>

namespace phdl { namespace parser {

	struct Context::Detail {
		std::string filename;
		std::shared_ptr<unicode::Characters> text;
		size_t position = 0;
	};

	Context::~Context() {}

	Context::Context(
		const std::string &filename,
		std::shared_ptr<Characters> text
	) :
		detail(new Detail)
	{
		// Keep track of the filename & text
		detail->filename = filename;
		detail->text     = text;
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

	std::string Context::filename() const {
		return detail->filename;
	}

	const std::shared_ptr<Characters> Context::text() const {
		return detail->text;
	}

	size_t Context::position() const {
		return detail->position;
	}

	void Context::set_position(size_t new_position) {
		detail->position = new_position;
	}

	const unicode::Character &Context::operator[](int offset) const {
		// Access the character from the text. If we're out of bounds, return
		// an placeholder empty character instead.
		try {
			return detail->text->at(position() + offset);
		} catch (const std::out_of_range &) {
			static const std::string empty_character;
			return empty_character;
		}
	}

	const unicode::Character &Context::operator*() const {
		return (*this)[0];
	}

	const unicode::Character *Context::operator->() const {
		return &(*this)[0];
	}

	Context &Context::operator++() { ++detail->position; return *this; };
	Context &Context::operator--() { --detail->position; return *this; };

	Context  Context::operator++(int) { Context t(*this); ++(*this); return t; }
	Context  Context::operator--(int) { Context t(*this); --(*this); return t; }

	Context &Context::operator+=(int offset) { detail->position += offset; return *this; }
	Context &Context::operator-=(int offset) { detail->position -= offset; return *this; }

	void Context::parse_error (
		const std::string &message,
		boost::optional<const User_Visible_Error> wrapped
	) {

		throw User_Visible_Error (
			phdl::error::Severity::Error,
			filename(), text(), position(),
			message,
			boost::optional<const User_Visible_Error &>(wrapped)
		);

	}

}}
