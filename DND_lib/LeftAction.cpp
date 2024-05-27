#include "Action.h"
#include <sstream>
#include "StringActionResponse.h"
namespace dnd_game
{
std::unique_ptr<ActionResponse> LeftAction::Act(Dungeon_mt& a_dungeon, Player& a_player) const
{
    int currDirection = static_cast< int > (a_player.GetDirection());
    int directionCount = static_cast< int > (Direction::SIZE);
    currDirection = (currDirection == 0) ? 3 : (currDirection - 1) % directionCount;

    a_player.SetDirection(static_cast< Direction > (currDirection));

    std::stringstream responseMessage;
    responseMessage << "You are now facing " << DIRECTION_STRING[static_cast< Number >(a_player.GetDirection())];

    return std::make_unique<StringActionResponse>(responseMessage.str());
}

} // dnd_game