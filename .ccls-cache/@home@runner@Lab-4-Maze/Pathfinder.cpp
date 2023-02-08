// bash check.sh

#include "Pathfinder.h"
	string Pathfinder::toString() const
	{
	    stringstream ss;
	    for(int row = 0; row < ROW_SIZE; row++) {
				for(int col = 0; col < COL_SIZE; col++) {
					for(int lev = 0; lev < LEV_SIZE; lev++) {
						ss << maze_grid[row][col][lev];
						if (lev < 4) {
							ss << " ";
						}
					}
				ss << endl;
				}
			if (row < 4) {
				ss << endl;
			}
		}
	  return ss.str();
	}

	/*
	* createRandomMaze
	*
	* Generates a random maze and stores it as the current maze.
	*
	* The generated maze must contain a roughly equal number of 1s and 0s and must have a 1
	* in the entrance cell (0, 0, 0) and in the exit cell (4, 4, 4).  The generated maze may be
	* solvable or unsolvable, and this method should be able to produce both kinds of mazes.
	*/
	void Pathfinder::createRandomMaze()
	{
	    for(int row = 0; row < ROW_SIZE; row++) {
				for(int col = 0; col < COL_SIZE; col++) {
					for(int lev = 0; lev < LEV_SIZE; lev++) {
						maze_grid[row][col][lev] = rand()%2;
					}
				}
	    }
			// same as when it was initialized, but instead of assigning it 0s and 1s, assign it a random 
			// 1 or 0, and make sure 0,0,0,0 and 4,4,4,4 are 1s
		maze_grid[0][0][0] = 1;
		maze_grid[ROW_SIZE - 1][COL_SIZE - 1][LEV_SIZE - 1] = 1;
	}
	//-----------------------------------------------------------------------------------------

	//Part 2-----------------------------------------------------------------------------------
	/*
	* importMaze
	*
	* Reads in a maze from a file with the given file name and stores it as the current maze.
	* Does nothing if the file does not exist or if the file's data does not represent a valid
	* maze.
	*
	* The file's contents must be of the format described above to be considered valid.
	*
	* Parameter:	file_name
	*				The name of the file containing a maze
	* Returns:		bool
	*				True if the maze is imported correctly; false otherwise
	*/
	bool Pathfinder::importMaze(string file_name)
	{
		cout << "importMaze from " << file_name << endl;
		int maze_grid_copy[ROW_SIZE][COL_SIZE][LEV_SIZE];
		int find_maze_grid[ROW_SIZE][COL_SIZE][LEV_SIZE];

		ifstream file (file_name.c_str());
		string temp;
		if (file.is_open()) {
			string line;
			int row_count = 0;
			for(int x = 0; x < ROW_SIZE; x++) {
				for (int y = 0; y < COL_SIZE; y++) {
					if (!(getline (file, line))) {
						return false;
					}
					stringstream ss(line);
					for (int z = 0; z < LEV_SIZE; z++) {
						int value; 
						if (!(ss >> value)) {
							return false;
						}
						if (!(value == 0 || value == 1)) {
							return false;
						}
						maze_grid_copy[x][y][z] = value;
						find_maze_grid[x][y][z] = value;
						//cout << value << " ";
					}
					//cout << endl;
					if(ss >> temp) {
						return false;
					}
				}
				getline(file, line);
			}
			if (getline(file, line)) {
				return false;
			}
		}
		else {
			return false;
		}
		
		if (maze_grid_copy[0][0][0] == 0 || maze_grid_copy[ROW_SIZE-1][COL_SIZE-1][LEV_SIZE-1] == 0) {
			return false;
		}
		for (int x = 0; x < ROW_SIZE; x++) {
			for (int y = 0; y < COL_SIZE; y++) {
				for (int z = 0; z < LEV_SIZE; z++) {
					maze_grid[x][y][z] = maze_grid_copy[x][y][z];
					cout << maze_grid[x][y][z] << " ";
				}
				cout << endl;
			}
		}

		// cout << toString();
		return true;
	}	
	
	bool Pathfinder::find_maze_path(int grid[ROW_SIZE][COL_SIZE][LEV_SIZE], int r, int c, int l) {
	  // cout << "find_maze_path ["<<r<<"]["<<c<<"]["<<l<<"]"<<endl;
	  // cout << this->toString();
	  if (r < 0 || c < 0 || l < 0 || r >= ROW_SIZE || c >= COL_SIZE || l >= LEV_SIZE)
	    return false;      // Cell is out of bounds.
	  else if (grid[r][c][l] != BACKGROUND)
	    return false;      // Cell is on barrier or dead end.
	  else if (r == ROW_SIZE - 1 && c == COL_SIZE - 1 && l == LEV_SIZE - 1) {
	    grid[r][c][l] = PATH;         // Cell is on path
	    solution.push_back("("+to_string(l)+", "+to_string(c)+", "+to_string(r)+")");
	    return true;               // and is maze exit.
	  }
	  else { 
	    // Recursive case.
	    // Attempt to find a path from each neighbor.
	    // Tentatively mark cell as on path.
	    grid[r][c][l] = PATH;

			if (	 find_maze_path(grid, r - 1, c, l)
	        || find_maze_path(grid, r + 1, c, l)
	        || find_maze_path(grid, r, c - 1, l)
	        || find_maze_path(grid, r, c + 1, l) 
	        || find_maze_path(grid, r, c, l - 1) 
	        || find_maze_path(grid, r, c, l + 1) 
				)	{

	      solution.push_back("("+to_string(l)+", "+to_string(c)+", "+to_string(r)+")");
	      return true;
	    }
	    else {
	      grid[r][c][l] = TEMPORARY;  // Dead end.
	      return false;
	    }
	  }
	}
	  //Part 3-----------------------------------------------------------------------------------
	/*
	* solveMaze
	*
	* Attempts to solve the current maze and returns a solution if one is found.
	*
	* A solution to a maze is a list of coordinates for the path from the entrance to the exit
	* (or an empty vector if no solution is found). This list cannot contain duplicates, and
	* any two consecutive coordinates in the list can only differ by 1 for only one
	* coordinate. The entrance cell (0, 0, 0) and the exit cell (4, 4, 4) should be included
	* in the solution. Each string in the solution vector must be of the format "(x, y, z)",
	* where x, y, and z are the integer coordinates of a cell.
	*
	* Understand that most mazes will contain multiple solutions
	*
	* Returns:		vector<string>
	*				A solution to the current maze, or an empty vector if none exists
	*/
	
	vector<string> Pathfinder::solveMaze()
	{
		solution.clear();
		solution_check.clear();

		int maze_grid_copy[ROW_SIZE][COL_SIZE][LEV_SIZE];
		for(int x = 0; x < ROW_SIZE; x++) {
			for(int y = 0; y < ROW_SIZE; y++) {
				for(int z = 0; z < ROW_SIZE; z++) {
					maze_grid_copy[x][y][z] = maze_grid[x][y][z];
					}
				}
			}

		find_maze_path (maze_grid_copy, 0, 0, 0);

		reverse(solution.begin(), solution.end()); 
		reverse(solution_check.begin(), solution_check.end());
		for(int i = 0; i < solution_check.size(); i++) {
			cout << solution_check.at(i) << endl;
		}
	    return solution;
	}
