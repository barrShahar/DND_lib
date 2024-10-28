#include "Action.h"
#include "GameParams.h"
#include "Treasure.h"
#include "StringActionResponse.h"

namespace dnd_game
{
std::unique_ptr<ActionResponse> TakeAction::Act(Dungeon_mt &a_dungeon, Player& a_player) const
{
	std::optional<TREASURE_TYPE> enumTreasure = a_dungeon.GetTreasure(a_player.GetRoomNumber());
	if (enumTreasure.has_value())
	{
		std::unique_ptr<Treasure> treasurePtr = Treasure::TreasureFactory(enumTreasure.value());
		a_player.WithLock([&] ()
						{
							if (treasurePtr)
							{
								(*treasurePtr)(a_player);  // Call the effect on the player
							}
						});

		return std::make_unique<StringActionResponse>(treasurePtr->Get_Reply());
	}
	return std::make_unique<StringActionResponse>("No treasure in this room");
}

}	// dnd_game