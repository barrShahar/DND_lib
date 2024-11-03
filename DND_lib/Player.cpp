#include "Player.h"

namespace dnd_game
{
Player::Player(const std::string& a_name,
			   const Number a_entryRoom,
			   Writer& a_writer)
	: m_name { a_name }
	, m_roomNumber { a_entryRoom }
	, m_commandArguemnts { }
	, m_writer { a_writer }
	, m_direction { Direction::NORTH }
	, m_hp { INITIAL_HEALTH_POINTS }
	, m_dmgP { INITIAL_ATTACK_POINTS }
	, m_mutex {}
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

const Number Player::GetHealthPoints() const
{
	return m_hp;
}

const Number Player::GetDmgPoints() const
{
	return m_dmgP;
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

void Player::SetHealthPoints(const Number a_hp)
{
	m_hp = a_hp;
}

void Player::SetDmgPoints(const Number a_dmgP)
{
	m_dmgP = a_dmgP;
}

void Player::SetArguments(std::string a_commandArguemnts)
{
	m_commandArguemnts = a_commandArguemnts;
}

}	// dnd_game