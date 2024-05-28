#pragma once
#include <string>
#include "GameParams.h"
#include "Writer.h"

namespace dnd_game
{
class Player
{
public:
	explicit Player(const std::string& a_name, const Number a_entryRoom, Writer& a_writer);
	Player(const Player& a_oother) = delete;
	Player& operator=(const Player& a_oother) = delete;
	~Player() = default;

	void NotifyPlayer(const std::string& a_message);
	//AttackPlayerResponse AttackPlayer(Number a_dmg);

	// Getters
	const std::string& GetName() const;
	const Direction& GetDirection() const;
	const Number GetRoomNumber() const;

	// Setters
	void SetDirection(Direction a_direction);
	void SetRoomNumber(const Number a_roomNumber);

private:
	const std::string m_name;
	Direction m_direction;
	Writer& m_writer;
	Number m_roomNumber;
};

}	// dnd_game
