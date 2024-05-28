#include "Player.h"

namespace dnd_game
{
Player::Player(const std::string& a_name,
			   const Number a_entryRoom,
			   Writer& a_writer)
	: m_name { a_name }
	, m_roomNumber { a_entryRoom }
	, m_writer { a_writer }
	, m_direction { Direction::NORTH }
{
	// CTOR
}


void Player::NotifyPlayer(const std::string & a_message)
{
	m_writer << a_message;
	m_writer.Endl();
}


// Getters
const std::string& dnd_game::Player::GetName() const
{
	return m_name;
}

const Direction& Player::GetDirection() const
{
	return m_direction;
}

const Number Player::GetRoomNumber() const
{
	return m_roomNumber;
}

// Setters
void Player::SetDirection(Direction a_direction)
{
	m_direction = a_direction;
}

void Player::SetRoomNumber(const Number a_roomNumber)
{
	m_roomNumber = a_roomNumber;
}

}	// dnd_game