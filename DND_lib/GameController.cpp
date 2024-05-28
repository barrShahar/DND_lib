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
	, m_parser {}
	, m_mapCommandToState {}
	, m_actions {}
	, m_currCommand { std::nullopt }
	, m_statesFunctionPointers {
		std::bind(&GameController::DrawRoom, this),
		std::bind(&GameController::WaitingForInput, this),
		std::bind(&GameController::ExecuteAction, this),
		std::bind(&GameController::GetUserInput, this),
		std::bind(&GameController::Help, this),
		std::bind(&GameController::AttackHandler, this) }
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
	m_mapCommandToState[COMMAND::left] = STATE::EXECUTE_ACTION;
	m_mapCommandToState[COMMAND::right] = STATE::EXECUTE_ACTION;
	m_mapCommandToState[COMMAND::walk] = STATE::EXECUTE_ACTION;
	m_mapCommandToState[COMMAND::talk] = STATE::GET_INPUT;
	m_mapCommandToState[COMMAND::shout] = STATE::GET_INPUT;
	m_mapCommandToState[COMMAND::look] = STATE::EXECUTE_ACTION;
	m_mapCommandToState[COMMAND::take] = STATE::EXECUTE_ACTION;
	m_mapCommandToState[COMMAND::help] = STATE::HELP;
	m_mapCommandToState[COMMAND::attack] = STATE::ATTACK;
	m_mapCommandToState[COMMAND::quit] = STATE::EXIT;
}



void GameController::Execute()
{
	STATE currState = STATE::HELP;
	while (currState != STATE::EXIT)
	{
		// Execute the current state's function and update currState
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




STATE GameController::DrawRoom()
{
	m_dungeon.DrawRoom(m_writer, 0, m_player.GetDirection());
	return STATE::WAITING_FOR_INPUT;
}


STATE GameController::WaitingForInput()
{
	const std::string untrustedUserCommand = m_reader.ReadLine();

	//if (!m_player.IsAlive())
	//{
	//	return STATE::EXIT;
	//}

	m_currCommand = m_parser.ParseCommand(untrustedUserCommand);
	if (m_currCommand.has_value())
	{
		return m_mapCommandToState.at(m_currCommand.value());
	}
	return STATE::WAITING_FOR_INPUT;
}

STATE GameController::ExecuteAction()
{
	assert(m_currCommand.has_value());
	std::unique_ptr<ActionResponse> respone = m_actions.at(m_currCommand.value()).get()->Act(m_dungeon, m_player);

	// Reset current command
	m_currCommand = std::nullopt;

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
	std::cout << "This game supports the following commands:" << std::endl;
	for (size_t i = 0; i < static_cast< size_t > (COMMAND::SIZE); ++i)
	{
		std::cout << COMMAND_STRING[i] << std::endl;
	}
	std::cout << "Press any key to continue..." << std::endl;
	std::cin.get();
	return STATE::DRAW_ROOM;
}

STATE GameController::AttackHandler()
{
	return STATE();
}





}