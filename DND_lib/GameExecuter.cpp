#include <iostream>
#include "GameExecuter.h"
#include "GameParams.h"

namespace dnd_game
{
GameExecuter::GameExecuter(Writer & a_writer,
						   Reader & a_reader,
						   Dungeon_mt & a_dungeon,
						   Player & a_player)
	: m_writer { a_writer }
	, m_reader { a_reader }
	, m_dungeon { a_dungeon }
	, m_player { a_player }
	, m_currentState { STATE::DRAW_ROOM }
	, m_parser {}
	, m_commandState {}
	, m_actions {}
	, m_currCommand { std::nullopt }
	, m_statesFunctionPointers {
		std::bind(&GameExecuter::DrawRoom, this),
		std::bind(&GameExecuter::PrintUserOptions, this),
		std::bind(&GameExecuter::WaitingForInput, this),
		std::bind(&GameExecuter::ExecuteAndReportCommand, this),
		std::bind(&GameExecuter::GetUserInput, this),
		std::bind(&GameExecuter::Help, this),
		std::bind(&GameExecuter::AttackHandler, this) }
{	// Ctor
	m_actions[COMMAND::left] = std::make_unique<LeftAction>();
	m_actions[COMMAND::right] = std::make_unique<RightAction>();
	m_actions[COMMAND::walk] = std::make_unique<WalkAction>();
	m_actions[COMMAND::talk] = std::make_unique<TalkAction>();
	m_actions[COMMAND::shout] = std::make_unique<ShoutAction>();
	m_actions[COMMAND::look] = std::make_unique<LookAction>();
	m_actions[COMMAND::take] = std::make_unique<TakeAction>();
	m_actions[COMMAND::attack] = std::make_unique<AttackAction>();


	m_commandState[COMMAND::left] = STATE::STRING_ACT;
	m_commandState[COMMAND::right] = STATE::STRING_ACT;
	m_commandState[COMMAND::walk] = STATE::STRING_ACT;
	m_commandState[COMMAND::talk] = STATE::GET_INPUT;
	m_commandState[COMMAND::shout] = STATE::GET_INPUT;
	m_commandState[COMMAND::look] = STATE::STRING_ACT;
	m_commandState[COMMAND::take] = STATE::STRING_ACT;
	m_commandState[COMMAND::help] = STATE::HELP;
	m_commandState[COMMAND::attack] = STATE::ATTACK;
	m_commandState[COMMAND::quit] = STATE::EXIT;
}



void GameExecuter::Execute()
{
	STATE currState = STATE::DRAW_ROOM;
	while (currState != STATE::EXIT)
	{
		// Call the function of the current state and updating currState;

		currState = m_statesFunctionPointers[static_cast< Number >(currState)]();
	}

	m_writer << "Write Test in Execute\n";
	// Grid grid(ASCII_AXIS_X, ASCII_AXIS_Y);

	Room room(NUMBER_ZERO, 
			  { false, 0 }, 
			  { true, 0 }, 
			  { false, 0 }, 
			  { true, 0 }, 
			  false, 
			  false);
	room.DrawRoom(m_writer, Direction::NORTH);
	/*m_dungeon.DrawRoom(m_writer, 0, Direction::NORTH);*/
}




STATE GameExecuter::DrawRoom()
{
	m_dungeon.DrawRoom(m_writer, 0, Direction::NORTH);
	return STATE::PRINT_USER_OPTIONS;
}

STATE GameExecuter::PrintUserOptions()
{
	m_writer.Write("This is menu option\n");
	return STATE::WAITING_FOR_INPUT;
}

STATE GameExecuter::WaitingForInput()
{
	const std::string unsafeInput = m_reader.ReadLine();
	std::cout << "Got " << unsafeInput << std::endl;
	return STATE::PRINT_USER_OPTIONS;
}

STATE GameExecuter::ExecuteAndReportCommand()
{
	return STATE();
}

STATE GameExecuter::GetUserInput()
{
	return STATE();
}

STATE GameExecuter::Help()
{
	return STATE();
}

STATE GameExecuter::AttackHandler()
{
	return STATE();
}





}