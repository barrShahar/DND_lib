#include "GameServer.h"
#include "NetWriter.h"
#include "NetReader.h"
#include "DND_lib/GameController.h"
#include "DND_lib/Player.h"
#include "Internet/UrlPathReader.h"
const int PORT = 4010;

GameServer::GameServer()
	: m_dungeon {}
	, m_net { PORT }
{

}

void GameServer::WaitForClient()
{
	while (true)
	{
		simplenet::SimpleNetMT::Connection c = m_net.WaitForConnection();
		NetWriter netWriter { c };
		NetReader netReader { c };
		netWriter.Write("Welcome! Please enter player name\n");
		std::string userName = netReader.ReadLine();
		dnd_game::Player player { userName, m_dungeon.GetEntryRoom(), netWriter };
		m_dungeon.RegisterPlayer(player);
		netWriter.Write("\nWelcome " + userName + "!\n");

		try
		{

			auto [path, isHttp] = simplenet::TryReadUrlPath(c);

			if (isHttp)  // Not supporting!!
			{

			}
			else
			{
				dnd_game::GameController gameController { netWriter, netReader, m_dungeon, player };

				gameController.Start();
			}

		}
		catch (const simplenet::SimpleNetMT::ConnectionClosed&)
		{
			// unregister player!
			m_dungeon.UnregisterPlayer(player, player.GetRoomNumber());
			std::cout << "CLOSED\n";
		}
	}

}

