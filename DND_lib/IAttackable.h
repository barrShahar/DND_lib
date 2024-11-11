#pragma once
#include "GameParams.h"

namespace dnd_game
{
    // Interface for all entities that can be attacked
    class IAttackable
    {
    public:
        virtual ~IAttackable() = default;
        virtual void TakeDamage(Number damage) = 0;
        virtual bool IsInPlay() const = 0;

        virtual const std::string& GetName() const = 0;
    };

} // namespace dnd_game
