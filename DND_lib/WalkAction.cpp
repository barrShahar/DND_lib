#include "Action.h"
#include "StringActionResponse.h"
namespace dnd_game
{
std::unique_ptr<ActionResponse> WalkAction::Act(Dungeon_mt &a_dungeon, Player& a_player) const
{
	//const std::string reply = a_dungeon.Walk_mt(a_player); 
	std::string reply = a_dungeon.Walk_mt(a_player);
	return std::make_unique<StringActionResponse>(reply);

	/* Walk and Look
	* 	std::string lookUp = LookAction::LookUp(a_dungeon, a_player);
	return std::make_unique<StringActionResponse>(reply + ENDL + lookUp);
	*/
}

}	// dnd_game