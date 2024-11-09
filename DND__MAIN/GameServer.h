#pragma once
#include <boost/asio.hpp>
#include "NetWriter.h"
#include "NetReader.h"
#include "DND_lib/GameController.h"
#include "DND_lib/Player.h"

class GameServer
{
public:
    explicit GameServer(boost::asio::io_context& io_context, unsigned short port);
    GameServer(const GameServer& a_other) = delete;
    GameServer& operator=(const GameServer& a_other) = delete;
    ~GameServer() = default;

    void WaitForClient();

private:
    dnd_game::Dungeon_mt m_dungeon;
    boost::asio::ip::tcp::acceptor m_acceptor;
    boost::asio::io_context& m_io_context;

    void AcceptConnection();
    void HandleClient(boost::asio::ip::tcp::socket socket);
};

/*
class GameServer
{
public:
    explicit GameServer();
    GameServer(const GameServer& a_other) = delete;
    GameServer& operator=(const GameServer& a_other) = delete;
    ~GameServer() = default;

    void WaitForClient();

private:
    dnd_game::Dungeon_mt m_dungeon;
    simplenet::SimpleNetMT m_net;

};


*/