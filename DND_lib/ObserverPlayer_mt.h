#pragma once
#include "GameParams.h"
#include <string>
#include <mutex>
#include "Player.h"

namespace dnd_game
{
class ObserverPlayer_mt
{
    using MessageType = std::string;
public:
    explicit ObserverPlayer_mt(Player& a_player);
    ObserverPlayer_mt(const ObserverPlayer_mt& a_other) = delete;
    ObserverPlayer_mt& operator=(const ObserverPlayer_mt& a_other) = delete;
    ~ObserverPlayer_mt() = default;

    Player& GetPlayer();
    std::string GetName() const;
    void Notify(const MessageType& a_message);
    bool IsEqual(const Player& a_player) const;
    //AttackPlayerResponse AttackPlayer(Number a_dmg);

private:
    Player& m_player;
    std::mutex m_notifyMutex;
};
}

