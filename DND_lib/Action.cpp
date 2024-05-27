
#include "Action.h"
#include <sstream>
#include "StringActionResponse.h"
namespace dnd_game
{
// Free functions 
std::unique_ptr<ActionResponse> changeDirection(Dungeon_mt& a_dungeon, Player& a_player, int delta) 
{
    int currDirection = static_cast< int >(a_player.GetDirection());
    const int size = static_cast< int >(Direction::SIZE);
    currDirection = (currDirection + delta + size) % size;
    a_player.SetDirection(static_cast< Direction >(currDirection));

    std::stringstream reply;
    reply << "You are now facing " << DIRECTION_STRING[static_cast< Number >(a_player.GetDirection())];

    return std::make_unique<StringActionResponse>(reply.str());
}
}