#pragma once
#include <vector>
#include <string>
#include "GameParams.h"

namespace dnd_game
{
class Monster
{
public:
    explicit Monster(Number a_maxHealth, Number a_attackDmg);
    Monster(const Monster& a_other) = delete;
    Monster& operator=(const Monster& a_other) = delete;
    // Defaulted or custom move constructor
    Monster(Monster&&) = default;
    ~Monster() = default;

    Number DamageAndAttack(Number a_hitPoints);
    bool IsAlive() const;
    virtual std::vector<Direction> BlcokedDirections() const = 0;
    virtual std::string GetName() const = 0;    

    Number GetLifePoints() const;
private:
    Number m_lifePoints;
    Number m_attackDmg;
};
}  // namespace dnd_game
