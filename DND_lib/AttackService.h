#pragma once
#include "Player.h"
namespace dnd_game
{
class AttackService
{
	AttackService() = delete;
	AttackService(const AttackService& a_other) = delete;
	AttackService& operator=(const AttackService& a_other) = delete;
	~AttackService() = default;

	static bool AttackEntity(Player& attacker, IAttackable& target);
};
}	// namespace dnd_game

