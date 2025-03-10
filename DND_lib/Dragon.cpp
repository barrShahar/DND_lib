#include "Dragon.h"


namespace dnd_game
{
Dragon::Dragon()
	: Monster(Number(DRAGON_INIT_LIFE_POINTS), Number(DRAGON_MAX_DAMAGE), "The Dragon")
	, m_name { "Dragon"}
{
	// Ctor
}

std::vector<Direction> Dragon::BlcokedDirections() const
{
	return { Direction::NORTH };
}

const std::string& Dragon::GetName() const
{
	return m_name;
}

Number Dragon::ReturnedDamage(Number a_damage) const
{
	return 30;
}



}	// namespace dnd_game
