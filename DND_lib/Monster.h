#pragma once
#include <vector>
#include <string>
#include "GameParams.h"
#include "IAttackable.h"

namespace dnd_game
{
class Monster : public IAttackable 
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
    virtual const std::string& GetName() const = 0;

    Number GetHP() const;
    void SetHP(Number a_hp);

    virtual void TakeDamage(Number damage);
    virtual bool IsInPlay() const;

private:
    Number m_healthPoints;
    Number m_attackDmg;
};
}  // namespace dnd_game
