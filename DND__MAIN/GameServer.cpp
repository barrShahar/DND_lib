
#include "GameServer.h"
#include <iostream>


GameServer::GameServer(boost::asio::io_context& io_context, unsigned short port)
    : m_io_context(io_context),
    m_acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
}

void GameServer::WaitForClient()
{
    AcceptConnection();
    m_io_context.run();
}

void GameServer::AcceptConnection()
{
    m_acceptor.async_accept(
        [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket)
        {
            if (!ec)
            {
                HandleClient(std::move(socket));
            }
            AcceptConnection();
        });
}

void GameServer::HandleClient(boost::asio::ip::tcp::socket socket)
{
    auto netWriter = std::make_shared<NetWriter>(socket);
    auto netReader = std::make_shared<NetReader>(socket);

    netWriter->Write("Welcome! Please enter player name\n");
    std::string userName = netReader->ReadLine();

    dnd_game::Player player{userName, m_dungeon.GetEntryRoom(), *netWriter };

    // Register the player to the dungeon
    m_dungeon.RegisterPlayer(player);


    netWriter->Write("\nWelcome " + userName + "!\n");

    try
    {
        boost::asio::streambuf buffer;
        boost::asio::read_until(socket, buffer, "\n");

        std::istream stream(&buffer);
        std::string request;
        std::getline(stream, request);

        if (request.find("HTTP/") != std::string::npos)
        {
            // Handle HTTP request here (using Boost.Beast, for example)
            std::cerr << "HTTP requests are not supported.\n";
        }
        else
        {
            dnd_game::GameController gameController{ *netWriter, *netReader, m_dungeon, player };
            // gameController.Start();
        }
    }
    catch (const std::exception& e)
    {
        // Unregister player
        m_dungeon.UnregisterPlayer(player, player.GetRoomNumber());
        std::cout << "Connection closed: " << e.what() << "\n";
    }
}


/*
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
		game_serverCoutWriter netWriter { c };
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

				//gameController.Start();
			}

		}
		catch (const simplenet::SimpleNetMT::ConnectionClosed&)
		{
			// unregister player
			m_dungeon.UnregisterPlayer(player, player.GetRoomNumber());
			std::cout << "CLOSED\n";
		}
	}


}



*/