#include "Room.h"
#include <random>
#include <cassert>

namespace dnd_game
{
Room::Room(Number a_roomNumber,
		   std::pair<bool, Number> a_isDoorNorth,
		   std::pair<bool, Number> a_isDoorEast,
		   std::pair<bool, Number> a_isDoorSouth,
		   std::pair<bool, Number> a_isDoorWest,
		   bool a_isTreasure,
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

//Room::Room(const Room& a_other)
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
Room::Room(Room&& a_other) noexcept
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

std::string Room::GetNames_mt() const
{
	{	// guard
		std::unique_lock lock(m_mutex);
		return m_monsterPtr ? m_subject.get()->GetNames_mt() + "monsters:" + ENDL + m_monsterPtr->GetName() + ENDL 
			: m_subject.get()->GetNames_mt();
	}

}

std::vector<std::string> Room::GetNamesVec() const
{
	return m_subject.get()->GetNamesVec();
}

std::optional<std::shared_ptr<Monster>> Room::GetMonster()
{
	return m_monsterPtr;
}



void Room::DrawRoom(Writer& a_wrier, Direction a_direction)
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

bool Room::isDoor(Direction a_direction) const
{
	return m_walls.at(static_cast< Number >(a_direction)).IsDoor();
}

std::optional<Number> Room::GetNextDoorRoomNumber(Direction a_direction) const
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


std::optional<TREASURE_TYPE> Room::GetTreasure_mt()
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

void Room::Register_mt(Player& a_player)
{
	{	// guard 
		std::unique_lock lock(m_mutex);
		Register_NoLock(a_player);
	}
}

void Room::Register_NoLock(Player& a_player)
{
	m_subject.get()->NotifyAll_mt(a_player.GetName() + " has entered the room");
	m_subject.get()->Register_mt(a_player);
}


void Room::Unregister_mt(Player& a_player)
{
	{	// guard 
		std::unique_lock lock(m_mutex);
		Unregister_NoLock(a_player);
	}
}

void Room::Unregister_NoLock(Player& a_player)
{
	m_subject.get()->NotifyAll_mt(a_player.GetName() + " has left the room");
	m_subject.get()->Unregister_mt(a_player);
}

void Room::NotifyAll_mt(const std::string& a_message)
{
	{	// guard 
		std::unique_lock lock(m_mutex);
		NotifyAll_NoLock(a_message);
	}
}

void Room::NotifyAll_NoLock(const std::string& a_message)
{
	m_subject.get()->NotifyAll_mt(a_message);
}

void Room::NotifyAllExcept_mt(const Player& a_excludedPlayer, const std::string& a_message)
{
	{	// guard 
		std::unique_lock lock(m_mutex);
		NotifyAllExcept_NoLock(a_excludedPlayer, a_message);
	}
}

void Room::NotifyAllExcept_NoLock(const Player& a_excludedPlayer, const std::string& a_message)
{
	m_subject.get()->NotifyAllExcept_mt(a_excludedPlayer, a_message);
}


Room::Iterator Room::begin()
{
	return Room::Iterator(m_walls.begin());
}

bool Room::ContainsMonster() const
{
		return (m_monsterPtr != nullptr);
}

bool Room::IsTreasure_mt() const
{
	{	// Guard 
		std::shared_lock lock(m_mutex);
		return m_isTreasure;
	}
}

Room::Iterator Room::end()
{
	return Room::Iterator(m_walls.end());
}

Room::ConstIterator Room::cbegin() const
{
	// CTOR
	return Room::ConstIterator(m_walls.cbegin());
}

Room::ConstIterator Room::cend() const
{
	return Room::ConstIterator(m_walls.cend());
}

Room::ConstIterator Room::begin() const
{
	return cbegin();
}

Room::ConstIterator Room::end() const
{
	return cend();
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

Room::ConstIterator::ConstIterator(Walls::const_iterator a_it)
	: m_it { a_it }
{
	// CTOR
}

Room::ConstIterator::ConstIterator(const ConstIterator& a_other)
	: m_it{ a_other.m_it }
{
	// CPY-CTOR

}

bool Room::ConstIterator::operator!=(const ConstIterator& a_other)
{
	return m_it != a_other.m_it;
}

Room::ConstIterator& Room::ConstIterator::operator++()
{
	++m_it;
	return *this;
}

const Wall& Room::ConstIterator::operator*() const
{
	return *m_it;
}

} // dnd_game