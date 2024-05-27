#include "DungeonAscii.h"
#include <cassert>


dnd_game::DungeonAscii::DungeonAscii(bool a_up,
								  bool a_right,
								  bool a_down,
								  bool a_left,
								  bool a_isDragon,
								  bool a_isTreasure)
	: m_up { a_up }
	, m_right { a_right }
	, m_down { a_down }
	, m_left { a_left }
	, m_isDragon { a_isDragon }
	, m_isTreasure { a_isTreasure }
{
	// Ctor
}

void DrawHorizontalLine(dnd_game::Grid& a_canvas, int a_xLeft, int a_xRight, int a_y)
{
	for (int i = a_xLeft; i <= a_xRight; ++i)
	{
		a_canvas(i, a_y) = dnd_game::HORIZONTAL_WALL_SYMBOL;
	}
}

void DrawVerticalLine(dnd_game::Grid& a_canvas, int a_yUp, int a_yDown, int a_x)
{
	for (size_t i = a_yUp; i < a_yDown; ++i)
	{
		a_canvas(a_x, i) = dnd_game::VERTICAL_WALL_SYMBOL;
	}
}


void dnd_game::DungeonAscii::Draw(Grid& a_canvas, Direction a_direction, int a_baseX, int a_baseY) const
{
	// room corners
	int xLeft = 2;
	int xRight = a_baseX - xLeft;
	int yUp = 3;
	int yDown = a_baseY - yUp;

	DrawHorizontalLine(a_canvas, xLeft, xRight, yUp); // draw upper wall
	DrawHorizontalLine(a_canvas, xLeft, xRight, yDown); // draw bottom wall
	DrawVerticalLine(a_canvas, yUp, yDown, xLeft);	  // draw left wall
	DrawVerticalLine(a_canvas, yUp, yDown, xRight);	  // draw right wall

	// Draw dragon

	int DRAGON_BASE_Y = yUp + 1;
	int DRAGON_BASE_X = a_baseX / 2 - DRAGON_LENGTH / 2 ;

	if (m_isDragon)
	{
		// Draw dragon
		int DRAGON_BASE_Y = yUp + 1;
		int DRAGON_BASE_X = a_baseX / 2 - DRAGON_LENGTH / 2 ;

		for (int y = 0; y < DRAGON_HEIGHT; ++y)
		{
			for (int x = 0; x < DRAGON_LENGTH; ++x)
			{
				a_canvas(DRAGON_BASE_X + x, DRAGON_BASE_Y + y) = dragon[y][x];
			}

			// Wall aligment Adjusment
			a_canvas(xRight, y + DRAGON_BASE_Y) = ' ';
			a_canvas(xRight + 1, y + DRAGON_BASE_Y) = VERTICAL_WALL_SYMBOL;
		}

	}
	int doorLenghtX = a_baseX / 4;
	int doorLengthY = doorLenghtX / 2;


	// up door
	if (m_up)
	{
		for (size_t i = 0 ; i < doorLenghtX; ++i)
		{
			a_canvas(i + (a_baseX - doorLenghtX) / 2, yUp) = ' ';
		}
		a_canvas((a_baseX - doorLenghtX) / 2, yUp) = '*';
		a_canvas((a_baseX - doorLenghtX) / 2, yUp - 1) = '*';
		a_canvas(doorLenghtX + (a_baseX - doorLenghtX) / 2, yUp) = '*';
		a_canvas(doorLenghtX + (a_baseX - doorLenghtX) / 2, yUp - 1) = '*';
	}
	// Down door
	if (m_down)
	{
		for (size_t i = 0 ; i < doorLenghtX; ++i)
		{
			a_canvas(i + (a_baseX - doorLenghtX) / 2, yDown) = ' ';
		}
		a_canvas((a_baseX - doorLenghtX) / 2, yDown) = '*';
		a_canvas((a_baseX - doorLenghtX) / 2, yDown + 1) = '*';
		a_canvas(doorLenghtX + (a_baseX - doorLenghtX) / 2, yDown) = '*';
		a_canvas(doorLenghtX + (a_baseX - doorLenghtX) / 2, yDown + 1) = '*';

	}
	// Right door

	if (m_right)
	{
		for (size_t i = 0 ; i < doorLengthY; ++i)
		{
			a_canvas(xRight, i + (a_baseY + doorLengthY) / 2) = ' ';
		}

		a_canvas(xRight, (a_baseY + doorLengthY) / 2) = '*';
		a_canvas(xRight + 1, (a_baseY + doorLengthY) / 2) = '*';

		a_canvas(xRight, doorLengthY + (a_baseY + doorLengthY) / 2) = '*';
		a_canvas(xRight + 1, doorLengthY + (a_baseY + doorLengthY) / 2) = '*';

	}
	// Left door
	if (m_left)
	{
		for (size_t i = 0 ; i < doorLengthY; ++i)
		{
			a_canvas(xLeft, i + (a_baseY + doorLengthY) / 2) = ' ';
		}
		a_canvas(xLeft, (a_baseY + doorLengthY) / 2) = '*';
		a_canvas(xLeft - 1, (a_baseY + doorLengthY) / 2) = '*';

		a_canvas(xLeft, doorLengthY + (a_baseY + doorLengthY) / 2) = '*';
		a_canvas(xLeft - 1, doorLengthY + (a_baseY + doorLengthY) / 2) = '*';
	}

	if (m_isTreasure)
	{
		int treasureBaseX = xLeft + 2;
		int treasureBaseY = yUp + 2;
		static std::string trasure = "!Treasure!";
		for (int i = 0; i < trasure.length(); ++i)
		{
			a_canvas(treasureBaseX + i, treasureBaseY - 1) = '!';
			a_canvas(treasureBaseX + i, treasureBaseY) = trasure[i];
			a_canvas(treasureBaseX + i, treasureBaseY + 1) = '!';

		}
	}

	int playerBaseX = a_baseX / 2;
	int playerBaseY = a_baseY / 2 + doorLengthY * 4 / 3;
	DrawPlayer(a_canvas, playerBaseX, playerBaseY, a_direction);
}

void dnd_game::DungeonAscii::DrawPlayer(Grid & a_canvas, int a_baseX, int a_baseY, Direction a_direction) const
{
	switch (a_direction)
	{
		case dnd_game::Direction::NORTH:
		{
			a_canvas(a_baseX, a_baseY) = '/';
			a_canvas(a_baseX + 1, a_baseY) = '\\';
			a_canvas(a_baseX - 1, a_baseY + 1) = '/';
			a_canvas(a_baseX + 1 + 1, a_baseY + 1) = '\\';
			break;
		}
		case dnd_game::Direction::WEST:
		{
			a_canvas(a_baseX, a_baseY) = '\\';
			a_canvas(a_baseX, a_baseY + 1) = '/';
			break;
		}
		case dnd_game::Direction::SOUTH:
		{
			a_canvas(a_baseX, a_baseY + 1) = '\\';
			a_canvas(a_baseX + 1, a_baseY + 1) = '/';
			a_canvas(a_baseX - 1, a_baseY) = '\\';
			a_canvas(a_baseX + 1 + 1, a_baseY) = '/';
			break;
		}
		case dnd_game::Direction::EAST:
		{
			a_canvas(a_baseX, a_baseY) = '/';
			a_canvas(a_baseX, a_baseY + 1) = '\\';
			break;
		}
		default:
			assert(!"no such enume Direction\n");
			break;
	}
}
