#include "Action.h"

namespace dnd_game
{
std::unique_ptr<ActionResponse> ShoutAction::Act(Dungeon_mt &a_dungeon, Player& a_player) const
{
	const std::string& message = a_player.GetArguments();

	a_dungeon.NotifyRoomExcept(a_player, a_player.GetRoomNumber(), a_player.GetName() + "<shout>: " + message);

	const Room_mt room = a_dungeon.GetRoom(a_player.GetRoomNumber());
	for (const Wall& wall : room)
	{
		if (wall.IsDoor())
		{
			a_dungeon.NotifyRoom(wall.GetNextRoomNumber(), a_player.GetName() + "<shout>: " + message);
		}
	}

	
	return {};
}

}	// dnd_game