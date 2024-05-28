#pragma once
#include "GameParams.h"
#include <optional>
#include <string>
#include <unordered_map>

namespace dnd_game
{
class UserCommandParser
{
public:
    explicit UserCommandParser();
    UserCommandParser(const UserCommandParser& a_other) = delete;
    UserCommandParser& operator=(const UserCommandParser& a_other) = delete;
    ~UserCommandParser() = default;

    std::optional<COMMAND> ParseCommand(const std::string& a_untrusted);
private:

    std::unordered_map<std::string, COMMAND> m_stringToCommands;
};

}	// dnd_game