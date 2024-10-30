#include "Action.h"
#include "GameParams.h"
#include "Treasure.h"
#include "StringActionResponse.h"

namespace dnd_game
{
std::unique_ptr<ActionResponse> TakeAction::Act(Dungeon_mt &a_dungeon, Player& a_player) const
{
    // Get the treasure type in the player's current room
    std::optional<TREASURE_TYPE> treasureType = a_dungeon.GetTreasure(a_player.GetRoomNumber());
    if (treasureType.has_value())
    {
        // Create the treasure based on the type obtained
        std::unique_ptr<Treasure> treasure = Treasure::TreasureFactory(treasureType.value());

        // Apply the treasure's effect to the player in a thread-safe manner
        a_player.WithLock([&] ()
                        {
                            if (treasure)
                            {
                                (*treasure)(a_player);  // Call the effect on the player
                            }
                        });

        return std::make_unique<StringActionResponse>(treasure->Get_Reply());
    }
	return std::make_unique<StringActionResponse>(
		"Take Action Failed: No treasure found"
	);
}

}	// dnd_game