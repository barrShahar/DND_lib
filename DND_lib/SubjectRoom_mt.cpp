#include "SubjectRoom_mt.h"
namespace dnd_game
{
std::string SubjectRoom_mt::GetNames() const
{
	std::lock_guard<std::mutex> lock(m_roomMutex);
	std::string playersNames = "Players Names:\n";
	for (const std::pair<std::string const, std::unique_ptr<ObserverPlayer_mt>>& player : m_observersMap)
	{
		playersNames.append(player.first);
		playersNames.append("\n");
	}
	return playersNames;
}

void SubjectRoom_mt::Register(Player& a_player)
{
	{	// gaurd
		std::lock_guard<std::mutex> lock(m_roomMutex);
		m_observersMap.insert(std::make_pair(a_player.GetName(), std::make_unique<ObserverPlayer_mt>(a_player)));
	}
}

void SubjectRoom_mt::Unregister(Player& a_player)
{
	{	// gaurd
		std::lock_guard<std::mutex> lock(m_roomMutex);
		for (Map::iterator it = m_observersMap.begin();
			 it != m_observersMap.end(); ++it)
		{
			if ((*it).second.get()->IsEqual(a_player))
			{
				m_observersMap.erase(it);
				break;
			}
		}
	}
}

void SubjectRoom_mt::NotifyAll(const std::string& a_message)
{
	std::pair< std::string, std::unique_ptr<ObserverPlayer_mt>> f;
	{	// gaurd
		std::lock_guard<std::mutex> lock(m_roomMutex); // This was commented but I think I need it
		for (std::pair<std::string const, std::unique_ptr<ObserverPlayer_mt>>& pair : m_observersMap)
		{
			pair.second.get()->Notify(a_message);
		}
	}
}

//AttackPlayerResponse SubjectRoom_mt::AttackPlayer(const std::string& a_playerToAttack, Number a_dmg)
//{
//	{	// gaurd
//		std::lock_guard<std::mutex> lock(m_roomMutex);
//		Map::iterator it = m_observersMap.find(a_playerToAttack);
//		if (it != m_observersMap.end())
//		{
//			return  it->second.get()->AttackPlayer(a_dmg);
//		}
//
//		return AttackPlayerResponse(false);
//	}
//}

}	// dnd_game