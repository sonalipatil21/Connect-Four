/*****************************************************************************
 *  Author: Sonali PAtil
 ****************************************************************************/
#include <iostream>
#include <string>
#include <ctime>
#include <Windows.h>
using namespace std;

/*****************************************************************************
 *  Board Class Declaration
 ****************************************************************************/
class Board
{
private:
	char board [6][7];
public:
	Board();
	void getArray(char theArray[][7]);
	void printBoard();
	bool checkMove(int col);
	void makeMove(char player, int col);
	void undoMove(int col);
	bool isFull();
	void reset();
	bool hasWon(char player);
	void hostTournament();
};

/*****************************************************************************
 *  PlayerX Class Declaration and Definitions
 ****************************************************************************/
class PlayerX
{
public:
    PlayerX()          { m_PlayType = 'X'; }
    PlayerX(char type) { m_PlayType = type; }

    char GetType()          { return m_PlayType; }
    void SetType(char type) { m_PlayType = type; }

    int bestMove(int scores[])
    {
        int highestIndex = 0;
        int duplicateCount = 0;
        int dupIndexArray[7] = { 0 };

        for (int i = 0; i < 7; i++)
        if (scores[i] > scores[highestIndex])
            highestIndex = i;

        for (int i = 0; i < 7; i++)
        {
            if (scores[i] == scores[highestIndex])
            {
                dupIndexArray[duplicateCount] = i;
                duplicateCount++;
            }
        }
        if (duplicateCount != 0)
            return dupIndexArray[rand() % duplicateCount];

        return highestIndex;
    }

    int evalMove(int col, Board b)
    {
        Board brd = b;

        // Check if the column is full
        if (b.checkMove(col) == false)
            return -1;

        // Check if the columns is winnable
        brd.makeMove(m_PlayType, col);
        if (brd.hasWon(m_PlayType) == true)
        {
            return 100;
        }

        // Check if the other can be a winner
        char otherPlayType;
        brd = b;

        if (m_PlayType == 'X')
            otherPlayType = 'O';
        else
            otherPlayType = 'X';

        brd.makeMove(otherPlayType, col);
        if (brd.hasWon(otherPlayType) == true)
        {
            return 75;
        }
        return 50;
    }

    int evalAllMoves(Board b)
    {
        int scores[7];

        for (int i = 0; i < 7; i++)
        {
            scores[i] = evalMove(i, b);
        }
        return bestMove(scores);
    }

private:
    char m_PlayType;
};


/*****************************************************************************
 *  PlayerO Class Declaration and Definitions
 ****************************************************************************/



/*****************************************************************************
 *  Board Function Definitions
 ****************************************************************************/

/* Board()
 * Description: Initializes the connect 4 board to be empty (as denoted by 'e')
 */
Board::Board()
{
	// for every row and column pair fill in an 'e'
	for (int row = 0; row < 6; row++)
		for (int col = 0; col < 7; col++)
			board[row][col] = 'e';
}
/* getArray
 * Description: Fill the parameter theArray with the values currently stored in the
 *              game board.
 */
void Board::getArray(char theArray[][7])
{
	for (int row = 0; row < 6; row++)
		for (int col = 0; col < 7; col++)
			theArray[row][col] = board[row][col];
}
/* printBoard()
 * Description: Prints the board to the screen in a readable format
 */
void Board::printBoard()
{
	system("CLS");
	for (int row = 0; row < 6; row++)
	{
		for (int col = 0; col < 7; col++)
		{
			// for each cell, print a blank space if it's empty or the player who
			// occupies that cell
			if (board[row][col] == 'e')
				cout << "|   ";
			else
				cout << "| " << board[row][col] << " ";
		}
		// break line at end of each row
		cout << "|\n";
	}
	// print column numbers at bottom
	cout << "-----------------------------\n  0   1   2   3   4   5   6\n\n";
}
/* checkMove()
 * Description: Tests to see if the proposed move is legal
 */
bool Board::checkMove(int col)
{
	// if outside of legal column values, return false
	if (col < 0 || col > 6)
		return false;
	// if column is full, return false
	if (board[0][col] != 'e')
		return false;
	// otherwise, it's ok
	return true;
}
/* makeMove()
 * Description: If the move is legal, updates the game board to reflect the
 *              move the player is making.
 */
void Board::makeMove(char player, int col)
{
	// Test for legality
	if (checkMove(col))
	{
		// Find lowest empty row in specified column
		for (int i = 5; i >= 0; i--)
			if (board[i][col] == 'e')
			{
				// Fill in player's move
				board[i][col] = player;
				return;
			}
	}
}
/* undoMove
 * Description: Removes the last token placed in the specified column
 */
void Board::undoMove(int col)
{
	for (int row = 0; row < 6; row++)
	{
		if (board[row][col] != 'e')
		{
			board[row][col] = 'e';
			return;
		}
	}
}
/* isFull()
 * Description: Tests to see if board is completely full by checking top of each column
 */
