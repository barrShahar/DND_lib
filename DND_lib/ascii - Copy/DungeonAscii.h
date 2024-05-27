#pragma once
#pragma once
#include "ascii/Grid.h"
#include "GameParams.h"
namespace experis
{

static constexpr char VERTICAL_WALL_SYMBOL = '=';
static constexpr char HORIZONTAL_WALL_SYMBOL = '=';

// Dragon
static constexpr int DRAGON_HEIGHT = 13;
static constexpr int DRAGON_LENGTH = 32;
static char dragon[13][32] =
{
    "             __.-/|            ",
    "             \\`o_O'            ",
    "              =( )=  +--------+",
    "                U|   | DRAGON |",
    "      /\\  /\\   / |   +--------+",
    "     ) /^\\) ^\\/ _)\\     |      ",
    "     )   /^\\/   _) \\    |      ",
    "     )   _ /  / _)  \\___|_     ",
    " /\\  )/\\/ ||  | )_)\\___,|))    ",
    "<  >      |(,,) )__)    |      ",
    " ||      /    \\)___)\\          ",
    " | \\____(      )___) )____     ",
    "  \\______(_______;;;)__;;;)    "
};

class DungeonAscii
{
public:
    explicit DungeonAscii(bool a_up, bool a_right, bool a_down, bool a_left, bool a_isDragon, bool a_isTreasure);
    DungeonAscii(const DungeonAscii& DungeonAscii) = delete;
    DungeonAscii& operator=(const DungeonAscii& a_other) = delete;
    ~DungeonAscii() = default;

    void Draw(Grid& a_canvas, Direction a_direction, int a_baseX, int a_baseY) const;
private:
    void DrawPlayer(Grid& a_canvas, int a_baseX, int a_baseY, Direction a_direction) const;
    void DrawDragon(Grid & a_canvas, int a_baseX, int a_baseY, int a_dragonX, int a_dragonY) const;


    bool m_up;
    bool m_right;
    bool m_down;
    bool m_left;
    bool m_isDragon;
    bool m_isTreasure;
};
}  // namespace experis
