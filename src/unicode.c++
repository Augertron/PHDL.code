#include "private.h++"

namespace phdl { namespace unicode {

	void validate(const std::string &input) {

		const uint8_t * error_location = u8_check(
			reinterpret_cast<const uint8_t *>(input.data()), input.size()
		);

		if (error_location) {
			// FIXME implement me correctly
			perror("u8_check");
			assert(false && "UTF-8 error");
		}

	}

	std::u32string to_UTF32(const std::string &input) {

		size_t converted_size = 0;
		uint32_t *converted = u8_to_u32 (
			reinterpret_cast<const uint8_t *>(input.data()), input.size(),
			nullptr, &converted_size
		);

		if (!converted) {
			// FIXME implement me correctly
			perror("u8_check");
			assert(false && "u8_to_u32");
		}
		std::u32string result(
			reinterpret_cast<const char32_t *>(converted), converted_size
		);

		free(converted);
		return result;

	}

	std::string to_UTF8 (const std::u32string &input) {

		size_t converted_size = 0;
		uint8_t *converted = u32_to_u8 (
			reinterpret_cast<const uint32_t *>(input.data()), input.size(),
			nullptr, &converted_size
		);

		if (!converted) {
			// FIXME implement me correctly
			perror("u8_check");
			assert(false && "u32_to_u8");
		}
		std::string result(
			reinterpret_cast<const char *>(converted), converted_size
		);

		free(converted);
		return result;

	}

	std::u32string normalize(const std::u32string &input) {

		size_t normalized_size = 0;
		uint32_t *normalized = u32_normalize (
			UNINORM_NFC,
			reinterpret_cast<const uint32_t *>(input.data()), input.size(),
			nullptr, &normalized_size
		);

		if (!normalized) {
			// FIXME implement me correctly
			perror("u8_check");
			assert(false && "u32_normalize");
		}
		std::u32string result(
			reinterpret_cast<const char32_t *>(normalized), normalized_size
		);

		free(normalized);
		return result;

	}

}} // namespace phdl::unicode
