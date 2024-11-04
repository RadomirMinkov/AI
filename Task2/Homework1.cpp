#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <chrono>
using namespace std;

const int GOAL_REACHED = -10;

vector<int> boardTiles;
vector<string> moveSequence;
int boardDimension;
int targetZeroPosition;
int currentZeroPosition;

int calculateInversions(const vector<int> &board)
{
    int inversionCount = 0;
    int totalTiles = board.size();

    for (int i = 0; i < totalTiles; ++i)
    {
        if (board[i] == 0)
            continue;
        for (int j = i + 1; j < totalTiles; ++j)
        {
            if (board[j] != 0 && board[i] > board[j])
            {
                inversionCount++;
            }
        }
    }
    return inversionCount;
}

int locateBlankRow(const vector<int> &board, int size)
{
    int blankPosition = 0;
    for (int i = 0; i < board.size(); ++i)
    {
        if (board[i] == 0)
        {
            blankPosition = i;
            break;
        }
    }
    return blankPosition / size;
}

bool isBoardSolvable()
{
    int dimension = static_cast<int>(sqrt(boardTiles.size()));
    int inversions = calculateInversions(boardTiles);
    int blankRow = locateBlankRow(boardTiles, dimension);

    if (dimension % 2 != 0)
    {
        return inversions % 2 == 0;
    }
    else
    {
        return (inversions + blankRow) % 2 != 0;
    }
}

int calculateManhattanDistance()
{
    int distanceSum = 0;
    int tilesBeforeZero = targetZeroPosition;
    int currentTile;
    int position;

    for (int i = 0; i < boardDimension; i++)
    {
        for (int j = 0; j < boardDimension; j++)
        {
            position = j + i * boardDimension;
            currentTile = boardTiles[position];
            if (currentTile == 0)
                continue;

            if (tilesBeforeZero > 0)
            {
                distanceSum += abs((currentTile - 1) / boardDimension - i) + abs((currentTile - 1) % boardDimension - j);
                --tilesBeforeZero;
            }
            else
            {
                distanceSum += abs(currentTile / boardDimension - i) + abs(currentTile % boardDimension - j);
            }
        }
    }
    return distanceSum;
}

bool isGoalState()
{
    return calculateManhattanDistance() == 0;
}

void swapPositions(int posA, int posB)
{
    swap(boardTiles[posA], boardTiles[posB]);
}

bool shiftUp()
{
    if (currentZeroPosition / boardDimension == boardDimension - 1)
    {
        return false;
    }
    swapPositions(currentZeroPosition, currentZeroPosition + boardDimension);
    currentZeroPosition = currentZeroPosition + boardDimension;
    return true;
}

bool shiftDown()
{
    if (currentZeroPosition / boardDimension == 0)
    {
        return false;
    }
    swapPositions(currentZeroPosition, currentZeroPosition - boardDimension);
    currentZeroPosition = currentZeroPosition - boardDimension;
    return true;
}

bool shiftRight()
{
    if (currentZeroPosition % boardDimension == 0)
    {
        return false;
    }
    swapPositions(currentZeroPosition, currentZeroPosition - 1);
    currentZeroPosition = currentZeroPosition - 1;
    return true;
}

bool shiftLeft()
{
    if (currentZeroPosition % boardDimension == boardDimension - 1)
    {
        return false;
    }
    swapPositions(currentZeroPosition, currentZeroPosition + 1);
    currentZeroPosition = currentZeroPosition + 1;
    return true;
}

int findZeroPosition()
{
    for (int i = 0; i < boardDimension * boardDimension; i++)
    {
        if (boardTiles[i] == 0)
        {
            return i;
        }
    }
    return -1;
}

int depthFirstSearch(int g, int limit)
{
    int estimatedCost = g + calculateManhattanDistance();

    if (estimatedCost > limit)
    {
        return estimatedCost;
    }

    if (isGoalState())
    {
        cout << moveSequence.size() - 1 << endl;
        for (int i = 1; i < moveSequence.size(); i++)
        {
            cout << moveSequence[i] << endl;
        }
        return GOAL_REACHED;
    }

    int minimumThreshold = INT_MAX;
    int threshold;

    if (moveSequence.back() != "up" && shiftDown())
    {
        moveSequence.push_back("down");
        threshold = depthFirstSearch(g + 1, limit);
        if (threshold == GOAL_REACHED)
            return GOAL_REACHED;
        if (threshold < minimumThreshold)
            minimumThreshold = threshold;
        moveSequence.pop_back();
        shiftUp();
    }
    if (moveSequence.back() != "down" && shiftUp())
    {
        moveSequence.push_back("up");
        threshold = depthFirstSearch(g + 1, limit);
        if (threshold == GOAL_REACHED)
            return GOAL_REACHED;
        if (threshold < minimumThreshold)
            minimumThreshold = threshold;
        moveSequence.pop_back();
        shiftDown();
    }
    if (moveSequence.back() != "left" && shiftRight())
    {
        moveSequence.push_back("right");
        threshold = depthFirstSearch(g + 1, limit);
        if (threshold == GOAL_REACHED)
            return GOAL_REACHED;
        if (threshold < minimumThreshold)
            minimumThreshold = threshold;
        moveSequence.pop_back();
        shiftLeft();
    }
    if (moveSequence.back() != "right" && shiftLeft())
    {
        moveSequence.push_back("left");
        threshold = depthFirstSearch(g + 1, limit);
        if (threshold == GOAL_REACHED)
            return GOAL_REACHED;
        if (threshold < minimumThreshold)
            minimumThreshold = threshold;
        moveSequence.pop_back();
        shiftRight();
    }
    return minimumThreshold;
}

void idaStarAlgorithm()
{
    int threshold = calculateManhattanDistance();
    moveSequence.push_back("start");
    int nextThreshold;

    while (true)
    {
        nextThreshold = depthFirstSearch(0, threshold);
        if (nextThreshold == GOAL_REACHED)
            break;
        threshold = nextThreshold;
    }
}

int main()
{
    int totalTiles, targetZeroPos;
    cin >> totalTiles >> targetZeroPos;
    int tileValue;

    for (int i = 0; i <= totalTiles; i++)
    {
        cin >> tileValue;
        boardTiles.push_back(tileValue);
    }

    if (isBoardSolvable())
    {
        boardDimension = sqrt(totalTiles + 1);

        if (targetZeroPos == -1)
        {
            targetZeroPosition = boardDimension * boardDimension - 1;
        }
        else
        {
            targetZeroPosition = targetZeroPos;
        }

        currentZeroPosition = findZeroPosition();

        //cout << "Starting the clock!" << std::endl;
        //auto start = std::chrono::high_resolution_clock::now();
        idaStarAlgorithm();
        //auto stop = std::chrono::high_resolution_clock::now();
        //cout << "Stopped the clock!" << std::endl;

//        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

//        cout << "The algorithm time is: " << duration.count()/ 1000 << " microseconds!" << std::endl;
    }
    else
    {
        cout << -1;
    }
    return 0;
}