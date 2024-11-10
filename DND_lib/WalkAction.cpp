#include "Action.h"

namespace dnd_game
{
std::unique_ptr<ActionResponse> WalkAction::Act(Dungeon_mt &a_dungeon, Player& a_player) const
{
	//const std::string reply = a_dungeon.Walk_mt(a_player); 
	std::string reply = a_dungeon.Walk_mt(a_player);

	// Debug
	reply += ENDL;
	const std::string monsterExist = a_dungeon.GetRoom(a_player.GetRoomNumber()).ContainsMonster() ? "Exist" : "Not Exist";
	reply += "<debug>:Monster: " + monsterExist;
	return std::make_unique<TextActionResponse>(reply);
}

}	// dnd_game