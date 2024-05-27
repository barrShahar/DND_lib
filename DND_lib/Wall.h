#pragma once
#include "GameParams.h"
namespace dnd_game
{
// One of 4 walls of Room.h class
class Wall
{
public:
    Wall(bool a_isDoor, Number a_nextRoomNumber);
    Wall(const Wall& a_other);
    Wall& operator=(const Wall& a_other) = delete;
    ~Wall() = default;

    const bool IsDoor() const;
    const Number GetNextRoomNumber() const;

private:
    const bool m_isDoor;
    const Number m_nextRoomNumber;
};
}  // namespace dnd_game