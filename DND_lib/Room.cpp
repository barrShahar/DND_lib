#include "Room.h"
#include <random>
namespace dnd_game
{
Room::Room(Number a_roomNumber,
		   std::pair<bool, Number> a_isDoorNorth,
		   std::pair<bool, Number> a_isDoorEast,
		   std::pair<bool, Number> a_isDoorSouth,
		   std::pair<bool, Number> a_isDoorWest,
		   bool a_isDragon, bool a_isTreasure)
	: // Initializer list
	m_isDragon { a_isDragon },
	m_isTreasure { a_isTreasure },
	m_roomNumber { a_roomNumber },
	m_dragon {},
	m_treasureDragonMutex {},
	m_walls { Wall(a_isDoorNorth.first, a_isDoorNorth.second),
	Wall {a_isDoorEast.first, a_isDoorEast.second },
	Wall {a_isDoorSouth.first, a_isDoorSouth.second },
	Wall { a_isDoorWest.first, a_isDoorWest.second } }
{
	// Ctor

}

Room::Room(const Room & a_other)
	: m_isDragon { a_other.m_isDragon }
	, m_isTreasure { a_other.m_isTreasure }
	, m_roomNumber { a_other.m_roomNumber }
	, m_walls { a_other.m_walls }
{
	// Copy ctor
}

std::string Room::GetNames() const
{
	return m_subject.GetNames();
}

void Room::DrawRoom(Writer& a_wrier, Direction a_direction)
{
	Grid canvas { ASCII_AXIS_X, ASCII_AXIS_Y };
	DungeonAscii dungRoom { m_walls.at(0).IsDoor(),
		m_walls.at(1).IsDoor(),
		m_walls.at(2).IsDoor(),
		m_walls.at(3).IsDoor(),
		m_isDragon,
		m_isTreasure };
	dungRoom.Draw(canvas, a_direction, ASCII_AXIS_X, ASCII_AXIS_Y);
	canvas.Print(a_wrier);
}

bool Room::isDoor(Direction a_direction) const
{
	return m_walls.at(static_cast< Number >(a_direction)).IsDoor();
}

Number Room::GetNextDoorRoomNumber(Direction a_direction) const
{
	return m_walls.at(static_cast< Number >(a_direction)).GetNextRoomNumber();
}

//AttackPlayerResponse Room::AttackPlayer(const std::string& a_attackedName, Number a_dmg)
//{
//	return m_subject.AttackPlayer(a_attackedName, a_dmg);
//}

//AttackDragonResponse Room::AttackDragon(Number a_dmg)
//{
//	std::lock_guard<std::mutex> lock(m_treasureDragonMutex);
//	if (!m_isDragon)
//	{
//		AttackDragonResponse respone;
//		respone.m_damageToPlayer = 0;
//		respone.m_dragonRemainingLife = 0;
//		return respone;
//	}
//
//	Number returnDmg = m_dragon.DamageAndAttack(a_dmg);
//	AttackDragonResponse respone;
//	respone.m_damageToPlayer = returnDmg;
//	respone.m_dragonRemainingLife = m_dragon.GetLifePoints();
//	if (!m_dragon.IsAlive())
//	{
//		m_isDragon = false;
//	}
//	return respone;
//}

std::optional<TREASURE> Room::GetTreasure_mt()
{
	std::lock_guard<std::mutex> lock(m_treasureDragonMutex);
	if (!m_isTreasure)
	{
		return std::nullopt;
	}
	// Seed the random number generator with a random device
	std::random_device rd;
	std::mt19937 gen(rd());

	// Define the range of random numbers
	std::uniform_int_distribution<> dis(0, Number(TREASURE::SIZE) - 1);

	// Generate a random number
	int randomNum = dis(gen);
	m_isTreasure = false;
	return std::make_optional(static_cast< TREASURE >(randomNum));
}

void Room::Register(Player& a_player)
{
	m_subject.Register(a_player);
}

void Room::Unregister(Player& a_player)
{
	m_subject.Unregister(a_player);
}

void Room::NotifyAll(const std::string& a_message)
{
	m_subject.NotifyAll(a_message);
}

Room::Iterator Room::begin()
{
	return Room::Iterator(m_walls.begin());
}

bool Room::IsDragon() const
{
	std::lock_guard<std::mutex> lock(m_treasureDragonMutex);
	return m_isDragon;
}

bool Room::IsTreasure() const
{
	return m_isTreasure;
}

Room::Iterator Room::end()
{
	return Room::Iterator(m_walls.end());
}

Room::Iterator::Iterator(const Walls::iterator& a_it)
	: m_it { a_it }
{
	// Iterator Ctor
}

Room::Iterator::Iterator(const Iterator & a_other)
	: m_it { a_other.m_it }
{
	// Iterator copy ctor
}



bool Room::Iterator::operator!=(const Iterator& a_other)
{
	return m_it != a_other.m_it;
}

Room::Iterator& Room::Iterator::operator++()
{
	++m_it;
	return *this;
}

Wall& Room::Iterator::operator*()
{
	return *m_it;
}

} // dnd_game