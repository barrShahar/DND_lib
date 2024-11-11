#include "UserParser.h"
#include <sstream>

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

ParsedCommand UserCommandParser::ParseCommand(const std::string& a_untrusted) const
{
	std::istringstream stream(a_untrusted);
	std::string commandString;
	stream >> commandString;  // Extract the command
	stream >> std::ws; 	// Skip any remaining whitespace

	decltype(m_stringToCommands)::const_iterator it = m_stringToCommands.find(commandString); 

	
	std::string message;
	std::getline(stream, message);  // Capture the remaining input as the message and flush t

	if (it != m_stringToCommands.end())
	{
		return {std::optional<COMMAND>(it->second), message};
	}
	
	// else
	return { std::nullopt, "" };
}


ParsedCommand::ParsedCommand(std::optional<COMMAND> a_command, std::string a_arguments)
	: m_command { a_command }
	, m_arguments { a_arguments }
{
	// CTOR
}

std::optional<COMMAND> ParsedCommand::GetCommand()
{
	return m_command;
}

std::string ParsedCommand::GetArguments()
{
	return m_arguments;
}

}	// dnd_game