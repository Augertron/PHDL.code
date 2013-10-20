#include <phdl/error.h++>
#include <phdl/unicode.h++>

// Standard
#include <limits>

// ICU headers
#include <unicode/brkiter.h>

namespace phdl { namespace unicode {

	Characters split_characters(const std::string &string) {
		// FIXME: ICU makes us use int32_t for positions, instead of size_t.
		// We can get around this by doing something more complex, but for
		// now, we're just going to make sure this works and hope that we
		// never see a string that's too long.
		uint64_t max_length = std::numeric_limits<int32_t>::max();
		dynamic_assert(string.size() <= max_length, "can't handle long strings");
		int32_t length = int32_t(string.size());

		// Create a UText object to let our string to work break iterator
		UErrorCode icu_error = U_ZERO_ERROR;
		UText *string_utext = utext_openUTF8(
			nullptr,
			string.data(),
			string.size(),
			&icu_error
		);
		dynamic_assert(U_SUCCESS(icu_error), "couldn't create UText");

		// Create a break iterator
		auto *break_iterator = icu::BreakIterator::createCharacterInstance(
			icu::Locale::getRoot(),
			icu_error
		);
		dynamic_assert(U_SUCCESS(icu_error), "couldn't create break iterator");

		// Point the break iterator at our string
		break_iterator->setText(
			string_utext,
			icu_error
		);
		dynamic_assert(U_SUCCESS(icu_error), "error setting text on break iterator");

		// Iterate through all of the boundaries and grab all of the
		// characters between them.
		int32_t start = break_iterator->first();
		Characters characters;
		while (start < length) {
			int32_t end = break_iterator->next();
			if (end == icu::BreakIterator::DONE) end = string.size();
			characters.push_back(std::string(
				string.begin() + start,
				string.begin() + end
			));
			start = end;
		}

		// Delete our break iterator
		delete break_iterator;

		// Delete our UText
		utext_close(string_utext);

		// Return the results
		return characters;
	}

	// Much different from the complexity needed for splitting, combining
	// is just a matter of pasting the characters back together.
	std::string combine_characters(const Characters &characters) {
		std::string string;
		for (auto c : characters) string.append(c);
		return string;
	}

}}

