#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <chrono> // Include the chrono library for time measurement

using namespace std;
using namespace std::chrono; // Use the chrono namespace for simplicity

bool isSolved(const vector<char> currentState, int n) {
    for (int i = 0; i < n; ++i) {
        if (currentState[i] != '<' || currentState[2 * n - i] != '>') {
            return false;
        }
    }
    return true;
}

void printCurrentState(const vector<char>& board) {
    for (char c : board) {
        cout << c;
    }
    cout << endl;
}

bool solveDFS(vector<char>& board, int emptyPos, int n, vector<vector<char>>& solutions) {
    if (isSolved(board, n)) {
        return true;
    }

    if (emptyPos > 0 && board[emptyPos - 1] == '>') {
        swap(board[emptyPos], board[emptyPos - 1]);
        solutions.push_back(board);
        if (solveDFS(board, emptyPos - 1, n, solutions)) {
            return true;
        }
        solutions.pop_back();
        swap(board[emptyPos], board[emptyPos - 1]);
    }

    if (emptyPos > 1 && board[emptyPos - 2] == '>') {
        swap(board[emptyPos], board[emptyPos - 2]);
        solutions.push_back(board);
        if (solveDFS(board, emptyPos - 2, n, solutions)) {
            return true;
        }
        solutions.pop_back();
        swap(board[emptyPos], board[emptyPos - 2]);
    }

    if (emptyPos < 2 * n && board[emptyPos + 1] == '<') {
        swap(board[emptyPos], board[emptyPos + 1]);
        solutions.push_back(board);
        if ((solveDFS(board, emptyPos + 1, n, solutions))) {
            return true;
        }
        solutions.pop_back();
        swap(board[emptyPos], board[emptyPos + 1]);
    }

    if (emptyPos < 2 * n - 1 && board[emptyPos + 2] == '<') {
        swap(board[emptyPos], board[emptyPos + 2]);
        solutions.push_back(board);
        if (solveDFS(board, emptyPos + 2, n, solutions)) {
            return true;
        }
        solutions.pop_back();
        swap(board[emptyPos], board[emptyPos + 2]);
    }

    return false;
}

int main() {
    int n;
    cin >> n;

    vector<char> board(2 * n + 1, '_');
    for (int i = 0; i < n; ++i) {
        board[i] = '>';
        board[2 * n - i] = '<';
    }

    vector<vector<char>> solutions;
    solutions.push_back(board);

    // Start measuring time
    auto start = high_resolution_clock::now();

    solveDFS(board, n, n, solutions);

    // Stop measuring time
    auto end = high_resolution_clock::now();

    // Calculate the duration
    auto duration = duration_cast<milliseconds>(end - start);

    // Output the result steps
    for (const auto& step : solutions) {
        printCurrentState(step);
    }

    // Output the time taken in milliseconds
    cout << "Time taken: " << duration.count() << " milliseconds" << endl;

    return 0;
}