#pragma once
#include <mutex>
#include <condition_variable>
#include <vector>
#include "GameParams.h"
#include "Room.h"
#include <algorithm>
#include <array>


namespace dnd_game
{

using Rooms = std::vector<Room>;

class Dungeon_mt
{
    // Dungeon_mt assuming subject and observer calsses are thread safe
public:
    static const int ENTRY_ROOM = 0;
    explicit Dungeon_mt();
    Dungeon_mt(const Dungeon_mt& a_other) = delete;
    Dungeon_mt& operator=(const Dungeon_mt& a_other) = delete;
    ~Dungeon_mt() = default;

    Number GetEntryRoom() const;
    const Room& GetRoom(Number a_roomNumber) const;
    void DrawRoom(Writer& a_writer, Number a_roomNum, Direction a_playerDirection);
    std::string Walk_mt(Player& a_player);
    void NotifyRoom(Number a_roomNumber, const std::string& a_message);
    void NotifyRoomExcept(const Player& a_excludedPlayer, Number a_roomNumber, const std::string& a_message);

    bool IsMonsterInTheRoom(Number a_roomNumber);
    std::string GetNames_mt(Number a_roomNumber) const;
    void ShoutAction(Number a_roomNumber, const std::string& a_message);
    void RegisterPlayer(Player& a_player, Number a_roomNumber = NUMBER_ZERO);
    void UnregisterPlayer(Player& a_player, Number a_roomNumber);

    AttackPlayerResponse AttackPlayer(Number a_roomNumber, const std::string& attackerName,
                                      const std::string& a_attackedName,
                                      Number a_dmg, Number a_attackLifePoints);

    AttackDragonResponse AttackDragon(Number a_roomNumber, Number a_dmg);
    std::optional<TREASURE_TYPE> GetTreasure(Number a_roomNumber);

    template <typename Func>
    void ExecuteWithRoomLock(Number a_roomNumber, Func func);

private:
    const Rooms CreateDungeon() const;
    mutable std::mutex m_mtx;
    std::condition_variable m_dungeonGuard;
    Rooms m_rooms;
    
};

template <typename Func>
void Dungeon_mt::ExecuteWithRoomLock(Number a_roomNumber, Func func)
{
    // Ensure room number is valid
    if (a_roomNumber < 0 || a_roomNumber >= static_cast<Number>(m_rooms.size()))
    {
        throw std::out_of_range("Invalid room number");
    }

    // Get the specified room
    Room& room = m_rooms[a_roomNumber];

    // Execute the function with room lock
    room.WithLock(func);
}

}  // dnd_game
