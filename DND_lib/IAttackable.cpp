#include "IAttackable.h"

namespace dnd_game
{
std::unique_ptr<AttackResponse> dnd_game::IAttackable::TakeDamage(Number a_damage)
{
	Number originalHp = GetHP();
	SetHP(std::max(0, originalHp - a_damage));
	return std::make_unique<AttackResponse>(GetName(), 
										    IsInPlay(), 
											originalHp, GetHP(), 
											ReturnedDamage(a_damage));
}
} // namespace dnd_game
