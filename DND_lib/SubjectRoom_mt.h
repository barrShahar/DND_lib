#pragma once
#include <unordered_map>
#include <memory>
#include "ObserverPlayer_mt.h"

namespace dnd_game
{
class SubjectRoom_mt
{
	using Map = std::unordered_map<std::string, std::unique_ptr<ObserverPlayer_mt>>;
public:
    explicit SubjectRoom_mt() = default;
    SubjectRoom_mt(const SubjectRoom_mt& a_other) = delete;
    SubjectRoom_mt& operator=(const SubjectRoom_mt& a_other) = delete;
    ~SubjectRoom_mt() = default;

    std::string GetNames() const;

    virtual void Register(Player& a_player);
    virtual void Unregister(Player& a_player);

    virtual void NotifyAll(const std::string& a_message);
    //AttackPlayerResponse AttackPlayer(const std::string& a_playerToAttack, Number a_dmg);
private:
    // std::vector<std::unique_ptr<ObserverPlayer_mt>> m_observers;
    Map m_observersMap;
    mutable std::mutex m_roomMutex;

};
}	// dnd_game
