 #include "ascii/Grid.h"

namespace dnd_game
{
dnd_game::Grid::Grid(size_t a_x, size_t a_y)
	: m_grid { std::make_unique<char[]>(a_x * a_y) }
	, m_maxX { a_x }
	, m_maxY { a_y }
{
    Clean();
}

char& Grid::operator()(size_t x, size_t y)
{
	return m_grid[x + y * m_maxX];
}

void dnd_game::Grid::Clean()
{
    for (int i = 0; i < m_maxX; i++)
    {
        for (int j = 0; j < m_maxY; j++)
        {
            (*this)(i, j) = ' ';
        }
    }
}

void Grid::Print(Writer& a_writer) const
{
    for (size_t y = 0; y < m_maxY; ++y)
    {
        for (size_t x = 0; x < m_maxX; ++x)
        {
            //if (m_grid[i * m_maxX + j] == 0)
            //{
            //    a_writer << ' ';
            //    continue;
            //}
            a_writer << m_grid[x + y * m_maxX];
        }
        a_writer.Endl();
    }
}



} // dnd_game