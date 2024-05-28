#pragma once
#include <string>
namespace dnd_game
{
class Writer
{
public:
    explicit Writer() = default;
    Writer(const Writer& a_other) = delete;
    Writer& operator=(const Writer& a_other) = delete;
    virtual ~Writer() = default;


    virtual void Write(const std::string& a_str) = 0;
    virtual void Write(const char a_char) = 0;
    virtual void Endl() = 0;

    Writer& operator<<(const std::string& a_str);
    Writer& operator<<(const char a_char);

private:
};
}  // namespace dnd_game