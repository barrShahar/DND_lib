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
    explicit Dungeon_mt();
    Dungeon_mt(const Dungeon_mt& a_other) = delete;
    Dungeon_mt& operator=(const Dungeon_mt& a_other) = delete;
    ~Dungeon_mt() = default;


    void DrawRoom(Writer& a_writer, Number a_roomNum, Direction a_playerDirection);
    Number Walk_mt(Number a_roomNumber, Direction a_direction);
    void NotifyRoom(Number a_roomNumber, const std::string& a_message);

    bool IsMonsterInTheRoom(Number a_roomNumber);
    std::string GetNames(Number a_roomNumber) const;
    void ShoutAction(Number a_roomNumber, const std::string& a_message);
    void RegisterPlayer(Player& a_player, Number a_roomNumber = NUMBER_ZERO);
    void UnregisterPlayer(Player& a_player, Number a_roomNumber);

    AttackPlayerResponse AttackPlayer(Number a_roomNumber, const std::string& attackerName,
                                      const std::string& a_attackedName,
                                      Number a_dmg, Number a_attackLifePoints);

    AttackDragonResponse AttackDragon(Number a_roomNumber, Number a_dmg);
    std::optional<TREASURE> GetTreasure(Number a_roomNumber);
private:
    const Rooms CreateDungeon() const;
    std::mutex m_mtx;
    std::condition_variable m_dungeonGaurd;
    Rooms m_rooms;
};
}  // dnd_game
