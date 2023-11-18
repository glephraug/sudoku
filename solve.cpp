
#include <iostream>
#include <fstream>

#include "sudoku.h"


int main()
{
   Board board;
   std::cin >> board;

   std::optional<Board> result = Solve(board);

   if(result){
      std::cout << *result << std::endl;
   }else{
      std::cout << "No solution found" << std::endl;
   }

   return 0;
}
