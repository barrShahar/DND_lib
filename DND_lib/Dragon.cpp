#include "Dragon.h"


namespace dnd_game
{
Dragon::Dragon()
	: Monster(Number(DRAGON_INIT_LIFE_POINTS), Number(DRAGON_MAX_DAMAGE))
{
	// Ctor
}
std::vector<Direction> Dragon::BlcokedDirections() const
{
	return { Direction::NORTH };
}

}	// namespace dnd_game
