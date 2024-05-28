// DND_Main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "DND_lib/GameController.h"
#include "DND_lib/Player.h"
#include "CinReader.h"
#include "CoutWriter.h"

//#include "DND_lib/Dungeon_mt.h"

int main()
{
    // Allocate communication stream of type ostream and istream
    game_server::CoutWriter coutW { std::cout };
    game_server::CinReader cinR { std::cin };

    // Dungeon_mt object will be reponsible ...
    dnd_game::Dungeon_mt dungeon_mt {};
    dnd_game::Player player { "tmp name", coutW };  // tmp player for debugging
    dnd_game::GameController gameExe { coutW, cinR, dungeon_mt, player };
    gameExe.Execute();
    
}
