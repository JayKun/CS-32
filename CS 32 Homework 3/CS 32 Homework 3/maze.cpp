#include <string>
#include <iostream>
using namespace std;



bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	if (sr == er && sc == ec)
	{
		return true;
	}
	maze[sr][sc] = '0';
	if (maze[sr - 1][sc] == '.')// Checking North
		if (pathExists(maze, nRows, nCols, sr - 1, sc, er, ec))
			return true;
	if (maze[sr][sc + 1] == '.') //Checking East
		if (pathExists(maze, nRows, nCols, sr, sc + 1, er, ec))
			return true;
	if (maze[sr + 1][sc] == '.')//Checking South
		if (pathExists(maze, nRows, nCols, sr + 1, sc, er, ec))
			return true;
	if (maze[sr][sc - 1] == '.')// Checking West
		if (pathExists(maze, nRows, nCols, sr, sc - 1, er, ec))
			return true;
	return false; 
}
