#include "private.h++"

#include <iostream>

void print_string(const std::string &what, const std::string &s) {
	std::cout << what << " = '" << s << "'\n";
}

int main(int argc, char **argv) {

	using namespace phdl::unicode;

	for (int i=1; i<argc; ++i) {
		std::string input(argv[i]);
		std::cout << "Argument #" << i << '\n';
		print_string("Input",input);
		validate(input);
		auto converted = to_UTF32(input);
		auto normalized = normalize(converted);
		auto converted8 = to_UTF8(normalized);
		print_string("Normalized",converted8);
	}

}
