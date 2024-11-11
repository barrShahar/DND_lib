#pragma once
#include <unordered_map>
#include <memory>
#include "ObserverPlayer_mt.h"
#include "optional"

namespace dnd_game
{
class SubjectRoom_mt
{
	using Map = std::unordered_map<std::string, std::unique_ptr<ObserverPlayer_mt>>;
public:
    explicit SubjectRoom_mt() = default;
    SubjectRoom_mt(const SubjectRoom_mt& a_other) = delete;
    SubjectRoom_mt(SubjectRoom_mt&& a_other) noexcept = default; // Allow move construction
    SubjectRoom_mt& operator=(SubjectRoom_mt&&) noexcept = default;  // Allow move assignment
    SubjectRoom_mt& operator=(const SubjectRoom_mt& a_other) = delete;
    ~SubjectRoom_mt() = default;

    std::string GetNames() const;
    std::vector<std::string> GetNamesVec() const;
    std::optional<std::reference_wrapper<Player>> GetPlayer(const std::string& a_player);

    virtual void Register(Player& a_player);
    virtual void Unregister(Player& a_player);
    virtual void NotifyAllExcept(const Player& a_player, const std::string& a_message);
    virtual void NotifyAllExcept(const std::vector<std::reference_wrapper<Player>>& a_excludedPlayers, const std::string& a_message);
    virtual void NotifyAll(const std::string& a_message);
    //AttackPlayerResponse AttackPlayer(const std::string& a_playerToAttack, Number a_dmg);
private:
    // std::vector<std::unique_ptr<ObserverPlayer_mt>> m_observers;
    Map m_observersMap;
    mutable std::mutex m_roomMutex;

};
}	// dnd_game

