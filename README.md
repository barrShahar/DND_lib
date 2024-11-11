# DND Game Server

A C++ project that implements a Dungeon & Dragons-inspired multiplayer game server. Players can connect via the network and explore a shared dungeon, interact with each other, and engage in combat.

## Features

- Multiplayer game server supporting multiple players interacting in a shared dungeon.
- Uses Boost.Asio for networking to handle client connections.
- Implements SOLID principles and the Strategy pattern to handle different player actions.


## **Requirements**
   - Requires Boost.Asio and C++17 or above.

## Connecting to the Server

Players can connect using Telnet or a similar tool:
```sh
telnet <server-ip> 4010


Project Structor: 

├── DND_main (Using boost)
│   ├── header
│   │   ├── NetReader.h      # Header file defining the NetReader class, responsible for reading input from network connections.
│   │   ├── NetWriter.h      # Header file defining the NetWriter class, responsible for writing output to network connections.
│   │   └── GameServer.h     # Header file for the GameServer class, managing the connection of players to the game and handling server-related operations.
│   └── source
│       ├── DND_Main.cpp     # Entry point of the game server. Responsible for initializing the server, setting up connections, and starting the game loop.
│       ├── GameServer.cpp   # Implementation of the GameServer class, managing player connections, creating threads, and interacting with the game components.
│       ├── NetReader.cpp    # Implementation of the NetReader class. Handles the logic for reading commands and data from connected clients via network.
│       └── NetWriter.cpp    # Implementation of the NetWriter class. Handles writing data and responses back to the clients connected to the server.
├── DND_staticLib
│   ├── header
│   │   ├── Game
│   │   │   ├── GameController.h      # Manages the state and actions of the game, utilizing the Strategy design pattern to handle different game actions.
│   │   │   ├── Action.h              # Strategy design pattern, contains base and derived classes 
│   │   │   ├── ShoutAction.h         # Shout specific action (see Action.h)
│   │   │   ├── WalkAction.h          # Walking action class (see Action.h)
│   │   │   ├── TakeAction.h          # Take treasure action (see Action.h)
│   │   │   ├── LookAction.h          # Look around action (see Action.h)
│   │   │   ├── AttackAction.h        # Attack specific action class (see Action.h)
│   │   │   ├── AttackService.h       # Service for managing attack logic 
│   │   │   ├── GameParams.h          # Game parameters configuration
│   │   │   └── Player.h              # Player class definition
│   │   ├── Dungeon
│   │   │   ├── Dungeon_mt.h          # Manages rooms, monsters, and players in the dungeon
│   │   │   ├── Room.h                # Room class definition
│   │   │   ├── Wall.h                # Wall definition within a room
│   │   │   ├── Monsters
│   │   │   │   ├── Monster.h         # Base class for monsters
│   │   │   │   ├── Dragon.h          # Dragon derived class
│   │   │   ├── Treasures
│   │   │   │   ├── Treasure.h        # Contains base and derived classes for treasures
│   │   │   │   ├── TreasureBase.h    # Base treasure class, possibly abstract
│   │   └── IO
│   │   │   ├── Reader.h              # Abstract base class for reading
│   │   │   ├── Writer.h              # Abstract base class for writing
│   │   │   ├── CinReader.h           # Reads from standard input (cin)
│   │   │   ├── CoutWriter.h          # Writes to standard output (cout)
│   │   └── SubjectObserver
│   │   │   ├── ObserverPlayer_mt.h   # Observer class representing a player in the subject
│   │   │   └── SubjectRoom_mt.h      # Subject that observers (players) can register to
│   │   └── Utilities
│   │   │   ├── ActionResponse.h          # Base class, reply from actions
│   │   │   ├── StringActionResponse.h    # Action response with a string reply
│   │   │   ├── RnStringActionResponse.h  # Specialized response for Windows
│   │   │   ├── TextActionResponse.h      # Generic text response
│   │   │   ├── UserCommandParser.h       # Parses user text commands
│   │   │   └── IAttackable.h             # Interface for attackable entities
│   └── source
│       ├── Game
│       │   ├── GameController.cpp
│       │   ├── Action.cpp
│       │   ├── AttackAction.cpp
│       │   ├── AttackService.cpp
│       │   ├── ShoutAction.cpp
│       │   ├── WalkAction.cpp
│       │   ├── TakeAction.cpp
│       │   ├── LookAction.cpp
│       │   └── GameParams.cpp
│       ├── Dungeon
│       │   ├── Dungeon_mt.cpp
│       │   ├── Room.cpp
│       │   ├── Wall.cpp
│       │   ├── Monsters
│       │   │   ├── Dragon.cpp
│       │   │   └── Monster.cpp
│       │   ├── Treasures
│       │   │   ├── Treasure.cpp
│       │   │   └── TreasureBase.cpp
│       ├── IO
│       │   ├── CinReader.cpp
│       │   ├── CoutWriter.cpp
│       └── SubjectObserver
│           ├── ObserverPlayer_mt.cpp
│           └── SubjectRoom_mt.cpp
│       ├── Utilities
│       │   ├── ActionResponse.cpp
│       │   ├── StringActionResponse.cpp
│       │   ├── RnStringActionResponse.cpp
│       │   ├── TextActionResponse.cpp
│       │   └── UserCommandParser.cpp

