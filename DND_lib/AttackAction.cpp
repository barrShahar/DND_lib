#include "Action.h"
#include "SubjectRoom_mt.h"

namespace dnd_game
{
std::unique_ptr<ActionResponse> AttackAction::Act(Dungeon_mt &a_dungeon, Player& a_player) const
{
	const std::string& nameToAttack = a_player.GetArguments();

	// lock the entire room
	// get reference of the person or monster object that being attack
	//Room_mt room = a_dungeon.GetRoom(a_player.GetRoomNumber());
	//std::vector<std::string> players = room.GetNamesVec();


	// attack and get response from the attacker (or not)
	// Notify the other player
	// Return info

	return {};
}

}	// dnd_game