#include <iostream>
#include <cassert>
#include "GameController.h"
#include "GameParams.h"

namespace dnd_game
{
GameController::GameController(Writer & a_writer,
						   Reader & a_reader,
						   Dungeon_mt & a_dungeon,
						   Player & a_player)
	: m_writer { a_writer }
	, m_reader { a_reader }
	, m_dungeon { a_dungeon }
	, m_player { a_player }
	, m_currentState { STATE::DRAW_ROOM }
	, m_userCommandParser {}
	, m_commandStateMap {}
	, m_actions {}
	, m_currentCommand { std::nullopt }
	, m_stateFunctions {
		std::bind(&GameController::DrawRoom, this),
		std::bind(&GameController::WaitingForInput, this),
		std::bind(&GameController::ExecuteAction, this),
		std::bind(&GameController::GetUserInput, this),
		std::bind(&GameController::Help, this),
		std::bind(&GameController::AttackHandler, this),
		std::bind(&GameController::CleanUp, this)
	}
{	// Ctor initialize action mappings
	m_actions[COMMAND::left] = std::make_unique<LeftAction>();
	m_actions[COMMAND::right] = std::make_unique<RightAction>();
	m_actions[COMMAND::walk] = std::make_unique<WalkAction>();
	m_actions[COMMAND::talk] = std::make_unique<TalkAction>();
	m_actions[COMMAND::shout] = std::make_unique<ShoutAction>();
	m_actions[COMMAND::look] = std::make_unique<LookAction>();
	m_actions[COMMAND::take] = std::make_unique<TakeAction>();
	m_actions[COMMAND::attack] = std::make_unique<AttackAction>();

	// Initialize state mappings for commands
	m_commandStateMap[COMMAND::left] = STATE::EXECUTE_ACTION;
	m_commandStateMap[COMMAND::right] = STATE::EXECUTE_ACTION;
	m_commandStateMap[COMMAND::walk] = STATE::EXECUTE_ACTION;
	m_commandStateMap[COMMAND::talk] = STATE::GET_INPUT;
	m_commandStateMap[COMMAND::shout] = STATE::GET_INPUT;
	m_commandStateMap[COMMAND::look] = STATE::EXECUTE_ACTION;
	m_commandStateMap[COMMAND::take] = STATE::EXECUTE_ACTION;
	m_commandStateMap[COMMAND::help] = STATE::HELP;
	m_commandStateMap[COMMAND::attack] = STATE::ATTACK;
	m_commandStateMap[COMMAND::quit] = STATE::CLEANUP;
}



void GameController::Start()
{
	STATE currState = STATE::HELP;
	while (currState != STATE::EXIT)
	{
		// Start the current state's function and update currState
		currState = m_stateFunctions[static_cast< Number >(currState)]();
	}

	m_writer << "Exit\n";
}




STATE GameController::DrawRoom()
{
	m_dungeon.DrawRoom(m_writer, m_player.GetRoomNumber(), m_player.GetDirection());
	return STATE::WAITING_FOR_INPUT;
}


STATE GameController::WaitingForInput()
{
	const std::string untrustedUserCommand = m_reader.ReadLine();

	m_currentCommand = m_userCommandParser.ParseCommand(untrustedUserCommand);
	if (m_currentCommand.has_value())
	{
		return m_commandStateMap.at(m_currentCommand.value());
	}
	return STATE::WAITING_FOR_INPUT;
}

STATE GameController::ExecuteAction()
{
	assert(m_currentCommand.has_value());
	std::unique_ptr<ActionResponse> respone = m_actions.at(m_currentCommand.value()).get()->Act(m_dungeon, m_player);

	// Reset current command
	m_currentCommand = std::nullopt;

	GameController::DrawRoom();

	// Output the data
	if (respone)
	{
		m_writer.Write(respone.get()->GetResponse().first.get());
		m_writer.Endl();
	}

	return STATE::WAITING_FOR_INPUT;
}

STATE GameController::GetUserInput()
{
	return STATE();
}

STATE GameController::Help()
{
	m_writer.Write("This game supports the following commands:");
	m_writer.Endl();
	for (size_t i = 0; i < static_cast< size_t > (COMMAND::SIZE); ++i)
	{
		m_writer.Write(COMMAND_STRING[i]);
		m_writer.Endl();
	}
	m_writer.Write("Press any key to continue...");
	m_reader.ReadLine();

	return STATE::DRAW_ROOM;
}

STATE GameController::AttackHandler()
{
	return STATE();
}

STATE GameController::CleanUp()
{
	// Do cleanup if needed
	m_writer << "Cleaing\n";
	return STATE::EXIT;
}





}