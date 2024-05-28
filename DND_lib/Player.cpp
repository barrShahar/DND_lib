#include "Player.h"

namespace dnd_game
{
Player::Player(const std::string& a_name,
			   Writer& a_writer)
	: m_name { a_name }
	, m_direction { Direction::NORTH }
	, m_writer { a_writer }
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

// Setters
void Player::SetDirection(Direction a_direction)
{
	m_direction = a_direction;
}

}	// dnd_game