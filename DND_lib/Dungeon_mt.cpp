#include "Dungeon_mt.h"
#include "GameParams.h"
#include <cassert>


#include <vector>
//#include <random>
//#include <queue>

namespace dnd_game
{
dnd_game::Dungeon_mt::Dungeon_mt()
	: m_dungeonGuard {}
	, m_mtx {}
	, m_rooms { CreateDungeon() }
{
	// CTOR 
}

Number Dungeon_mt::GetEntryRoom() const
{
	return Dungeon_mt::ENTRY_ROOM;
}

const Room_mt& Dungeon_mt::GetRoom(Number a_roomNumber) const
{
	return m_rooms[a_roomNumber];
}

void Dungeon_mt::DrawRoom(Writer& a_writer, Number a_roomNum, Direction a_playerDirection)
{
	m_rooms.at(a_roomNum).DrawRoom(a_writer, a_playerDirection);
}

std::string Dungeon_mt::Walk_mt(Player& a_player)
{
	const Direction playerDirection = a_player.GetDirection();
	Room_mt& currentRoom = m_rooms[a_player.GetRoomNumber()];

	if (!currentRoom.isDoor(playerDirection))
	{
		return "There is no door in that direction!";
	}

	const std::optional<Number> newRoomNumber = currentRoom.GetNextDoorRoomNumber(playerDirection);
	if (!newRoomNumber.has_value())
	{
		return "Path is blocked";
	}


	currentRoom.Unregister(a_player);	// Unregister player(observer) from the subject(room)
	m_rooms[newRoomNumber.value()].Register(a_player);	// Register player to the next room
	a_player.SetRoomNumber(newRoomNumber.value());	
	

	return "You've entered room " + std::to_string(newRoomNumber.value());

}


void Dungeon_mt::NotifyRoom(Number a_roomNumber, const std::string& a_message)
{
	Room_mt& room = m_rooms.at(a_roomNumber);
	room.NotifyAll(a_message);
}

void Dungeon_mt::NotifyRoomExcept(const Player& a_excludedPlayer, Number a_roomNumber, const std::string& a_message)
{
	Room_mt& room = m_rooms.at(a_roomNumber);
	room.NotifyAllExcept(a_excludedPlayer, a_message);
}



bool Dungeon_mt::IsMonsterInTheRoom(Number a_roomNumber)
{
	return m_rooms.at(a_roomNumber).ContainsMonster();
}

std::string Dungeon_mt::GetNames(Number a_roomNumber) const
{
	const Room_mt& room = m_rooms.at(a_roomNumber);
	std::string names = room.GetNames();
	return room.GetNames();
}

void Dungeon_mt::ShoutAction(Number a_roomNumber, const std::string& a_message)
{
	Room_mt& room = m_rooms.at(a_roomNumber);
	for (Wall& wall : room)
	{
		if (wall.IsDoor())
		{
			NotifyRoom(wall.GetNextRoomNumber(), a_message);
		}
	}
	NotifyRoom(a_roomNumber, a_message);
}

void Dungeon_mt::RegisterPlayer(Player& a_player, Number a_roomNumber)
{
	m_rooms.at(a_roomNumber).Register(a_player);
}

void Dungeon_mt::UnregisterPlayer(Player& a_player, Number a_roomNumber)
{
	m_rooms.at(a_roomNumber).Unregister(a_player);
}

//AttackPlayerResponse Dungeon_mt::AttackPlayer(Number a_roomNumber,
//											  const std::string& attackerName,
//											  const std::string& a_attackedName,
//											  Number a_dmg, Number a_attackLifePoints)
//{
//	// TODO: Find player through subject and attack him
//	Room_mt& room = m_rooms.at(a_roomNumber);
//	AttackPlayerResponse info = room.AttackPlayer(a_attackedName, a_dmg);
//	Number lifePoints = (a_attackLifePoints > info.GetDamageToThisPlayer()) ?
//		a_attackLifePoints - info.GetDamageToThisPlayer() : NUMBER_ZERO;
//	info.m_PlayerRemainingLife = lifePoints;
//	if (info.m_isValid)
//	{
//		// notify other dudes
//		std::string message = "\n" + attackerName + " inflicted " + std::to_string(a_dmg) + " to " + a_attackedName + "!\n";
//		message += a_attackedName + " dealth " + std::to_string(info.GetDamageToThisPlayer()) + " back.\n";
//		message += attackerName + " life remaining: " + std::to_string(lifePoints) + "\n";
//		message += a_attackedName + " remaining life points: " + std::to_string(info.m_otherPlayerRemainingLife);
//		NotifyRoom(a_roomNumber, message);
//	}
//	return info;
//}

//AttackDragonResponse Dungeon_mt::AttackDragon(Number a_roomNumber, Number a_dmg)
//{
//	Room_mt& room = m_rooms.at(a_roomNumber);
//	return room.AttackDragon(a_dmg);
//}

std::optional<TREASURE_TYPE> Dungeon_mt::GetTreasure(Number a_roomNumber)
{
	return 	m_rooms.at(a_roomNumber).GetTreasure_mt();
}

const dnd_game::Rooms Dungeon_mt::CreateDungeon() const
{
	using std::make_pair;
	// Hard coded dungeon
	Number roomNumber = 0;
/*
	Rooms rooms // return value
	{
		Room_mt { NUMBER_ZERO, make_pair(true,Number(1)),make_pair(false,-1), make_pair(false,-1), make_pair(true, Number(2)), false, true, nullptr},
		Room_mt { Number(1), make_pair(true,Number(3)),make_pair(true,Number(4)), make_pair(true,NUMBER_ZERO), make_pair(false, -1), false, false, nullptr },
		Room_mt { Number(2), make_pair(false,-1),make_pair(true,NUMBER_ZERO), make_pair(false,-1), make_pair(false, -1), true, true, std::make_shared<Dragon>()},
		Room_mt { Number(3), make_pair(true,Number(7)),make_pair(true,Number(5)), make_pair(true,Number(1)), make_pair(false, -1), true, true, std::make_shared<Dragon>() },
		Room_mt { Number(4), make_pair(true,Number(5)),make_pair(false,Number(-1)), make_pair(false,Number(-1)), make_pair(true, Number(1)), false, true, nullptr },
		Room_mt { Number(5), make_pair(true,Number(6)),make_pair(false,Number(-1)), make_pair(true,Number(4)), make_pair(true, Number(3)), false, false, nullptr },
		Room_mt { Number(6), make_pair(false,Number(-1)),make_pair(false,Number(-1)), make_pair(true,Number(5)), make_pair(true, Number(7)), false, false, nullptr },
		Room_mt { Number(8), make_pair(false,Number(-1)),make_pair(true,Number(6)), make_pair(true,Number(3)), make_pair(false, Number(-1)), true, true, std::make_shared<Dragon>() },
	};
	return rooms;
*/

	Rooms rooms; // return value
	rooms.emplace_back(NUMBER_ZERO, make_pair(true, Number(1)), make_pair(false, -1), make_pair(false, -1), make_pair(true, Number(2)), true, nullptr);
	rooms.emplace_back(Number(1), make_pair(true, Number(3)), make_pair(true, Number(4)), make_pair(true, NUMBER_ZERO), make_pair(false, -1), false, nullptr);
	rooms.emplace_back(Number(2), make_pair(false, -1), make_pair(true, NUMBER_ZERO), make_pair(false, -1), make_pair(false, -1), true, std::make_shared<Dragon>());
	rooms.emplace_back(Number(3), make_pair(true, Number(7)), make_pair(true, Number(5)), make_pair(true, Number(1)), make_pair(false, -1), true, std::make_shared<Dragon>());
	rooms.emplace_back(Number(4), make_pair(true, Number(5)), make_pair(false, -1), make_pair(false, -1), make_pair(true, Number(1)), true, nullptr);
	rooms.emplace_back(Number(5), make_pair(true, Number(6)), make_pair(false, -1), make_pair(true, Number(4)), make_pair(true, Number(3)), false, nullptr);
	rooms.emplace_back(Number(6), make_pair(false, -1), make_pair(false, -1), make_pair(true, Number(5)), make_pair(true, Number(7)), false, nullptr);
	rooms.emplace_back(Number(8), make_pair(false, -1), make_pair(true, Number(6)), make_pair(true, Number(3)), make_pair(false, -1), true, std::make_shared<Dragon>());

	return std::move(rooms);

	
}

Number AttackDragonResponse::GetDragonRemainingLifePoints()
{
	return m_dragonRemainingLife;
}

Number AttackDragonResponse::GetDamageToPlayer() const
{
	return m_damageToPlayer;
}

} // dnd_game


