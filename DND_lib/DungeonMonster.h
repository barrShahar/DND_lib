#pragma once
#include "GameParams.h"
namespace dnd_game
{
class DungeonMonster
{
public:
    explicit DungeonMonster(Number a_maxHealth, Number a_attackDmg);
    DungeonMonster(const DungeonMonster& a_other) = delete;
    DungeonMonster& operator=(const DungeonMonster& a_other) = delete;
    ~DungeonMonster() = default;

    Number DamageAndAttack(Number a_hitPoints);
    bool IsAlive() const;

    Number GetLifePoints() const;
private:
    Number m_lifePoints;
    Number m_attackDmg;
};
}  // namespace dnd_game
