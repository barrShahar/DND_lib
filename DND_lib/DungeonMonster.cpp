#include "Monster.h"

namespace dnd_game
{
Monster::Monster(Number a_maxHealth, Number a_ttackDmg)
	: m_lifePoints { a_maxHealth }
	, m_attackDmg { a_ttackDmg }
{
	// Ctor
}

Number Monster::DamageAndAttack(Number a_hitPoints)
{

	m_lifePoints = (m_lifePoints < a_hitPoints) ? 0 : m_lifePoints - a_hitPoints;
	return (m_lifePoints == 0) ? 0 : m_attackDmg; // If dragon is dead he cannot attack back
}

bool Monster::IsAlive() const
{
	return m_lifePoints > 0;
}

Number Monster::GetLifePoints() const
{
	return m_lifePoints;
}

}
