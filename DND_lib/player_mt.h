#pragma once
#include "Player.h"

namespace dnd_game
{
class Player_mt: public Player
{
public:
	explicit Player_mt(const std::string& a_name, const Number a_entryRoom, Writer& a_writer);
	Player_mt(const Player_mt& a_oother) = delete;
	Player_mt& operator=(const Player_mt& a_oother) = delete;
	~Player_mt() = default;

private:
};
}  // namespace shahar
