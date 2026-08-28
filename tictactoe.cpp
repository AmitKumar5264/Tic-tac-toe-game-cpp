#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

const char EMPTY = ' ';
const char PLAYER_X = 'X';
const char PLAYER_O = 'O';

// ---------- Board setup & display ----------

void initializeBoard(char board[3][3])
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = EMPTY;
}

void displayBoard(char board[3][3])
{
    cout << "\n";
    for (int i = 0; i < 3; i++)
    {
        cout << " " << board[i][0] << " | " << board[i][1] << " | " << board[i][2] << " \n";
        if (i < 2)
            cout << "---|---|---\n";
    }
    cout << "\n";
}

// ---------- Game logic ----------

bool isBoardFull(char board[3][3])
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == EMPTY)
                return false;
    return true;
}

bool checkWin(char board[3][3], char player)
{
    // Rows and columns
    for (int i = 0; i < 3; i++)
    {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player)
            return true;
        if (board[0][i] == player && board[1][i] == player && board[2][i] == player)
            return true;
    }
    // Diagonals
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player)
        return true;
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player)
        return true;

    return false;
}

bool isValidMove(char board[3][3], int row, int col)
{
    if (row < 0 || row > 2 || col < 0 || col > 2)
        return false;
    return board[row][col] == EMPTY;
}

// Ask a human player for row/col until a valid move is entered
void humanMove(char board[3][3], char player)
{
    int row, col;
    while (true)
    {
        cout << "Player " << player << ", enter your move (row and column, 1-3 each, e.g. 2 3): ";
        if (!(cin >> row >> col))
        {
            if (cin.eof())
            {
                cout << "\nNo more input available. Exiting.\n";
                exit(0);
            }
            // Non-numeric input, clear the error state
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Please enter two numbers.\n";
            continue;
        }
        row -= 1; // convert 1-3 to 0-2
        col -= 1;

        if (isValidMove(board, row, col))
        {
            board[row][col] = player;
            break;
        }
        else
        {
            cout << "That cell is invalid or already taken. Try again.\n";
        }
    }
}

// Very simple computer AI:
// 1) Win if possible, 2) Block opponent's win if possible, 3) Otherwise random empty cell
void computerMove(char board[3][3], char computer, char opponent)
{
    // Try to win
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] == EMPTY)
            {
                board[i][j] = computer;
                if (checkWin(board, computer))
                {
                    cout << "Computer plays " << (i + 1) << " " << (j + 1) << "\n";
                    return;
                }
                board[i][j] = EMPTY;
            }
        }
    }

    // Try to block opponent
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] == EMPTY)
            {
                board[i][j] = opponent;
                bool blocks = checkWin(board, opponent);
                board[i][j] = EMPTY;
                if (blocks)
                {
                    board[i][j] = computer;
                    cout << "Computer plays " << (i + 1) << " " << (j + 1) << "\n";
                    return;
                }
            }
        }
    }

    // Otherwise pick a random empty cell
    while (true)
    {
        int row = rand() % 3;
        int col = rand() % 3;
        if (board[row][col] == EMPTY)
        {
            board[row][col] = computer;
            cout << "Computer plays " << (row + 1) << " " << (col + 1) << "\n";
            return;
        }
    }
}

// ---------- Game modes ----------

void playTwoPlayerGame()
{
    char board[3][3];
    initializeBoard(board);

    char currentPlayer = PLAYER_X;
    bool gameOver = false;

    displayBoard(board);

    while (!gameOver)
    {
        humanMove(board, currentPlayer);
        displayBoard(board);

        if (checkWin(board, currentPlayer))
        {
            cout << "Player " << currentPlayer << " wins!\n";
            gameOver = true;
        }
        else if (isBoardFull(board))
        {
            cout << "It's a draw!\n";
            gameOver = true;
        }
        else
        {
            currentPlayer = (currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
        }
    }
}

void playVsComputerGame()
{
    char board[3][3];
    initializeBoard(board);

    char human = PLAYER_X;
    char computer = PLAYER_O;
    char currentPlayer = PLAYER_X; // human goes first
    bool gameOver = false;

    displayBoard(board);

    while (!gameOver)
    {
        if (currentPlayer == human)
            humanMove(board, human);
        else
            computerMove(board, computer, human);

        displayBoard(board);

        if (checkWin(board, currentPlayer))
        {
            if (currentPlayer == human)
                cout << "You win!\n";
            else
                cout << "Computer wins! Better luck next time.\n";
            gameOver = true;
        }
        else if (isBoardFull(board))
        {
            cout << "It's a draw!\n";
            gameOver = true;
        }
        else
        {
            currentPlayer = (currentPlayer == human) ? computer : human;
        }
    }
}

// ---------- Main ----------

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    char playAgain = 'y';

    while (playAgain == 'y' || playAgain == 'Y')
    {
        cout << "=============================\n";
        cout << "        TIC TAC TOE\n";
        cout << "=============================\n";
        cout << "1. Two Player (X vs O)\n";
        cout << "2. Player vs Computer\n";
        cout << "Choose a mode (1 or 2): ";

        int mode;
        while (!(cin >> mode) || (mode != 1 && mode != 2))
        {
            if (cin.eof())
            {
                cout << "\nNo more input available. Exiting.\n";
                return 0;
            }
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid choice. Enter 1 or 2: ";
        }

        if (mode == 1)
            playTwoPlayerGame();
        else
            playVsComputerGame();

        cout << "Play again? (y/n): ";
        cin >> playAgain;
    }

    cout << "Thanks for playing!\n";
    return 0;
}