bool Board::isFull()
{
	bool full = true;
	// since fill from bottom (row 5), just test row 0 for each column
	for (int col = 0; col < 7; col++)
		if (board[0][col] == 'e')
			full = false;
	return full;
}
/* reset()
 * Description: Resets the board to be full of e's
 */
void Board::reset()
{
	// for every row and column pair fill in an 'e'
	for (int row = 0; row < 6; row++)
		for (int col = 0; col < 7; col++)
			board[row][col] = 'e';
}
/* hasWon
 * Description: Tests to see if the specified player has won the game by checking all
 *              4-in-a-row, 4-in-a-column and 4-on-a-diagonal possibilities.
 */
bool Board::hasWon(char player)
{
	// test for 4 in a row
	for (int row = 0; row < 6; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			if (board[row][col] == player &&
				board[row][col+1] == player &&
				board[row][col+2] == player &&
				board[row][col+3] == player)
				return true;
		}
	}
	// test for 4 in a column
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 7; col++)
		{
			if (board[row][col] == player &&
				board[row+1][col] == player &&
				board[row+2][col] == player &&
				board[row+3][col] == player)
				return true;
		}
	}
	// test for up-right diagonals
	for (int row = 5; row > 2; row--)
	{
		for (int col = 0; col < 4; col++)
		{
			if (board[row][col] == player &&
				board[row-1][col+1] == player &&
				board[row-2][col+2] == player &&
				board[row-3][col+3] == player)
				return true;
		}
	}
	// test for up-left diagonals
	for (int row = 5; row > 2; row--)
	{
		for (int col = 6; col > 2; col--)
		{
			if (board[row][col] == player &&
				board[row-1][col-1] == player &&
				board[row-2][col-2] == player &&
				board[row-3][col-3] == player)
				return true;
		}
	}

	// else, hasn't won
	return false;
}
/* hostTournament
 * Description: hosts 2 games between two computer Player objects: PlayerX and PlayerO.
 *              PlayerX moves first in the first round and PlayerO moves first in the
 *              second round.
 */
void Board::hostTournament()
{
	// Create two player objects
	PlayerX px('X');
	PlayerX po('O');

	// Round 1: X plays first
	// initialize game board and who plays X and O
	bool gameOver = false;
	int move = -1;
	// announce round and pause for user to acknowledge
	cout << "Here's round 1: PlayerX goes first!\n";
	system("PAUSE");
	printBoard();
	Sleep(500);
	do
	{
		// Player X makes a move.
		move = px.evalAllMoves(*this);
		makeMove('X',move);
		printBoard();
		Sleep(500);
		// check to see if it was a winning move
		if (hasWon('X'))
		{
			cout << "Congratulations PlayerX, you have won!!\n\n";
			gameOver = true;
		}
		// check to see if move resulted in a stalemate
		else if (isFull())
		{
			cout << "A Stalemate, too bad!\n\n";
			gameOver = true;
		}
		else
		{
			// Player O makes a move
			move = po.evalAllMoves(*this);
			makeMove('O',move);
			printBoard();
			Sleep(500);
			// check to see if it was the winning move
			if (hasWon('O'))
			{
				cout << "Congratulations PlayerO, you have won!!\n\n";
				gameOver = true;
			}
			// check to see if move resulted in a statemate
			else if (isFull())
			{
				cout << "A Stalemate, too bad!\n\n";
				gameOver = true;
			}
		}		
	// continue until either one player wins or the board fills up
	}while (!gameOver);

	// Round 2: O plays first
	// reset game board 
	reset();
	gameOver = false;
	// announce second round and pause for player to acknowledge
	cout << "Here's round 2: PlayerO goes first!\n";
	system("PAUSE");
	printBoard();
	Sleep(500);
	do
	{
		// Player O makes a move.
		move = po.evalAllMoves(*this);
		makeMove('O',move);
		printBoard();
		Sleep(500);
		// check to see if it is the winning move
		if (hasWon('O'))
		{
			cout << "Congratulations PlayerO, you have won!!\n\n";
			gameOver = true;
		}
		// check for stalemate
		else if (isFull())
		{
			cout << "A Stalemate, too bad!\n\n";
			gameOver = true;
		}
		else
		{
			// Player X makes a move.
			move = px.evalAllMoves(*this);
			makeMove('X',move);
			printBoard();
			Sleep(500);
			// check for a win
			if (hasWon('X'))
			{
				cout << "Congratulations PlayerX, you have won!!\n\n";
				gameOver = true;
			}
			// check for statemate
			else if (isFull())
			{
				cout << "A Stalemate, too bad!\n\n";
				gameOver = true;
			}
		}
			
	}while (!gameOver);
}

/*****************************************************************************
 *  main 
 ****************************************************************************/

int main ()
{
	// in case anyone's using random numbers
	srand((unsigned)time(0));
	// create game board
	Board b;
	// start tournament
	b.hostTournament();
	system("PAUSE");
}
