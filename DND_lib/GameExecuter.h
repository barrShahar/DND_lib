#pragma once
#include <vector>
#include <functional>
#include "Writer.h"
#include "Reader.h"
#include "Action.h"
#include "UserParser.h"

/*
tmp includes
*/
#include "ascii/Grid.h"
#include "Room.h"
/*
tmp includes
*/

namespace dnd_game
{
// Responsibility: Layer between user interface and the game
using ActMap = std::unordered_map<COMMAND, std::unique_ptr<Action>>;

class GameExecuter
{
public:
	explicit GameExecuter(Writer& a_writer,
						  Reader& a_reader,
						  Dungeon_mt& a_dungeon,
						  Player& a_player);
	GameExecuter(const GameExecuter& a_other) = delete;
	GameExecuter& operator=(const GameExecuter& a_other) = delete;
	~GameExecuter() = default;

	void Execute();

private:


    Dungeon_mt& m_dungeon;
    Writer& m_writer;
    Reader& m_reader;
    Player& m_player;
    UserParser m_parser;

    STATE m_currentState;
    ActMap m_actions;
    std::optional<COMMAND> m_currCommand;
    std::unordered_map<COMMAND, STATE> m_commandState;

    // states
    std::vector <std::function<STATE()>> m_statesFunctionPointers;
    STATE DrawRoom();
    STATE PrintUserOptions();
    STATE WaitingForInput();
    STATE ExecuteAndReportCommand();
    STATE GetUserInput();
    STATE Help();
    STATE AttackHandler();

};

}