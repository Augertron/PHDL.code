namespace phdl { namespace parser {

	template <typename Result, typename Parser>
	boost::optional<Result> Context::match (Parser &parser) {
		try {
			Context context;
			auto result = parser(context);
			*this = std::move(context);
			return result;
		} catch (const phdl::User_Visible_Error &) {
			return boost::none;
		}
	}

	template <typename Result, typename Parser>
	Result Context::expect (Parser &parser) {
		return parser(*this);
	}

}}
