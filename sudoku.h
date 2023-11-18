
#pragma once

#include <cstdint>
#include <array>
#include <bitset>
#include <optional>


using Cell = std::bitset<9>;
using Board = std::array<Cell,81>;

std::ostream & operator<<(std::ostream & out, const Board & board);
std::istream & operator>>(std::istream & in, Board & board);

Board GenerateSolvedBoard();

char Symbol(Cell mask);

std::bitset<9> Mask(char symbol);

std::optional<Board> Solve(Board board);
