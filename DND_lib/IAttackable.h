#pragma once
#include <memory>
#include "GameParams.h"
#include "AttackResponse.h"

namespace dnd_game
{
// Interface for all entities that can be attacked
class IAttackable
{
public:
    virtual ~IAttackable() = default;
    virtual std::unique_ptr<AttackResponse> TakeDamage(Number a_damage);
    virtual Number ReturnedDamage(Number a_damage) const = 0;
    virtual bool IsInPlay() const = 0;

    virtual const std::string& GetName() const = 0;
    virtual Number GetHP() const = 0;
    virtual void SetHP(Number a_hp) = 0;
};

} // namespace dnd_game
