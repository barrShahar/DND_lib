#pragma once
#include <memory>
#include <iostream>
#include "Writer.h"
#include "Grid.h"

namespace experis
{
class Grid
{
public:
    explicit Grid(size_t a_x, size_t a_y);
    Grid(const Grid& a_other) = delete;
    Grid& operator=(const Grid& a_other) = delete;
    ~Grid() = default;

    char& operator()(size_t x, size_t y);
    void Clean();
    void Print(Writer& a_os) const;
private:
    std::unique_ptr<char[]> m_grid;
    size_t m_maxX;
    size_t m_maxY;
};
}  // namespace experis

