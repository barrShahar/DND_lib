#pragma once
#include "Monster.h"
#include "GameParams.h"
#include <string.h>
namespace dnd_game
{
static constexpr Number DRAGON_MAX_DAMAGE = 10;
static constexpr Number DRAGON_INIT_LIFE_POINTS = 100;

class Dragon : public Monster
{
public:
    explicit Dragon();
    Dragon(const Dragon& a_other) = delete;
    Dragon& operator=(const Dragon& a_other) = delete;
    Dragon(Dragon&&) = default;
    ~Dragon() = default;

    virtual std::vector<Direction> BlcokedDirections() const;
    virtual std::string GetName() const;

private:
     std::string m_name;
};
}  // namespace dnd_game