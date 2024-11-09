#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00 // For Windows 10
#endif

#pragma once
#include <boost/asio.hpp>
#include "DND_lib/GameController.h"
#include "DND_lib/Player.h"
#include "DND_lib/Dungeon_mt.h"



class GameServer
{
public:
    explicit GameServer(boost::asio::io_context& a_ioService, int a_port);
    GameServer(const GameServer& a_other) = delete;
    GameServer& operator=(const GameServer& a_other) = delete;
    ~GameServer() = default;

    void WaitForClient(); // Function to wait for a client connection and handle the game

private:
    boost::asio::io_service m_ioService; // Handles input/output for Boost.Asio
    boost::asio::ip::tcp::acceptor m_acceptor; // Accepts incoming connections
    dnd_game::Dungeon_mt m_dungeon; // The dungeon used for the game
    int m_port;
};