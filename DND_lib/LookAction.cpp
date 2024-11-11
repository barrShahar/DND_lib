#include "Action.h"
#include "StringActionResponse.h"

namespace dnd_game
{
std::unique_ptr<ActionResponse> LookAction::Act(Dungeon_mt &a_dungeon, Player& a_player) const
{
	//const Room_mt& room = a_dungeon.GetRoom(a_player.GetRoomNumber());
	//std::string info = "Players in the room:" + ENDL + room.GetNames();

	return std::make_unique<StringActionResponse>(
		LookUp(a_dungeon, a_player)
	);
}

std::string LookAction::LookUp(Dungeon_mt& a_dungeon, Player& a_player)
{
	const Room_mt& room = a_dungeon.GetRoom(a_player.GetRoomNumber());
	return "Players in the room:" + ENDL + room.GetNames();
}

}	// dnd_game