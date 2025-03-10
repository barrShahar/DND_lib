#include "Player.h"
#include <cassert>

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

const std::string Player::GetArguments() const
{
	return m_commandArguemnts;
}

const bool Player::IsAlive() const
{
	assert(m_hp >= 0 && "Players hp became negative!");
	return (m_hp > 0);
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

std::unique_ptr<AttackResponse> Player::TakeDamage_mt(Number a_damage)
{
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		Number hp = GetHP();
		SetHP(std::max(0, hp - a_damage));
		return std::make_unique<AttackResponse>(m_name, IsInPlay(), hp, GetHP(), ReturnedDamage(a_damage));
	}
}


bool Player::IsInPlay() const
{
	return IsAlive();
}

Number Player::ReturnedDamage(Number a_damage) const
{
	return 0;
}

Number Player::GetHP() const
{
	return GetHealthPoints();
}

void Player::SetHP(Number a_hp)
{
	SetHealthPoints(a_hp);
}

}	// dnd_game