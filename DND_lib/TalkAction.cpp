#include "Action.h"
#include "StringActionResponse.h"

namespace dnd_game
{
std::unique_ptr<ActionResponse> TalkAction::Act(Dungeon_mt &a_dungeon, Player& a_player) const
{
	const std::string& message = a_player.GetArguments();

	a_dungeon.NotifyRoomExcept(a_player, a_player.GetRoomNumber(), a_player.GetName() + "<talk>: " + message);

	return std::make_unique<StringActionResponse>(
		"Sent<talk>: " + message
	);


}

}	// dnd_game