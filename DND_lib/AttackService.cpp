#include "AttackService.h"

bool dnd_game::AttackService::AttackEntity(Player& attacker, IAttackable& target)
{
    target.TakeDamage(attacker.GetDmgPoints());
    // Notification and other logic can go here
    return true;
}
