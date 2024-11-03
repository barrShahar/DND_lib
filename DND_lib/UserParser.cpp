#include "UserParser.h"
namespace dnd_game
{
UserCommandParser::UserCommandParser()
	: m_stringToCommands {}
{
	for (size_t i = 0; i < static_cast< size_t >(COMMAND::SIZE); ++i)
	{
		m_stringToCommands.insert(std::make_pair(COMMAND_STRING[i], static_cast< COMMAND >(i)));
	}
}

std::optional<COMMAND> UserCommandParser::ParseCommand(const std::string& a_untrusted) const
{
	decltype(m_stringToCommands)::const_iterator it = m_stringToCommands.find(a_untrusted);	// using auto was forbidden in this project
	return (it != m_stringToCommands.end()) ? std::optional<COMMAND>(it->second) : std::nullopt;
}

}	// dnd_game