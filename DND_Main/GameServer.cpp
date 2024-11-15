#include "GameServer.h"
#include <iostream>
#include <memory>
#include "NetWriter.h"
#include "NetReader.h"
#include "DND_lib/GameController.h"
#include "DND_lib/Player.h"



// Constructor for GameServer
GameServer::GameServer(boost::asio::io_context& a_ioService, int a_port)
// Initialize the acceptor to listen on the specified port and allow IPv4 connections
    : m_acceptor(m_ioService, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), a_port))
    , m_port{ a_port }
{
    // Nothing else to initialize here
}

// Main server loop that waits for clients to connect
void GameServer::WaitForClient()
{
    while (true)
    {
        try
        {

            // Create a socket for the connection
            boost::asio::ip::tcp::socket socket(m_ioService);

            // Accept a new client connection
            std::cout << "Waiting for client connection on port " << m_port << "..." << std::endl;
            m_acceptor.accept(socket); // Blocks until a client connects
            std::cout << "Client connected!" << std::endl;

            // Set buffer sizes for the socket
            boost::asio::socket_base::send_buffer_size sendBufferSizeOption(65536 * 2); // Example: 64*2 KB
            socket.set_option(sendBufferSizeOption);


            // Allocate network writers and readers for the connected socket
            auto netWriter = std::make_shared<NetWriter>(socket);
            auto netReader = std::make_shared<NetReader>(socket);

            // Welcome message for the player
            netWriter->Write("Welcome! Please enter your player name:");
            netWriter->Endl();
            std::string userName = netReader->ReadLine(); // Read player name from client

            // Create a player instance with the given name
            dnd_game::Player player(userName, m_dungeon.GetEntryRoom(), *netWriter);

            // Register_mt the player to the dungeon
            m_dungeon.RegisterPlayer(player, player.GetRoomNumber());
            std::cout << "Registered player " << player.GetName() << " into room " << player.GetRoomNumber() << std::endl;

            // Welcome message with the player's name
            netWriter->Write("Welcome " + userName + "!");
            netWriter->Endl();

            // Create a GameController to manage gameplay for the connected player
            dnd_game::GameController gameController(*netWriter, *netReader, m_dungeon, player);

            // Start the game (commented out, can be uncommented when implemented)
            gameController.Start();

        }
        catch (const std::exception& e)
        {
            // If an error occurs (e.g., client disconnects), print the error message
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

/*

GameServer::GameServer(boost::asio::io_context& io_context, unsigned short port)
    : m_acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
    m_dungeon_mt{} // initialize dungeon_mt
{
}

void GameServer::StartAccepting()
{
    // Start accepting connections
    m_acceptor.async_accept([this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
        if (!ec) {
            std::cout << "New connection accepted." << std::endl;
            HandleAccept(std::move(socket), ec);
        }
        else {
            std::cerr << "Error on accept: " << ec.message() << std::endl;
        }
        // Continue accepting the next connection
        StartAccepting();
        });
}

void GameServer::HandleAccept(boost::asio::ip::tcp::socket socket, const boost::system::error_code& error) {
    if (!error) {

        //// Allocate network writers and readers
        // auto netWriter = std::make_shared<game_server::CoutWriter>(std::cout);
        //auto netReader = std::make_shared<game_server::CinReader>(std::cin); // Placeholder reader


        // Allocate network writers and readers using Boost.Asio socket
auto netWriter = std::make_shared<NetWriter>(socket);
auto netReader = std::make_shared<NetReader>(socket);

// Create a player for the connected client
std::string playerName = "Client";  // Placeholder name
dnd_game::Player player{ playerName, dnd_game::Dungeon_mt::ENTRY_ROOM, *netWriter };
m_dungeon_mt.RegisterPlayer(player);

// Create a GameController to handle commands from the player
auto gameController = std::make_shared<dnd_game::GameController>(*netWriter, *netReader, m_dungeon_mt, player);

// Launch a thread to handle the client's gameplay
std::thread([gameController]() mutable {
    gameController->Start();
    }).detach();
    }
    else {
        std::cerr << "Error handling client: " << error.message() << std::endl;
    }
}
*/
