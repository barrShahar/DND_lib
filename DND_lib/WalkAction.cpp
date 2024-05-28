#include "Action.h"

namespace dnd_game
{
std::unique_ptr<ActionResponse> WalkAction::Act(Dungeon_mt &a_dungeon, Player& a_player) const
{
	const std::string reply = a_dungeon.Walk_mt(a_player); 
	return std::make_unique<TextActionResponse>(reply);
}

}	// dnd_game