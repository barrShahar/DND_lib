#include "Room.h"
#include <random>
#include <cassert>

namespace dnd_game
{
Room_mt::Room_mt(Number a_roomNumber,
		   std::pair<bool, Number> a_isDoorNorth,
		   std::pair<bool, Number> a_isDoorEast,
		   std::pair<bool, Number> a_isDoorSouth,
		   std::pair<bool, Number> a_isDoorWest,
		   bool a_isDragon, bool a_isTreasure,
		   std::shared_ptr<Monster> a_monsterPtr)
	: // Initializer list
	m_isTreasure { a_isTreasure },
	m_subject { std::make_shared<SubjectRoom_mt>()},
	m_roomNumber { a_roomNumber },
	m_mutex {},
	m_walls { Wall(a_isDoorNorth.first, a_isDoorNorth.second),
	Wall {a_isDoorEast.first, a_isDoorEast.second },
	Wall {a_isDoorSouth.first, a_isDoorSouth.second },
	Wall { a_isDoorWest.first, a_isDoorWest.second } },
	m_monsterPtr { a_monsterPtr }
{
	// CTOR
}

//Room_mt::Room_mt(const Room_mt& a_other)
//	: m_isTreasure { a_other.m_isTreasure }
//	, m_roomNumber { a_other.m_roomNumber }
//	, m_walls { a_other.m_walls }
//	, m_monsterPtr { a_other.m_monsterPtr }
//	, m_subject{ a_other.m_subject }
//{
//	// COPY-CTOR
//	std::cout << "debug: Room_my COPY-CTOR\n";
//}

// Move constructor
Room_mt::Room_mt(Room_mt&& a_other) noexcept
	: m_isTreasure{ a_other.m_isTreasure },
	m_roomNumber{ a_other.m_roomNumber },
	m_walls{ std::move(a_other.m_walls) },
	m_monsterPtr{ std::move(a_other.m_monsterPtr) },
	m_subject{ std::move(a_other.m_subject) }
{
	// MOVE-CTOR
	std::cout << "debug: Room_mt MOVE-CTOR\n";

	// Reset moved-from object's optional members
	a_other.m_isTreasure = false;
	a_other.m_monsterPtr = nullptr;
	a_other.m_subject = nullptr;
}

std::string Room_mt::GetNames() const
{
	{	// guard
		std::unique_lock lock(m_mutex);
		return m_monsterPtr ? m_subject.get()->GetNames() + "monsters:" + ENDL + m_monsterPtr->GetName() + ENDL 
			: m_subject.get()->GetNames();
	}

}

std::vector<std::string> Room_mt::GetNamesVec() const
{
	return m_subject.get()->GetNamesVec();
}



void Room_mt::DrawRoom(Writer& a_wrier, Direction a_direction)
{
	std::cout << "m_subject Reference count: " << m_subject.use_count() << std::endl;
	Grid canvas { ASCII_AXIS_X, ASCII_AXIS_Y };
	DungeonAscii dungRoom { m_walls.at(0).IsDoor(),
		m_walls.at(1).IsDoor(),
		m_walls.at(2).IsDoor(),
		m_walls.at(3).IsDoor(),
		ContainsMonster(),
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
	try {
		if (m_monsterPtr.get()) {
			const std::vector<Direction> blockedDirections = m_monsterPtr->BlcokedDirections();
			for (Direction direction : blockedDirections) {
				if (direction == a_direction) {
					return std::nullopt;
				}
			}
		}

		return m_walls.at(static_cast<Number>(a_direction)).GetNextRoomNumber();
	}

	catch (...) 
	{
		assert(m_monsterPtr == nullptr);
	}
	
	// finally
	return m_walls.at(static_cast< Number >(a_direction)).GetNextRoomNumber();
}


std::optional<TREASURE_TYPE> Room_mt::GetTreasure_mt()
{
	{	// guard 
		std::unique_lock lock(m_mutex);
		if (!m_isTreasure)
		{
			return std::nullopt;
		}

		if (ContainsMonster())
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
		m_subject.get()->NotifyAll(a_player.GetName() + " has entered the room");
		m_subject.get()->Register(a_player);
	}
}

void Room_mt::Unregister(Player& a_player)
{
	{	// guard 
		std::unique_lock lock(m_mutex);
		m_subject.get()->NotifyAll(a_player.GetName() + " has left the room");
		m_subject.get()->Unregister(a_player);
	}
}

void Room_mt::NotifyAll(const std::string& a_message)
{
	{	// guard 
		std::unique_lock lock(m_mutex);
		m_subject.get()->NotifyAll(a_message);
	}
}

void Room_mt::NotifyAllExcept(const Player& a_excludedPlayer, const std::string& a_message)
{
	{	// guard 
		std::unique_lock lock(m_mutex);
		m_subject.get()->NotifyAllExcept(a_excludedPlayer, a_message);
	}
}

Room_mt::Iterator Room_mt::begin()
{
	return Room_mt::Iterator(m_walls.begin());
}

bool Room_mt::ContainsMonster() const
{
		return (m_monsterPtr != nullptr);
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

Room_mt::ConstIterator Room_mt::cbegin() const
{
	// CTOR
	return Room_mt::ConstIterator(m_walls.cbegin());
}

Room_mt::ConstIterator Room_mt::cend() const
{
	return Room_mt::ConstIterator(m_walls.cend());
}

Room_mt::ConstIterator Room_mt::begin() const
{
	return cbegin();
}

Room_mt::ConstIterator Room_mt::end() const
{
	return cend();
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

Room_mt::ConstIterator::ConstIterator(Walls::const_iterator a_it)
	: m_it { a_it }
{
	// CTOR
}

Room_mt::ConstIterator::ConstIterator(const ConstIterator& a_other)
	: m_it{ a_other.m_it }
{
	// CPY-CTOR

}

bool Room_mt::ConstIterator::operator!=(const ConstIterator& a_other)
{
	return m_it != a_other.m_it;
}

Room_mt::ConstIterator& Room_mt::ConstIterator::operator++()
{
	++m_it;
	return *this;
}

const Wall& Room_mt::ConstIterator::operator*() const
{
	return *m_it;
}

} // dnd_game