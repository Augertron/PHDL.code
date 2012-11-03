#ifndef __phdlif__header__
#define __phdlif__header__

#include <iostream>
#include <map>
#include <string>
#include <stdexcept>

namespace phdlif {

	struct Error : std::runtime_error {};

	using Name  = std::string;
	using Value = std::string;

	struct Attribute {
		Name  name;
		Value value;
	};
	using Attributes = std::map<Name,Attribute>;

	struct Pin {
		Name       name;
		Attributes attributes;
	};
	using Pins = std::map<Name,Pin>;

	struct Instance {
		Name       name;
		Pins       pins;
		Attributes attributes;
	};
	using Instances = std::map<Name,Instance>;

	struct Connection {
		Name       instance;
		Name       pin;
		Attributes attributes;
	};
	using Connections = std::map<Name,Connection>;

	struct Net {
		Name        name;
		Connections connections;
		Attributes  attributes;
	};

	struct Design {
		Name       name;
		Instances  instances;
		Nets       nets;
		Attributes attributes;
	};

	Design  read_phdlif(const std::istream &);
	void   write_phdlif(const std::ostream &);

}

#endif
