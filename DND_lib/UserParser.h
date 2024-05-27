#pragma once
#include "GameParams.h"
#include <optional>
#include <string>
#include <unordered_map>

namespace dnd_game
{
class UserParser
{
public:
    explicit UserParser();
    UserParser(const UserParser& a_other) = delete;
    UserParser& operator=(const UserParser& a_other) = delete;
    ~UserParser() = default;

    std::optional<COMMAND> ParseUserInput(const std::string& a_untrusted);
private:

    std::unordered_map<std::string, COMMAND> m_stringToCommands;
};

}	// dnd_game