#include <iostream>
using namespace std;

char board[3][3] =
{
    { '-', '-', '-' },
    { '-', '-', '-' },
    { '-', '-', '-' }
};

char emptyCell = '-';
int maxScore = 10;
int minScore = -10;
bool isPersonTurn;
char computerSymbol;
char personSymbol;
int invalidPosition = -1;
int movesAvailable = 9;

int maximizer(int a, int b, int depth);
int minimizer(int a, int b, int depth);

int evaluateBoard(int depth) 
{
    for (int row = 0; row < 3; row++)
    {
        if (board[row][0] == board[row][1] && board[row][1] == board[row][2])
        {
            if (board[row][0] == computerSymbol)
            {
                return maxScore - depth;
            }
            else if (board[row][0] == personSymbol)
            {
                return minScore + depth;
            }
        }
    }
    for (int col = 0; col < 3; col++)
    {
        if (board[0][col] == board[1][col] && board[1][col] == board[2][col])
        {
            if (board[0][col] == computerSymbol)
            {
                return maxScore - depth;
            }
            else if (board[0][col] == personSymbol)
            {
                return minScore + depth;
            }
        }
    }

    if (board[0][0] == board[1][1] && board[1][1] == board[2][2])
    {
        if (board[0][0] == computerSymbol)
        {
            return maxScore - depth;
        }
        else if (board[0][0] == personSymbol)
        {
            return minScore + depth;
        }
    }

    if (board[0][2] == board[1][1] && board[1][1] == board[2][0])
    {
        if (board[0][2] == computerSymbol)
        {
            return maxScore - depth;
        }
        else if (board[0][2] == personSymbol)
        {
            return minScore + depth;
        }
    }
    return 0;
}

void printBoard() {

    cout << "-------------" << endl;

    for (int i = 0; i < 3; i++) {
        cout << "| ";
        for (int j = 0; j < 3; j++) {            
            cout << board[i][j];
            cout << " | ";            
        }
        cout << endl << "-------------" << endl;
    }
}

bool areThereMovesLeft()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] == emptyCell)
            {
                return true;
            }
        }
    }       
    return false;
}

int maximizer(int a, int b, int depth)
{
    int curScore = evaluateBoard(depth);

    if (curScore != 0)
    {
        return curScore;
    }

    if (!areThereMovesLeft())
    {
        return 0;
    }

    int bestScore = INT_MIN;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] == emptyCell)
            {
                board[i][j] = computerSymbol;

                bestScore = max(bestScore, minimizer(a, b, depth + 1));

                board[i][j] = emptyCell;

                if (bestScore >= b)
                {
                    return bestScore;
                }
                a = max(a, bestScore);
            }
        }
    }
    return bestScore;    
}

int minimizer(int a, int b, int depth)
{
    int curScore = evaluateBoard(depth);

    if (curScore != 0)
    {
        return curScore;
    }

    if (!movesAvailable)
    {
        return 0;
    }

    int bestScore = INT_MAX;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] == emptyCell)
            {
                board[i][j] = personSymbol;

                bestScore = min(bestScore, maximizer(a, b, depth + 1));

                board[i][j] = emptyCell;

                if (bestScore <= a)
                {
                    return bestScore;
                }
                b = min(b, bestScore);
            }
        }
    }
    return bestScore;
}

pair<int, int> findBestTurnForComputer()
{
    int bestValue = INT_MIN;
    pair<int, int> bestNextTurn;

    bestNextTurn.first = invalidPosition;
    bestNextTurn.second = invalidPosition;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] == emptyCell)
            {
                board[i][j] = computerSymbol;

                int curTurnValue = minimizer(INT_MIN, INT_MAX, 0);

                board[i][j] = emptyCell;

                if (curTurnValue > bestValue)
                {
                    bestNextTurn.first = i;
                    bestNextTurn.second = j;
                    bestValue = curTurnValue;
                }
            }
        }
    }
    return bestNextTurn;
}

bool isThereWinner()
{
    if (evaluateBoard(0) != 0)
    {
        return true;
    }
    return false;
}

bool makeTurn(int i, int j, bool isPersonTurn)
{
    if (board[i][j] == emptyCell)
    {
        if (isPersonTurn)
        {
            board[i][j] = personSymbol;
        }
        else
        {
            board[i][j] = computerSymbol;
        }

        cout << "==========" << endl;
        movesAvailable--;
        printBoard();

        if (isThereWinner())
        {
            if (isPersonTurn)
            {
                cout << "Congratulations, you are the winner!" << endl;
            }
            else
            {
                cout << "Sorry, you lost!" << endl;
            }
        }

        return true;
    }
    
    cout << "This cell is not empty! Choose another." << endl;
    return false;
}

int main()
{
    printBoard();

    cout << "Do you want to start the game? Enter 1 for first and 0 for second : ";
    cin >> isPersonTurn;

    if (isPersonTurn)
    {
        computerSymbol = 'o';
        personSymbol = 'x';
    }
    else
    {
        computerSymbol = 'x';
        personSymbol = 'o';
    }

    while (movesAvailable > 0 && !isThereWinner())
    {
        int i, j;
        if (isPersonTurn)
        {
           do
           {
               cout << "Row:"; 
               cin >> i;
               cout << "Column:"; 
               cin >> j;
           } while (!makeTurn(i - 1, j - 1, isPersonTurn));
           isPersonTurn = !isPersonTurn;
           continue;
        }
        pair<int, int> bestTurn = findBestTurnForComputer();
        makeTurn(bestTurn.first, bestTurn.second, isPersonTurn);
        isPersonTurn = !isPersonTurn;
    }

    if (!isThereWinner() && !movesAvailable)
    {
        cout << "The game ends in an draw!";
    }
    
    return 0;
}