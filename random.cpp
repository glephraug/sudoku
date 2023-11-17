
#include <iostream>

#include "sudoku.h"


int main()
{
   Board board = GenerateSolvedBoard();
   std::cout << board << std::endl;
   return 0;
}
