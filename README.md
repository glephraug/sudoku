# Sudoku Solver

This project builds two executables, 'random' will generate a random solved sudoku board and print it on standard output. 'solve' reads a board from standard input and prints the solved board on standard output.

Boards are specified in ascii text as a 2D array of values. '1' to '9' represent given squares, and '.' is a placeholder for an unknown value. The 'tests' folder contains some example boards.

# But Why?

After years of solving relatively easy sudoku puzzles, I tried a few hard ones and learned that backtracking search is a necessary tool. There's no clever trick to avoid it. So, if mastering sudoku requires brute force, I'm going to be efficient about it.

Now that I've written the solver, I can finally walk away from any unfinished sudoku puzzle, confident that I could finish it if I really, really wanted to. I'm finally free!
