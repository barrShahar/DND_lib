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
#include <shared_mutex>
namespace dnd_game
{

// Dungeon room
class Room_mt
{
public:
    using Walls = std::array<Wall, 4>;
    explicit Room_mt(Number a_roomNumber,
                  std::pair<bool, Number> a_isDoorNorth,
                  std::pair<bool, Number> a_isDoorEast,
                  std::pair<bool, Number> a_isDoorSouth,
                  std::pair<bool, Number> a_isDoorWest,
                  bool a_isTreasure,
                  std::shared_ptr<Monster> a_monsterPtr
        );

    Room_mt(const Room_mt& a_other) = delete;
    Room_mt(Room_mt&&) noexcept;  // Allow move construction
    Room_mt& operator=(Room_mt&&) noexcept = default;  // Allow move assignment
    Room_mt& operator=(const Room_mt& a_other) = delete;
    ~Room_mt() = default;

    std::string GetNames() const;
    std::vector<std::string> GetNamesVec() const;
    std::optional<std::shared_ptr<Monster>> GetMonster();
    void DrawRoom(Writer& a_wrier, Direction a_direction);
    bool isDoor(Direction a_direction) const;
    bool ContainsMonster() const;
    bool IsTreasure() const;
    std::optional<Number> GetNextDoorRoomNumber(Direction a_direction) const;

 /*   AttackPlayerResponse AttackPlayer(const std::string& a_attackedName, Number a_dmg);
    AttackDragonResponse AttackDragon(Number a_dmg);*/


    std::optional<TREASURE_TYPE> GetTreasure_mt();
    void Register(Player& a_player);
    void Unregister(Player& a_player);
    void NotifyAll(const std::string& a_message);
    void NotifyAllExcept(const Player& a_excludedPlayer, const std::string& a_message);

    // Template method to lock player instance during callable execution
    template <typename Func>
    void WithLock(Func func);

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


    Iterator begin();
    Iterator end();

    // ConstIterator for const iteration
    class ConstIterator
    {
    public:
        explicit ConstIterator(Walls::const_iterator a_it);
        ConstIterator(const ConstIterator& a_other);
        ConstIterator& operator=(const ConstIterator& a_other) = delete;
        ~ConstIterator() = default;

        bool operator!=(const ConstIterator& a_other);
        ConstIterator& operator++();
        const Wall& operator*() const;

    private:
        Walls::const_iterator m_it;
    };

    ConstIterator cbegin() const;
    ConstIterator cend() const;
    ConstIterator begin() const; // Const version for iteration
    ConstIterator end() const;   // Const version for iteration

private:
    Walls m_walls;
    Number m_roomNumber;
    //SubjectRoom_mt m_subject;
    bool m_isTreasure;
    std::shared_ptr<SubjectRoom_mt> m_subject;
    std::shared_ptr<Monster> m_monsterPtr;


    mutable std::shared_mutex m_mutex;  // Mutex for thread safety
};

// Template function definition
template <typename Func>
inline void Room_mt::WithLock(Func func) {
    std::unique_lock<std::shared_mutex> lock(m_mutex);
    func(m_monsterPtr, m_subject);
}

}  // namespace dnd_game