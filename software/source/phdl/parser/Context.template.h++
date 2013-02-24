namespace phdl { namespace parser {

	template <typename Result, typename Parser>
	optional<Result> Context::match (Parser &parser) {
		try {
			Context context;
			auto result = parser(context);
			*this = std::move(context);
			return result;
		} catch (const Parse_Error &) {
			return none;
		}
	}

	template <typename Result, typename Parser>
	Result Context::expect (Parser &parser) {
		return parser(*this);
	}

}}
