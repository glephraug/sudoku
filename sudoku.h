
#pragma once

#include <array>
#include <bitset>
#include <optional>


// A cell can possibly be some subset of the nine symbols. If a bit is set in the cell,
// it means we haven't ruled out the possibility of the cell having that value.
using Cell = std::bitset<9>;

// A board is a 9x9 array of cells.
using Board = std::array<Cell,81>;

// Print a board in a human readable format. '.' means the cell is unknown.
std::ostream & operator<<(std::ostream & out, const Board & board);

// Read the above format.
std::istream & operator>>(std::istream & in, Board & board);

// Create a random filled sudoku board.
Board GenerateSolvedBoard();

// Get the standard symbol for the mask, '1' to '9' for a single possible value, otherwise '.'.
char Symbol(Cell mask);

// Create the bitset matching the symbol. '.' returns the cell with every bit set.
Cell Mask(char symbol);

// Solve the board. Either returns the solution, or std::nullopt if unsolvable.
std::optional<Board> Solve(Board board);
