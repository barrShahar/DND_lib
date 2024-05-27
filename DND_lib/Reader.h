#pragma once
#include <string>

namespace dnd_game
{
class Reader
{
public:
    explicit Reader() = default;
    Reader(const Reader& a_other) = delete;
    Reader& operator=(const Reader& a_other) = delete;
    ~Reader() = default;

    virtual std::string ReadLine() const = 0;
private:
};
}  // namespace dnd_game