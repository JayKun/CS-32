#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include "globals.h"
#include "Robot.h"
#include "Player.h"
#include "Arena.h"
#include "Game.h"
#include "History.h"

using namespace std; 
History::History(int nRows, int nCols)
{
	m_Rows = nRows;
	m_Cols = nCols;
	for (int k = 0; k < nRows; k++)
	{
		for (int i = 0; i < nCols; i++)
		{
			recordGrid[k][i] = '0';
		}
	}
}

bool History::record(int r, int c)
{
	if (r <= 0 || c <= 0 || r > MAXROWS || c > MAXCOLS)
	
		return false;
	else
	{
		recordGrid[r - 1][c - 1] ++; 
		return true;
	}
}

void History::display() const

{
	
	for (int k = 0; k < m_Rows; k++)
	{
		for (int i = 0; i < m_Cols; i++)
		{
			if (recordGrid[k][i] == '0')
				cout << '.';
			else
				cout << (char)('A'-1 + (recordGrid[k][i] - '0'))  ;
		}
		 cout<< endl; 
	}
	cout << endl;
}