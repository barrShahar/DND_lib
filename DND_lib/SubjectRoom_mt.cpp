#include "SubjectRoom_mt.h"
#include <optional>
namespace dnd_game
{
std::string SubjectRoom_mt::GetNames() const
{
	std::lock_guard<std::mutex> lock(m_roomMutex);
	std::string playersNames = "";
	for (const std::pair<std::string const, std::unique_ptr<ObserverPlayer_mt>>& player : m_observersMap)
	{
		playersNames.append(player.first);
		playersNames.append(ENDL);
	}

	return playersNames;
}

std::vector<std::string> SubjectRoom_mt::GetNamesVec() const
{
	{	// gaurd
		std::lock_guard<std::mutex> lock(m_roomMutex);
		std::vector<std::string> playersNames;
		for (const std::pair<std::string const, std::unique_ptr<ObserverPlayer_mt>>& player : m_observersMap)
		{
			playersNames.push_back(player.first);
		}

		return playersNames;
	}
}

std::optional<std::reference_wrapper<Player>> SubjectRoom_mt::GetPlayer(const std::string& a_player)
{
	std::lock_guard<std::mutex> lock(m_roomMutex);  // Lock to ensure thread safety
	auto it = m_observersMap.find(a_player);
	if (it != m_observersMap.end() && it->second) {
		return it->second->GetPlayer(); 
	}
	return std::nullopt;
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
		for (Map::iterator it = m_observersMap.begin(); it != m_observersMap.end(); ++it)
		{
			if ((*it).second.get()->IsEqual(a_player))
			{
				m_observersMap.erase(it);
				break;
			}
		}
	}
}

void SubjectRoom_mt::NotifyAllExcept(const Player& a_player, const std::string& a_message)
{
	{	// gaurd
		std::lock_guard<std::mutex> lock(m_roomMutex);
		for (std::pair<std::string const, std::unique_ptr<ObserverPlayer_mt>>& pair : m_observersMap)
		{
			if (pair.first != a_player.GetName())
			{
				pair.second.get()->Notify(a_message);
			}
		}
	}
}

void SubjectRoom_mt::NotifyAllExcept(const std::vector<std::reference_wrapper<Player>>& a_excludedPlayers, const std::string& a_message)
{
	{	// gaurd
		std::lock_guard<std::mutex> lock(m_roomMutex);
		for (std::pair<std::string const, std::unique_ptr<ObserverPlayer_mt>>& pair : m_observersMap)
		{
			bool notifyPerson = true;
			for (Player& excludedPlayer : a_excludedPlayers)
			{
				if (excludedPlayer.GetName() == pair.first)
				{
					notifyPerson = false;
					break;
				}
			}
			
			if (notifyPerson)
			{
				pair.second.get()->Notify(a_message);
			}
		}
	}
}

void SubjectRoom_mt::NotifyAll(const std::string& a_message)
{
	std::pair< std::string, std::unique_ptr<ObserverPlayer_mt>> f;
	{	// gaurd
		std::lock_guard<std::mutex> lock(m_roomMutex); 
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