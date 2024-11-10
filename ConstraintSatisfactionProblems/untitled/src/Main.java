import java.util.ArrayList;
import java.util.Random;
import java.util.Scanner;

public class Main {

    private static boolean hasConflicts;
    private static int boardSize;
    private static int[] queenPositions; // queenPositions[col] = row where queen is placed in col
    private static int[] rowConflicts;   // rowConflicts[row] = number of queens in the row
    private static int[] mainDiagonalConflicts; // conflicts for main diagonals (top-left to bottom-right)
    private static int[] secondaryDiagonalConflicts; // conflicts for secondary diagonals (top-right to bottom-left)
    private static Random random = new Random();

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        boardSize = scanner.nextInt();

        queenPositions = new int[boardSize];
        rowConflicts = new int[boardSize];
        mainDiagonalConflicts = new int[2 * boardSize - 1];
        secondaryDiagonalConflicts = new int[2 * boardSize - 1];

        hasConflicts = true;

        long startTime = System.currentTimeMillis();

        initializeBoard();

        solveConflicts();

        long endTime = System.currentTimeMillis();

        if( boardSize < 100) {
            printBoard();
        }
        System.out.println("Time: " + (endTime - startTime) + " ms");
    }

    private static void initializeBoard() {
        int column = 1;
        for (int row = 0; row < boardSize; row++) {
            queenPositions[column] = row;
            rowConflicts[row]++;
            mainDiagonalConflicts[column - row + boardSize - 1]++;
            secondaryDiagonalConflicts[column + row]++;

            column += 2;
            if (column >= boardSize) {
                column = 0;
            }
        }
    }

    private static int getColumnWithMaxConflicts() {
        int maxConflicts = -1;
        ArrayList<Integer> columnsWithMaxConflicts = new ArrayList<>();

        for (int column = 0; column < boardSize; column++) {
            int row = queenPositions[column];
            int currentConflicts = rowConflicts[row]
                    + mainDiagonalConflicts[column - row + boardSize - 1]
                    + secondaryDiagonalConflicts[column + row] - 3;

            if (currentConflicts == maxConflicts) {
                columnsWithMaxConflicts.add(column);
            } else if (currentConflicts > maxConflicts) {
                maxConflicts = currentConflicts;
                columnsWithMaxConflicts.clear();
                columnsWithMaxConflicts.add(column);
            }
        }

        if (maxConflicts == 0) {
            hasConflicts = false;
        }

        return columnsWithMaxConflicts.get(random.nextInt(columnsWithMaxConflicts.size()));
    }

    private static int getRowWithMinConflicts(int column) {
        int minConflicts = boardSize + 1;
        ArrayList<Integer> rowsWithMinConflicts = new ArrayList<>();

        for (int row = 0; row < boardSize; row++) {
            int currentConflicts = (queenPositions[column] == row)
                    ? rowConflicts[row] + mainDiagonalConflicts[column - row + boardSize - 1] + secondaryDiagonalConflicts[column + row] - 3
                    : rowConflicts[row] + mainDiagonalConflicts[column - row + boardSize - 1] + secondaryDiagonalConflicts[column + row];

            if (currentConflicts == minConflicts) {
                rowsWithMinConflicts.add(row);
            } else if (currentConflicts < minConflicts) {
                minConflicts = currentConflicts;
                rowsWithMinConflicts.clear();
                rowsWithMinConflicts.add(row);
            }
        }

        return rowsWithMinConflicts.get(random.nextInt(rowsWithMinConflicts.size()));
    }

    private static void updateBoardState(int newRow, int column) {
        int oldRow = queenPositions[column];

        // Remove the queen from its old position
        rowConflicts[oldRow]--;
        mainDiagonalConflicts[column - oldRow + boardSize - 1]--;
        secondaryDiagonalConflicts[column + oldRow]--;

        // Place the queen in the new position
        queenPositions[column] = newRow;
        rowConflicts[newRow]++;
        mainDiagonalConflicts[column - newRow + boardSize - 1]++;
        secondaryDiagonalConflicts[column + newRow]++;
    }

    private static void solveConflicts() {
        int iteration = 0;

        while (iteration++ <= boardSize) {
            int column = getColumnWithMaxConflicts();
            if (!hasConflicts) {
                return;
            }
            int newRow = getRowWithMinConflicts(column);
            updateBoardState(newRow, column);
        }

        if (hasConflicts) {
            solveConflicts();
        }
    }

    private static void printBoard() {
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                if (queenPositions[j] == i) {
                    System.out.print("* ");
                } else {
                    System.out.print("_ ");
                }
            }
            System.out.println();
        }
    }
}