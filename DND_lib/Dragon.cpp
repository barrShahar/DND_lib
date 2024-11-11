#include "Dragon.h"


namespace dnd_game
{
Dragon::Dragon()
	: Monster(Number(DRAGON_INIT_LIFE_POINTS), Number(DRAGON_MAX_DAMAGE))
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



}	// namespace dnd_game
