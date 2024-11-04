#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <tgmath.h>
using namespace std;

//any negative integer would work here since value of f is always positive
const int FOUND = -10;

vector<int> tiles;
vector<string> path;
int sizeOfBoard;
int goalIndexOfZero;
int curIndexOfZero;

int manhattan()
{
    int sum = 0;
    int positionsBeforeZeroLeft = goalIndexOfZero;
    int cur;
    int x;
    for (int i = 0; i < sizeOfBoard; i++)
    {
        for (int j = 0; j < sizeOfBoard; j++)
        {
            x = j + i * sizeOfBoard;
            cur = tiles[x];
            if (cur == 0)
            {
                continue;
            }

            if (positionsBeforeZeroLeft > 0) {
                sum += abs((cur - 1) / sizeOfBoard - i) + abs((cur - 1) % sizeOfBoard - j);
                --positionsBeforeZeroLeft;
            }
            else {
                sum += abs(cur / sizeOfBoard - i) + abs(cur % sizeOfBoard - j);
            }
        }
    }
    return sum;
}

// is this board the goal board?
bool isGoal()
{
    return manhattan() == 0;
}

void swapTiles(int posA, int posB) {
    swap(tiles[posA], tiles[posB]);
}

bool moveUp()
{
    //if zero is in the last row
    if (curIndexOfZero / sizeOfBoard == sizeOfBoard - 1)
    {
        return false;
    }
    swapTiles(curIndexOfZero, curIndexOfZero + sizeOfBoard);
    curIndexOfZero = curIndexOfZero + sizeOfBoard;
    return true;
}

bool moveDown()
{
    //if zero is in the first row
    if (curIndexOfZero / sizeOfBoard == 0)
    {
        return false;
    }
    swapTiles(curIndexOfZero, curIndexOfZero - sizeOfBoard);
    curIndexOfZero = curIndexOfZero - sizeOfBoard;
    return true;
}

bool moveRight()
{
    //if zero is in the left most column
    if (curIndexOfZero % sizeOfBoard == 0)
    {
        return false;
    }
    swapTiles(curIndexOfZero, curIndexOfZero - 1);
    curIndexOfZero = curIndexOfZero - 1;
    return true;
}

bool moveLeft()
{
    //if zero is in the right most column
    if (curIndexOfZero % sizeOfBoard == sizeOfBoard - 1)
    {
        return false;
    }
    swapTiles(curIndexOfZero, curIndexOfZero + 1);
    curIndexOfZero = curIndexOfZero + 1;
    return true;
}

int posOfZero()
{
    for (int i = 0; i < sizeOfBoard * sizeOfBoard; i++)
    {
        if (tiles[i] == 0)
        {
            return i;
        }
    }
}

int search(int g, int threshold)
{
    int f = g + manhattan();

    //greater f encountered
    if (f > threshold)
    {
        return f;
    }

    //Goal node found
    if (isGoal())
    {
        cout << path.size() - 1 << endl;
        for (int i = 1; i < path.size(); i++) {
            cout << path[i] << endl;
        }
        return FOUND;
    }

    int min = INT_MAX;
    int temp;

    if (path.back() != "up" && moveDown())
    {
        path.push_back("down");
        temp = search(g + 1, threshold);
        if (temp == FOUND)
        {
            return FOUND;
        }
        if (temp < min)
        {
            min = temp;
        }
        path.pop_back();
        moveUp();
    }
    if (path.back() != "down" && moveUp())
    {
        path.push_back("up");
        temp = search(g + 1, threshold);
        if (temp == FOUND)
        {
            return FOUND;
        }
        if (temp < min)
        {
            min = temp;
        }
        path.pop_back();
        moveDown();
    }
    if (path.back() != "left" && moveRight())
    {
        path.push_back("right");
        temp = search(g + 1, threshold);
        if (temp == FOUND)
        {
            return FOUND;
        }
        if (temp < min)
        {
            min = temp;
        }
        path.pop_back();
        moveLeft();
    }
    if (path.back() != "right" && moveLeft())
    {
        path.push_back("left");
        temp = search(g + 1, threshold);
        if (temp == FOUND)
        {
            return FOUND;
        }
        if (temp < min)
        {
            min = temp;
        }
        path.pop_back();
        moveRight();
    }

    //return the minimum 'f' encountered greater than threshold
    return min;
}

void idastar() {
    int threshold = manhattan();
    path.push_back("init");
    int temp;

    while (true)
    {
        temp = search(0, threshold);
        if (temp == FOUND)
        {
            break;
        }
        threshold = temp;
    }
}

int main()
{
    int n, index;
    cin >> n >> index;
    int tile;

    for (int i = 0; i <= n; i++)
    {
        cin >> tile;
        tiles.push_back(tile);
    }

    sizeOfBoard = sqrt(n + 1);

    if (index == -1) {
        goalIndexOfZero = sizeOfBoard * sizeOfBoard - 1; // bottom right corner cell
    }
    else {
        goalIndexOfZero = index;
    }

    curIndexOfZero = posOfZero();

    idastar();

    return 0;
}

/*
8
0
3 5 1
6 4 2
7 0 8

8
-1
3 5 1
6 4 2
7 0 8

15
-1
3 5 4 7
14 11 9 6
1 12 2 15
13 0 10 8

15
-1
7 1 3 8
2 6 4 12
5 10 0 11
9 13 14 15

15
-1
10 5 8 11
2 12 9 6
13 1 0 15
4 7 3 14
*/

