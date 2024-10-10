#pragma once
#include <vector>
#include <functional>
#include "Writer.h"
#include "Reader.h"
#include "Action.h"
#include "UserParser.h"


namespace dnd_game
{
// This class manages the state and actions of the game,
// utilizing the Strategy design pattern to handle different game actions.
using ActMap = std::unordered_map<COMMAND, std::unique_ptr<Action>>;

class GameController
{

    /**
     * @class GameController
     * @brief Manages the state and actions of the game, serving as the intermediary between the user interface and the game logic.
     *
     * The GameController class is responsible for executing the main game loop, handling user input, and transitioning between various game states.
     * It utilizes the Strategy design pattern to manage different game actions dynamically.
     *
     * The states are managed using function pointers stored in a vector, and transitions between states are controlled by returning the next state
     * from the current state's function.
     *
     * @note The order of states in the STATE enum and the m_stateFunctions vector must be consistent and maintained correctly.
     *
     * ### How to Add a New State:
     * 1. **Define the New State:**
     *    Add a new state to the STATE enum in the appropriate header file:
     *    @code
     *    enum class STATE {
     *        DRAW_ROOM,
     *        WAITING_FOR_INPUT,
     *        EXECUTE_AND_REPORT_COMMAND,
     *        GET_USER_INPUT,
     *        HELP,
     *        ATTACK,
     *        EXIT
     *        NEW_STATE,  // Add your new state here
     *    };
     *    @endcode
     *
     * 2. **Declare the New State Function:**
     *    Declare a new function for the state in GameController.h:
     *    @code
     *    STATE NewStateFunction();
     *    @endcode
     *
     * 3. **Define the New State Function:**
     *    Define the new function in GameController.cpp:
     *    @code
     *    STATE GameController::NewStateFunction()
     *    {
     *        // Implement the logic for the new state
     *        return STATE::NEXT_STATE;  // Return the next state after this one
     *    }
     *    @endcode
     *
     * 4. **Bind the New State Function:**
     *    Bind the new state function in the constructor of GameController:
     *    Ensure the order matches the STATE enum definition.
     *    @code
     *    , m_stateFunctions {
     *        std::bind(&GameController::DrawRoom, this),
     *        std::bind(&GameController::PrintUserOptions, this),
     *        std::bind(&GameController::WaitForInput, this),
     *        std::bind(&GameController::ExecuteAction, this),
     *        std::bind(&GameController::GetUserInput, this),
     *        std::bind(&GameController::ShowHelp, this),
     *        std::bind(&GameController::HandleAttack, this),
     *        std::bind(&GameController::NewStateFunction, this)  // Add binding for the new state
     *    }
     *    @endcode
     * 
     * 5. m_actions map associates a user command (COMMAND enum) with
     *    a unique_ptr to an Action subclass. The Action subclass implements 
     *    the logic for handling that specific command (from the user).
     *    m_commandStateMap maps a user command into state.
     *    adjust the m_actions map or m_commandStateMap (at the GameController constructor) if needed.
     *
     * ### How to Add a New Action:
     * 1. Create a new action class derived from Action in the appropriate header file.
     *
     * 2. Include the new action class header.
     *
     * 3. Add the new command to the COMMAND enum in the appropriate header file.
     *
     * 4. m_actions map associates a user command (COMMAND enum) with
     *    a unique_ptr to an Action subclass. The Action subclass implements 
     *    the logic for handling that specific command (from the user).
     *    Add the new action to the m_actions map in the GameController constructor.
     *    @note **IMPORTANT!!** The order of actions in the COMMAND enum must correspond to the order in m_actions.
     *
     * 5. m_actions object maps between a command into a state,
     *    adjust the m_actions map (at the GameController constructor) if needed
     * 
     */


public:
	explicit GameController(Writer& a_writer,
						  Reader& a_reader,
						  Dungeon_mt& a_dungeon,
						  Player& a_player);
	GameController(const GameController& a_other) = delete;
	GameController& operator=(const GameController& a_other) = delete;
	~GameController() = default;

	void Start();

private:

    // References to various game components
    Dungeon_mt& m_dungeon;
    Writer& m_writer;
    Reader& m_reader;
    Player& m_player;
    UserCommandParser m_userCommandParser;

    // State management variables
    STATE m_currentState;
    ActMap m_actions;
    std::optional<COMMAND> m_currentCommand;
    std::unordered_map<COMMAND, STATE> m_commandStateMap;

    // State (machine) functions 
    std::vector <std::function<STATE()>> m_stateFunctions;
    STATE DrawRoom();
    STATE WaitingForInput();
    STATE ExecuteAction();
    STATE GetUserInput();
    STATE Help();
    STATE AttackHandler();
    STATE CleanUp();

};

}