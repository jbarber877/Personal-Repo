//Lab 06 - I added this comment




import java.util.Scanner;

public class ConnectFour {
    public static void printBoard (char[][] array){
        //this will print the board
        for (int i = 0; i < array.length; i++){
            for (int j = 0; j < array[0].length; j++){
                System.out.print(array[(array.length - 1)- i][j]);
            }
            System.out.println();
        }
        System.out.println();
    }

    public static void initializeBoard(char[][]array) {
        //This will set each spot in the array to “-”.
        for (int i = 0; i < array.length; i++) {
            for (int j = 0; j < array[0].length; j++) {
                array[i][j] = '-';
            }
        }
    }

    public static int insertChip (char[][] array, int col, char chipType){
        //Places the token in the column that the user has chosen
        int row;

        for (row = 0; row < array.length; row ++) {
            //Will find the next available spot in that column
            if (array[row][col] == '-') {
                array[row][col] = chipType;
                break;
            } else {
                continue;
            }
        }

        // The row that the token is placed in is returned.
        return row;
    }

    public static boolean checkIfWinner (char[][] array, int col, int row, char chipType){
        //After a token is added, checks whether the token in this location,
        // of the specified chip type, creates four in a row.
        int runLength;
        boolean isWinner = false;

        //Begin by checking the rows
        runLength = 0;
        for (int i = 0; i < array[0].length; i++){
            if (array[row][i] == chipType) {
                runLength += 1;
                //check if there are four in a row
                if (runLength == 4) {
                    isWinner = true;
                }
            }
            else {
                runLength = 0;
            }
        }

        //next check the columns
        runLength = 0;
        for (int i = 0; i < array.length; i++){
            if (array[i][col] == chipType) {
                runLength += 1;
                //check if there are four in a row
                if (runLength == 4) {
                    isWinner = true;
                }
            }
            else {
                runLength = 0;
            }
        }
        // Will return true if someone won, and false otherwise.
        return isWinner;
    }

    public static boolean checkIfDraw (char[][] array){
        //checks for open slots. If no slots are open, it is a draw.
        int numOpenSlots = 0;
        boolean isDraw;

        //examine only the top row since it is the last to fill.
        for (int i = 0; i < array[0].length; i++){
            if (array[array.length -1][i] == '-'){
                numOpenSlots += 1;
            }
        }

        isDraw = (numOpenSlots > 0)? false:true;
        return isDraw;
    }

    public static void main (String[] args) {
        //set up the scanner
        Scanner scnr = new Scanner(System.in);

        //local variables
        int boardHeight, boardLength;
        int player1Col, player2Col;
        int row;
        char player1Chip = 'x', player2Chip = 'o';
        boolean isWinner = false, isDraw = false;
        String winner ="";

        //ask for height and length of the board
        System.out.println("What would you like the height of the board to be?");
        boardHeight = scnr.nextInt();
        System.out.println("What would you like the length of the board to be?");
        boardLength = scnr.nextInt();

        //initialize and print the empty board
        char[][] board = new char[boardHeight][boardLength];
        initializeBoard(board);
        printBoard(board);

        //tell the players what their tokens are
        System.out.println("Player 1: " + player1Chip);
        System.out.println("Player 2: " + player2Chip);
        System.out.println();

        //The players will take turns placing their tokens by choosing columns
        while ((!isWinner) && (!isDraw)) {

            // check if it is a draw
            isDraw = checkIfDraw(board);
            if (isDraw){
                System.out.println("Draw. Nobody wins.");
                continue;
            }

            System.out.println("Player 1: Which column would you like to choose?");
            player1Col = scnr.nextInt();
            row = insertChip(board, player1Col, player1Chip);

            //print the board
            printBoard(board);

            //check if player1 has won
            isWinner = checkIfWinner(board, player1Col, row, player1Chip);
            if (isWinner){
                //declare victory
                System.out.println("Player 1 won the game!");
                continue;
            }

            //otherwise, continue
            System.out.println("Player 2: Which column would you like to choose?");
            player2Col = scnr.nextInt();
            row = insertChip(board, player2Col, player2Chip);

            printBoard(board);

            //check if player2 has won
            isWinner = checkIfWinner(board, player2Col, row, player2Chip);
            if (isWinner){
                //announce winner
                System.out.println("Player 2 won the game!");
                continue;
            }
        }
    }
}
