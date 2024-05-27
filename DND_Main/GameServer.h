#pragma once
#include "DND_lib/Dungeon_mt.h"

class GameServer
{
public:
	explicit GameServer();
	GameServer(const GameServer& a_other) = delete;
	GameServer& operator=(const GameServer& a_other) = delete;
	~GameServer() = default;

private:
	dnd_game::Dungeon_mt m_dungeon;
	//simplenet::SimpleNetMT m_net;

};

