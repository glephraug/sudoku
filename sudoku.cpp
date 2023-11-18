
#include "sudoku.h"

#include <random>
#include <iostream>
#include <queue>


char Symbol(Cell mask)
{
   if(mask.count() == 1){
      char c = '0';
      while(mask.any()){
         mask >>= 1;
         ++c;
      }
      return c;
   }else{
      return '.';
   }
}


std::bitset<9> Mask(char symbol)
{
   if(symbol == '.'){
      return std::bitset<9>("111111111");
   }else if(symbol >= '1' && symbol <= '9'){
      return std::bitset<9>("1") << (symbol - '1');
   }else{
      throw std::runtime_error("Mask: invalid character");
   }
}


Board GenerateSolvedBoard()
{
   std::random_device r;
   std::default_random_engine gen(r());
   std::uniform_int_distribution<int> d(0,8);

   std::array<std::uint8_t,81> cells{
      0, 1, 2,  3, 4, 5,  6, 7, 8,
      3, 4, 5,  6, 7, 8,  0, 1, 2,
      6, 7, 8,  0, 1, 2,  3, 4, 5,

      1, 2, 3,  4, 5, 6,  7, 8, 0,
      4, 5, 6,  7, 8, 0,  1, 2, 3,
      7, 8, 0,  1, 2, 3,  4, 5, 6,

      2, 3, 4,  5, 6, 7,  8, 0, 1,
      5, 6, 7,  8, 0, 1,  2, 3, 4,
      8, 0, 1,  2, 3, 4,  5, 6, 7
   };

   for(int i = 0; i < 1000; ++i)
   {
      int j = d(gen);
      int k = (j/3)*3 + d(gen)%3;

      //std::cout << "swap " << j << " & " << k << std::endl;

      if(i%2){
         for(int l = 0; l < 9; ++l)
         {
            std::swap(cells[j*9 + l], cells[k*9 + l]);
         }
      }else{
         for(int l = 0; l < 9; ++l)
         {
            std::swap(cells[j + l*9], cells[k + l*9]);
         }
      }
   }

   Board board;
   for(int i = 0; i < 81; ++i) board[i] = Mask(cells[i]+'1');
   return board;
}


std::ostream & operator<<(std::ostream & out, const Board & board)
{
   for(int i = 0; i < 81; ++i){
      if(i > 0){
         if(i%9 == 0){
            out << std::endl;
            if(i%27 == 0) out << std::endl;
         }else if(i %3 == 0){
            out << " ";
         }
      }
      out << Symbol(board[i]);
   }
   return out;
}


std::istream & operator>>(std::istream & in, Board & board)
{
   int i = 0;
   while(i < 81){
      int c = in.get();
      if(in.fail()){
         throw std::runtime_error("Board operator>>: stream ended before full board read");
      }
      if(c == '.' || (c >= '1' && c <= '9')){
         board[i] = Mask(c);
         ++i;
      }
   }
   return in;
}


std::optional<Board> Reduce(Board board)
{
   std::queue<int> next;
   for(int i = 0; i < 81; ++i){
      Cell c = board[i];
      if(c.count() == 1) next.push(i);
   }

   while(!next.empty())
   {
      int i = next.front();
      next.pop();
      Cell c = board[i];
      if(c.none()) return std::nullopt; // cell emptied while in queue

      //std::cout << i << " mask " << c.to_string() << std::endl;
           
      // row
      for(int j = 0; j < 9; ++j){
         int l = (i/9)*9 + j;
         if(l != i && (c&board[l]).any()){
            board[l] &= ~c;
            if(board[l].none()) return std::nullopt;
            if(board[l].count() == 1) next.push(l);
         }
      }

      // column
      for(int j = 0; j < 9; ++j){
         int l = i%9 + j*9;
         if(l != i && (c&board[l]).any()){
            board[l] &= ~c;
            if(board[l].none()) return std::nullopt;
            if(board[l].count() == 1) next.push(l);
         }
      }

      // square
      for(int j = 0; j < 9; ++j){
         int l = (i/27)*27 + ((i%9)/3)*3 + (j/3)*9 + j%3;
         if(l != i && (c&board[l]).any()){
            board[l] &= ~c;
            if(board[l].none()) return std::nullopt;
            if(board[l].count() == 1) next.push(l);
         }
      }
   }
   return board;
}


std::optional<Board> Solve(Board board)
{
   std::optional<Board> result = Reduce(board);
   if(!result){
      std::cout << "Reduce failed" << std::endl;
      return std::nullopt;
   }
   board = *result;

   // Find an unknown cell and solve it
   for(int i = 0; i < 81; ++i){
      Cell c = board[i];
      if(c.count() == 0){
         return std::nullopt;
      }else if(c.count() > 1){
         for(int j = 0; j < 9; ++j){
            if(c.test(j)){
               Board test = board;
               test[i] = Cell(1) << j;

              // std::cout << "test" << std::endl << test << std::endl;

               std::optional<Board> result = Solve(test);
               if(result) return result; // We found a solution
               //std::cout << "failed" << std::endl;
            }
         }
         //std::cout << "found nothing" << std::endl;
         return std::nullopt; // There were no solutions
      }
   }
   return board; // The board is already solved
}
