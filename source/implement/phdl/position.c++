#include "phdl/private.h++"

namespace phdl { namespace position {

	using unicode::Character;
	using unicode::Characters;

	static bool  is_newline(const Character &c) { return  unicode::is_newline(c); };
	static bool not_newline(const Character &c) { return !unicode::is_newline(c); };

	size_t line_number(const Characters &text, size_t position) {
		// We can easily determine the line number of our current position by
		// counting the newlines that occur before our current position.
		//
		// Line numbers probably ought to start at 0, but are conventially
		// numbered starting at 1. So we start counting there to be more
		// friendly to our users.
		auto start = text.begin();
		auto end   = text.begin() + std::min(position, text.size());
		return std::count_if(start, end, is_newline) + 1;
	}

	size_t column_number(const Characters &text, size_t position) {
		// We can determine our column number very easily by counting the
		// characters that occur before our position up to the previous
		// newline.
		//
		// Column numbers probably ought to start at 0, but are conventially
		// numbered starting at 1. So we start counting there to be more
		// friendly to our users.
		auto rstart = text.rbegin() + (text.size() - std::min(position, text.size()));
		auto rend   = std::find_if(rstart, text.rend(), is_newline);
		return std::count_if(rstart, rend, not_newline) + 1;
	}

	std::string line_content(const Characters &text, size_t position) {
		// We can get the content of the current line by finding the previous
		// newline and the next newline and grabbing everything between them.
		auto rstart = text.rbegin() + (text.size() - std::min(position, text.size()));
		auto rend   = std::find_if(rstart, text.rend(), is_newline);
		auto start = text.begin() + std::min(position, text.size());
		auto end   = std::find_if(start, text.end(), is_newline);
		Characters line;
		std::copy(rend.base(), end, std::back_inserter(line));
		return unicode::combine_characters(line);
	}

	std::string line_pointer(const Characters &text, size_t position) {
		// All we need to generate our line pointer is to get the column
		// number, which we already have a function to do conveniently. Then,
		// we just line up our pointer with the right number of spaces.
		size_t column = column_number(text, position);
		std::string line;
		std::fill_n(std::back_inserter(line), column-1, ' ');
		line.push_back('^');
		return line;
	}

}}
