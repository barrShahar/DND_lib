#include "UserParser.h"
namespace dnd_game
{
UserParser::UserParser()
	: m_stringToCommands {}
{
	for (size_t i = 0; i < static_cast< size_t >(COMMAND::SIZE); ++i)
	{
		m_stringToCommands.insert(std::make_pair(COMMAND_STRING[i], static_cast< COMMAND >(i)));
	}
}

std::optional<COMMAND> UserParser::ParseUserInput(const std::string& a_untrusted)
{
	decltype(m_stringToCommands)::iterator it = m_stringToCommands.find(a_untrusted);
	return (it != m_stringToCommands.end()) ? std::optional<COMMAND>(it->second) : std::nullopt;
}

}	// dnd_game