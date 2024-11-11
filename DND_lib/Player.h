#pragma once
#include <string>
#include <mutex>
#include "GameParams.h"
#include "Writer.h"

namespace dnd_game {

static constexpr Number INITIAL_HEALTH_POINTS = 100;
static constexpr Number INITIAL_ATTACK_POINTS = 20;

class Player {
public:
    explicit Player(const std::string& a_name, const Number a_entryRoom, Writer& a_writer);
    Player(const Player& a_other) = delete;
    Player& operator=(const Player& a_other) = delete;
    ~Player() = default;

    void NotifyPlayer(const std::string& a_message);

    // Template method to lock player instance during callable execution
    template <typename Func>
    void WithLock(Func func);

    // Getters
    const std::string& GetName() const;
    const Direction& GetDirection() const;
    const Number GetRoomNumber() const;
    const Number GetHealthPoints() const;
    const Number GetDmgPoints() const;
    const std::string GetArguments() const;
    const bool IsAlive() const;

    // Setters
    void SetDirection(Direction a_direction);
    void SetRoomNumber(Number a_roomNumber);
    void SetHealthPoints(Number a_hp);
    void SetDmgPoints(Number a_dmgP);
    void SetArguments(std::string a_arguemnts);

private:
    const std::string m_name;
    std::string m_commandArguemnts;
    Direction m_direction;
    Writer& m_writer;
    Number m_roomNumber;
    unsigned int m_hp;
    unsigned int m_dmgP;
    mutable std::mutex m_mutex; // Allows modification in const methods
};

// Template function definition
template <typename Func>
inline void Player::WithLock(Func func) {
    std::lock_guard<std::mutex> lock(m_mutex);
    func();
}

}  // namespace dnd_game
