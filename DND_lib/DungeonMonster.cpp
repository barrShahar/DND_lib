#include "Monster.h"

namespace dnd_game
{
Monster::Monster(Number a_maxHealth, Number a_ttackDmg, const std::string& a_adressedName)
	: m_healthPoints { a_maxHealth }
	, m_attackDmg { a_ttackDmg }
	, m_adressedName { a_adressedName }
{
	// Ctor
}

Number Monster::DamageAndAttack(Number a_hitPoints)
{

	m_healthPoints = (m_healthPoints < a_hitPoints) ? 0 : m_healthPoints - a_hitPoints;
	return (m_healthPoints == 0) ? 0 : m_attackDmg; // If dragon is dead he cannot attack back
}

bool Monster::IsAlive() const
{
	return m_healthPoints > 0;
}

Number Monster::GetHP() const
{
	return m_healthPoints;
}

void Monster::SetHP(Number a_hp)
{
	m_healthPoints = a_hp;
}

std::unique_ptr<AttackResponse> Monster::TakeDamage(Number damage)
{
	Number hp = GetHP();
	SetHP(std::max(0, hp - damage));
	return std::make_unique<AttackResponse>(m_adressedName, IsInPlay(), hp, GetHP(), ReturnedDamage(damage));
}

bool Monster::IsInPlay() const
{
	return GetHP() > 0;
}

}
