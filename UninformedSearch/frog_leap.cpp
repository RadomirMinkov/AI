#include <iostream>
#include <vector>
#include <chrono>

std::vector<std::vector<char>> path;

void printBoard(const std::vector<char>& board) {
	for (char c : board) {
		std::cout << c;
	}
	std::cout << std::endl;
}

bool isGoalState(const std::vector<char>& board, int number) {
	for (int i = 0; i < number; i++) {
		if (board[i] != '<' || board[2 * number - i] != '>') {
			return false;
		}
	}
	return board[number] == '_';
}

bool dfs(std::vector<char>& board, int zeroPos, int number) {
	if (isGoalState(board, number)) {
		return true;
	}

	if (zeroPos > 0 && board[zeroPos - 1] == '>') {
		std::swap(board[zeroPos], board[zeroPos - 1]);
		path.push_back(board);
		if (dfs(board, zeroPos - 1, number)) {
			return true;
		}
		path.pop_back();
		std::swap(board[zeroPos], board[zeroPos - 1]);
	}

	if (zeroPos > 1 && board[zeroPos - 2] == '>') {
		std::swap(board[zeroPos], board[zeroPos - 2]);
		path.push_back(board);
		if (dfs(board, zeroPos - 2, number)) {
			return true;
		}
		path.pop_back();
		std::swap(board[zeroPos], board[zeroPos - 2]);
	}

	if (zeroPos < 2 * number && board[zeroPos + 1] == '<') {
		std::swap(board[zeroPos], board[zeroPos + 1]);
		path.push_back(board);
		if (dfs(board, zeroPos + 1, number)) {
			return true;
		}
		path.pop_back();
		std::swap(board[zeroPos], board[zeroPos + 1]);
	}

	if (zeroPos < 2 * number - 1 && board[zeroPos + 2] == '<') {
		std::swap(board[zeroPos], board[zeroPos + 2]);
		path.push_back(board);
		if (dfs(board, zeroPos + 2, number)) {
			return true;
		}
		path.pop_back();
		std::swap(board[zeroPos], board[zeroPos + 2]);
	}

	return false;
}

int main() {
	int number;

	std::cout << "Enter the number of the frogs on one of the sides: ";
	std::cin >> number;
	std::vector<char> board(2 * number + 1);

	for (int i = 0; i < number; ++i) {
		board[i] = '>';
	}

	board[number] = '_';
	for (int i = number + 1; i < 2 * number + 1; ++i) {
		board[i] = '<';
	}

	path.push_back(board);

	std::cout << "Starting the clock!" << std::endl;
	auto start = std::chrono::high_resolution_clock::now();
	dfs(board, number, number);
	auto stop = std::chrono::high_resolution_clock::now();
	std::cout << "Stopped the clock!" << std::endl;

	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	std::cout << "The algorithm time is: " << duration.count() << " microseconds!" << std::endl;

	// Print the solution path
	for (size_t i = 0; i < path.size(); ++i) {
		printBoard(path[i]);
	}

	return 0;
}