/*
*
std::pair<bool,Number> getRandomBool_mt(double a_probability, Number& a_num)
{
	bool isDoor = getRandomBool_mt(a_probability);
	return isDoor ? std::make_pair(isDoor, ++a_num) : std::make_pair(isDoor, Number(0));
}

std::unique_ptr<Room_mt> CreateRoom(Number& a_roomNumber)
{
	std::array<std::pair<bool,Number>, 4> bools;
	std::generate(bools.begin(), bools.end(), [&] ()
				  {
					  return getRandomBool_mt(IS_DOOR_PROBABILITY, a_roomNumber);
				  });

	Room_mt
	{
	a_roomNumber,
	bools.at(0),
	bools.at(1),
	bools.at(2),
	bools.at(3),
	getRandomBool_mt(IS_DRAGON_PROBABILITY),
	getRandomBool_mt(IS_TREASURE_PROBABILITY)
	};

	std::unique_ptr<Room_mt> returnValue = std::make_unique<Room_mt>(
		a_roomNumber,
		bools.at(0),
		bools.at(1),
		bools.at(2),
		bools.at(3),
		getRandomBool_mt(IS_DRAGON_PROBABILITY),
		getRandomBool_mt(IS_TREASURE_PROBABILITY));

	return std::move(returnValue);
}

// This function is thread safe since there is no shared memory between
// threads, each thread allocate is own variables each time.
// Also, this function called only once befor creating new threads
bool getRandomBool_mt(double a_probability)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::bernoulli_distribution dist(a_probability);
	return dist(gen);
}
*/