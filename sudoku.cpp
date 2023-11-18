
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


Cell Mask(char symbol)
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

   // We return a permutation on the below board.
   std::array<char,81> cells{
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

      if(i%2){ // Swap rows
         for(int l = 0; l < 9; ++l)
         {
            std::swap(cells[j*9 + l], cells[k*9 + l]);
         }
      }else{ // Swap columns
         for(int l = 0; l < 9; ++l)
         {
            std::swap(cells[j + l*9], cells[k + l*9]);
         }
      }
   }

   // Convert to board format
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
         return in; // Fail bit is already set.
      }
      if(c == '.' || (c >= '1' && c <= '9')){
         board[i] = Mask(c);
         ++i;
      }
   }
   return in;
}


// For each single-valued cell, remove that value from the set of possibilities
// for all neighbors. This greatly reduces the search space and also tells us
// when the board is unsolvable if a cell has zero possibilities.
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
   // First reduce the board so we throw out searches doomed to failure.
   std::optional<Board> result = Reduce(board);
   if(!result){
      // Reduce failed, so there is no solution.
      return std::nullopt;
   }
   board = *result;

   // Find an unknown cell and solve it
   for(int i = 0; i < 81; ++i){
      Cell c = board[i];
      if(c.count() == 0){
         // There is a bad cell in our board. This should be caught by Reduce().
         return std::nullopt;
      }else if(c.count() > 1){
         for(int j = 0; j < 9; ++j){
            if(c.test(j)){ // Only try possible values
               Board test = board;
               test[i] = Cell(1) << j;

               // See if there is a solution using this value at this cell.
               std::optional<Board> result = Solve(test);
               if(result) return result; // We found a solution
            }
         }
         return std::nullopt; // There were no solutions
      }
   }
   return board; // The board is already solved
}
