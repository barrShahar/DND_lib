#pragma once
#include <array>
#include <optional>
#include "mutex"
#include "Player.h"
#include "Wall.h"
#include "ascii/Grid.h"
#include "ascii/DungeonAscii.h"
#include "GameParams.h"
#include "SubjectRoom_mt.h"
#include "Dragon.h"
namespace dnd_game
{

// Dungeon room
class Room
{
public:
    using Walls = std::array<Wall, 4>;
    class Iterator
    {
    public:
        explicit Iterator(const Walls::iterator& a_it);
        Iterator(const Iterator& a_other);
        Iterator& operator=(const Iterator& a_other) = delete;
        ~Iterator() = default;

        bool operator!=(const Iterator& a_other);
        Iterator& operator++();
        Wall& operator*();
    private:
        Walls::iterator m_it;
    };

    explicit Room(Number a_roomNumber,
                  std::pair<bool, Number> a_isDoorNorth,
                  std::pair<bool, Number> a_isDoorEast,
                  std::pair<bool, Number> a_isDoorSouth,
                  std::pair<bool, Number> a_isDoorWest,
                  bool a_isDragon,
                  bool a_isTreasure);

    Room(const Room& a_other);
    Room& operator=(const Room& a_other) = delete;
    ~Room() = default;

    std::string GetNames() const;
    void DrawRoom(Writer& a_wrier, Direction a_direction);
    bool isDoor(Direction a_direction) const;
    bool IsDragon() const;
    bool IsTreasure() const;
    Number GetNextDoorRoomNumber(Direction a_direction) const;

 /*   AttackPlayerResponse AttackPlayer(const std::string& a_attackedName, Number a_dmg);
    AttackDragonResponse AttackDragon(Number a_dmg);*/


    std::optional<TREASURE_TYPE> GetTreasure_mt();
    void Register(Player& a_player);
    void Unregister(Player& a_player);
    void NotifyAll(const std::string& a_message);

    Iterator begin();
    Iterator end();

private:
    Walls m_walls;
    Number m_roomNumber;
    SubjectRoom_mt m_subject;
    bool m_isDragon;
    bool m_isTreasure;
    Dragon m_dragon;
    mutable std::mutex m_treasureDragonMutex;
};
}  // namespace dnd_game