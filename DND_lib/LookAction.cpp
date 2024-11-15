#include "Action.h"
#include "StringActionResponse.h"

namespace dnd_game
{
std::unique_ptr<ActionResponse> LookAction::Act(Dungeon_mt &a_dungeon, Player& a_player) const
{
	//const Room& room = a_dungeon.GetRoom(a_player.GetRoomNumber());
	//std::string info = "Players in the room:" + ENDL + room.GetNames_mt();

	return std::make_unique<StringActionResponse>(
		LookUp(a_dungeon, a_player)
	);
}

std::string LookAction::LookUp(Dungeon_mt& a_dungeon, Player& a_player)
{
	const Room& room = a_dungeon.GetRoom(a_player.GetRoomNumber());
	return "Players in the room:" + ENDL + room.GetNames_mt();
}

}	// dnd_game