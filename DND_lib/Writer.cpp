#include "Writer.h"

namespace dnd_game
{
Writer& dnd_game::Writer::operator<<(const std::string& a_str)
{
    Write(a_str);
    return *this;
}
Writer & Writer::operator<<(const char a_char)
{
    Write(a_char);
    return *this;
}
} // dnd_game
