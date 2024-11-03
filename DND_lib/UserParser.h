#pragma once
#include "GameParams.h"
#include <optional>
#include <string>
#include <unordered_map>

namespace dnd_game
{

class ParsedCommand
{
public:
    ParsedCommand() = delete;
    ParsedCommand(std::optional<COMMAND> a_command, std::string a_arguments);
    ParsedCommand(const ParsedCommand& a_other) = delete;
    ParsedCommand& operator=(const ParsedCommand& a_other) = delete;
    ~ParsedCommand() = default;

    std::optional<COMMAND> GetCommand();
    std::string GetArguments();

private:
    std::optional<COMMAND> m_command;
    std::string m_arguments;
    
};

class UserCommandParser
{
public:
    explicit UserCommandParser();
    UserCommandParser(const UserCommandParser& a_other) = delete;
    UserCommandParser& operator=(const UserCommandParser& a_other) = delete;
    ~UserCommandParser() = default;

    ParsedCommand ParseCommand(const std::string& a_untrusted) const;
private:

    std::unordered_map<std::string, COMMAND> m_stringToCommands;
};

}	// dnd_game