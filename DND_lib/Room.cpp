#include "Room.h"
#include <random>
namespace dnd_game
{
Room_mt::Room_mt(Number a_roomNumber,
		   std::pair<bool, Number> a_isDoorNorth,
		   std::pair<bool, Number> a_isDoorEast,
		   std::pair<bool, Number> a_isDoorSouth,
		   std::pair<bool, Number> a_isDoorWest,
		   bool a_isDragon, bool a_isTreasure)
	: // Initializer list
	m_containsMonster { a_isDragon },
	m_isTreasure { a_isTreasure },
	m_roomNumber { a_roomNumber },
	m_dragon {},
	m_mutex {},
	m_walls { Wall(a_isDoorNorth.first, a_isDoorNorth.second),
	Wall {a_isDoorEast.first, a_isDoorEast.second },
	Wall {a_isDoorSouth.first, a_isDoorSouth.second },
	Wall { a_isDoorWest.first, a_isDoorWest.second } }
{
	// CTOR
}

Room_mt::Room_mt(const Room_mt& a_other)
	: m_containsMonster { a_other.m_containsMonster }
	, m_isTreasure { a_other.m_isTreasure }
	, m_roomNumber { a_other.m_roomNumber }
	, m_walls { a_other.m_walls }
{
	// COPY-CTOR
}

std::string Room_mt::GetNames() const
{
	return m_subject.GetNames();
}

void Room_mt::DrawRoom(Writer& a_wrier, Direction a_direction)
{
	Grid canvas { ASCII_AXIS_X, ASCII_AXIS_Y };
	DungeonAscii dungRoom { m_walls.at(0).IsDoor(),
		m_walls.at(1).IsDoor(),
		m_walls.at(2).IsDoor(),
		m_walls.at(3).IsDoor(),
		m_containsMonster,
		m_isTreasure };
	dungRoom.Draw(canvas, a_direction, ASCII_AXIS_X, ASCII_AXIS_Y);
	canvas.Print(a_wrier);
}

bool Room_mt::isDoor(Direction a_direction) const
{
	return m_walls.at(static_cast< Number >(a_direction)).IsDoor();
}

std::optional<Number> Room_mt::GetNextDoorRoomNumber(Direction a_direction) const
{
	if (m_containsMonster)
	{
		const std::vector<Direction> blockedDirections = m_dragon.BlcokedDirections();
		for (Direction direction : blockedDirections)
		{
			if (direction == a_direction)
			{
				return std::nullopt;
			}
		}
	}
	
	return m_walls.at(static_cast< Number >(a_direction)).GetNextRoomNumber();
}

//AttackPlayerResponse Room_mt::AttackPlayer(const std::string& a_attackedName, Number a_dmg)
//{
//	return m_subject.AttackPlayer(a_attackedName, a_dmg);
//}

//AttackDragonResponse Room_mt::AttackDragon(Number a_dmg)
//{
//	std::lock_guard<std::mutex> lock(m_treasureDragonMutex);
//	if (!m_containsMonster)
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
//		m_containsMonster = false;
//	}
//	return respone;
//}

std::optional<TREASURE_TYPE> Room_mt::GetTreasure_mt()
{
	{	// guard 
		std::unique_lock lock(m_mutex);
		if (!m_isTreasure)
		{
			return std::nullopt;
		}

		if (m_containsMonster)
		{
			return TREASURE_TYPE::GUARDED;
		}

		// Return random type of treasure (random number from TREASURE_TYPE enume
		// Seed the random number generator with a random device
		std::random_device rd;
		std::mt19937 gen(rd());

		// Define the range of random numbers
		std::uniform_int_distribution<> dis(0, Number(TREASURE_TYPE::SIZE) - 1);

		// Generate a random number
		int randomNum = dis(gen);
		m_isTreasure = false;
		return std::make_optional(static_cast< TREASURE_TYPE >(randomNum));
	}
	
}

void Room_mt::Register(Player& a_player)
{
	{	// guard 
		std::unique_lock lock(m_mutex);
		m_subject.Register(a_player);
		m_subject.NotifyAll(a_player.GetName() + " has entered the room");
	}
}

void Room_mt::Unregister(Player& a_player)
{
	{	// guard 
		std::unique_lock lock(m_mutex);
		m_subject.NotifyAll(a_player.GetName() + " has left the room");
		m_subject.Unregister(a_player);
	}
}

void Room_mt::NotifyAll(const std::string& a_message)
{
	{	// guard 
		std::unique_lock lock(m_mutex);
		m_subject.NotifyAll(a_message);
	}
}

void Room_mt::NotifyAllExcept(const Player& a_excludedPlayer, const std::string& a_message)
{
	{	// guard 
		std::unique_lock lock(m_mutex);
		m_subject.NotifyAllExcept(a_excludedPlayer, a_message);
	}
}

Room_mt::Iterator Room_mt::begin()
{
	return Room_mt::Iterator(m_walls.begin());
}

bool Room_mt::ContainsMonster() const
{
	{	// Guard
		std::shared_lock lock(m_mutex);
		return m_containsMonster;
	}
}

bool Room_mt::IsTreasure() const
{
	{	// Guard 
		std::shared_lock lock(m_mutex);
		return m_isTreasure;
	}
}

Room_mt::Iterator Room_mt::end()
{
	return Room_mt::Iterator(m_walls.end());
}

Room_mt::Iterator::Iterator(const Walls::iterator& a_it)
	: m_it { a_it }
{
	// Iterator Ctor
}

Room_mt::Iterator::Iterator(const Iterator & a_other)
	: m_it { a_other.m_it }
{
	// Iterator copy ctor
}



bool Room_mt::Iterator::operator!=(const Iterator& a_other)
{
	return m_it != a_other.m_it;
}

Room_mt::Iterator& Room_mt::Iterator::operator++()
{
	++m_it;
	return *this;
}

Wall& Room_mt::Iterator::operator*()
{
	return *m_it;
}

} // dnd_game