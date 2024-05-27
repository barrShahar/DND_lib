#include "ObserverPlayer_mt.h"
namespace dnd_game
{
ObserverPlayer_mt::ObserverPlayer_mt(Player& a_player)
	: m_player { a_player }
	, m_notifyMutex {}
{
	// Ctor
}

Player& ObserverPlayer_mt::GetPlayer()
{
	return m_player;
}

std::string ObserverPlayer_mt::GetName() const
{
	return m_player.GetName();
}

void ObserverPlayer_mt::Notify(const MessageType& a_message)
{
	{	// gaurd
		std::lock_guard<std::mutex> lock(m_notifyMutex);	// I dont need it! 
		m_player.NotifyPlayer(a_message);
	}
}

bool ObserverPlayer_mt::IsEqual(const Player & a_player) const
{
	return &a_player == &m_player;
}

//AttackPlayerResponse ObserverPlayer_mt::AttackPlayer(Number a_dmg)
//{
//	return m_player.AttackPlayer(a_dmg);
//}

}	// dnd_game
