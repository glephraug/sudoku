
#pragma once

#include <cstdint>
#include <array>
#include <bitset>

class Board
{
public:

   using Cell = std::bitset<9>;

   Board();

   void set(int i, Cell c);

private:
   std::array<std::bitset<9>,81> cells;

   friend std::ostream & operator<<(std::ostream & out, const Board & board);
   friend std::istream & operator>>(std::istream & in, Board & board);
};


Board GenerateSolvedBoard();

char Symbol(std::bitset<9> mask);
std::bitset<9> Mask(char symbol);
