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
    Monster(Number a_maxHealth, Number a_ttackDmg, const std::string& a_adressedName);
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

    // Attackable interface
    virtual std::unique_ptr<AttackResponse> TakeDamage(Number damage);
    virtual bool IsInPlay() const;

private:
    Number m_healthPoints;
    Number m_attackDmg;
    std::string const m_adressedName;
};
}  // namespace dnd_game
