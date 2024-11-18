import java.util.Scanner;

public class Main {
    static char[][] board = {
            {'-', '-', '-'},
            {'-', '-', '-'},
            {'-', '-', '-'}
    };

    static char emptyCell = '-';
    static int maxScore = 10;
    static int minScore = -10;
    static boolean isPersonTurn;
    static char computerSymbol;
    static char personSymbol;
    static int invalidPosition = -1;
    static int movesAvailable = 9;

    public static void main(String[] args) {
        printBoard();

        Scanner scanner = new Scanner(System.in);
        System.out.print("Do you want to start the game? Enter 1 for first and 0 for second: ");
        isPersonTurn = scanner.nextInt() == 1;

        if (isPersonTurn) {
            computerSymbol = 'o';
            personSymbol = 'x';
        } else {
            computerSymbol = 'x';
            personSymbol = 'o';
        }

        while (movesAvailable > 0 && !isThereWinner()) {
            if (isPersonTurn) {
                int i, j;
                do {
                    System.out.print("Row: ");
                    i = scanner.nextInt();
                    System.out.print("Column: ");
                    j = scanner.nextInt();
                } while (!makeTurn(i - 1, j - 1, isPersonTurn));
                isPersonTurn = !isPersonTurn;
            } else {
                int[] bestTurn = findBestTurnForComputer();
                makeTurn(bestTurn[0], bestTurn[1], isPersonTurn);
                isPersonTurn = !isPersonTurn;
            }
        }

        if (!isThereWinner() && movesAvailable == 0) {
            System.out.println("The game ends in a draw!");
        }
        scanner.close();
    }

    static void printBoard() {
        System.out.println("-------------");
        for (int i = 0; i < 3; i++) {
            System.out.print("| ");
            for (int j = 0; j < 3; j++) {
                System.out.print(board[i][j] + " | ");
            }
            System.out.println("\n-------------");
        }
    }

    static boolean areThereMovesLeft() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == emptyCell) {
                    return true;
                }
            }
        }
        return false;
    }

    static int evaluateBoard(int depth) {
        for (int row = 0; row < 3; row++) {
            if (board[row][0] == board[row][1] && board[row][1] == board[row][2]) {
                if (board[row][0] == computerSymbol) {
                    return maxScore - depth;
                } else if (board[row][0] == personSymbol) {
                    return minScore + depth;
                }
            }
        }

        for (int col = 0; col < 3; col++) {
            if (board[0][col] == board[1][col] && board[1][col] == board[2][col]) {
                if (board[0][col] == computerSymbol) {
                    return maxScore - depth;
                } else if (board[0][col] == personSymbol) {
                    return minScore + depth;
                }
            }
        }

        if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
            if (board[0][0] == computerSymbol) {
                return maxScore - depth;
            } else if (board[0][0] == personSymbol) {
                return minScore + depth;
            }
        }

        if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
            if (board[0][2] == computerSymbol) {
                return maxScore - depth;
            } else if (board[0][2] == personSymbol) {
                return minScore + depth;
            }
        }

        return 0;
    }

    static int maximizer(int alpha, int beta, int depth) {
        int curScore = evaluateBoard(depth);

        if (curScore != 0) {
            return curScore;
        }

        if (!areThereMovesLeft()) {
            return 0;
        }

        int bestScore = Integer.MIN_VALUE;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == emptyCell) {
                    board[i][j] = computerSymbol;

                    bestScore = Math.max(bestScore, minimizer(alpha, beta, depth + 1));

                    board[i][j] = emptyCell;

                    if (bestScore >= beta) {
                        return bestScore;
                    }
                    alpha = Math.max(alpha, bestScore);
                }
            }
        }
        return bestScore;
    }

    static int minimizer(int alpha, int beta, int depth) {
        int curScore = evaluateBoard(depth);

        if (curScore != 0) {
            return curScore;
        }

        if (!areThereMovesLeft()) {
            return 0;
        }

        int bestScore = Integer.MAX_VALUE;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == emptyCell) {
                    board[i][j] = personSymbol;

                    bestScore = Math.min(bestScore, maximizer(alpha, beta, depth + 1));

                    board[i][j] = emptyCell;

                    if (bestScore <= alpha) {
                        return bestScore;
                    }
                    beta = Math.min(beta, bestScore);
                }
            }
        }
        return bestScore;
    }

    static int[] findBestTurnForComputer() {
        int bestValue = Integer.MIN_VALUE;
        int[] bestNextTurn = {invalidPosition, invalidPosition};

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == emptyCell) {
                    board[i][j] = computerSymbol;

                    int curTurnValue = minimizer(Integer.MIN_VALUE, Integer.MAX_VALUE, 0);

                    board[i][j] = emptyCell;

                    if (curTurnValue > bestValue) {
                        bestNextTurn[0] = i;
                        bestNextTurn[1] = j;
                        bestValue = curTurnValue;
                    }
                }
            }
        }
        return bestNextTurn;
    }

    static boolean isThereWinner() {
        return evaluateBoard(0) != 0;
    }

    static boolean makeTurn(int i, int j, boolean isPersonTurn) {

        if (i < 0 || j < 0) {
            System.out.println("There are no such coordinates");
            printBoard();
            return false;
        }
        if (board[i][j] == emptyCell) {
            board[i][j] = isPersonTurn ? personSymbol : computerSymbol;
            movesAvailable--;

            System.out.println("==========");
            printBoard();

            if (isThereWinner()) {
                if (isPersonTurn) {
                    System.out.println("Congratulations, you are the winner!");
                } else {
                    System.out.println("Sorry, you lost!");
                }
            }
            return true;
        }
        System.out.println("This cell is not empty! Choose another.");
        return false;
    }
}