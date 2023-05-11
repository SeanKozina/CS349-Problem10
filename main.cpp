#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

// Define the chess board as a 2D array
char board[8][8] = {
    {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
    {' ', '.', ' ', '.', ' ', '.', ' ', '.'},
    {'.', ' ', '.', ' ', '.', ' ', '.', ' '},
    {' ', '.', ' ', '.', ' ', '.', ' ', '.'},
    {'.', ' ', '.', ' ', '.', ' ', '.', ' '},
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
};

// Map the column letters to their corresponding array indices
unordered_map<char, int> col_map = { {'a', 0}, {'b', 1}, {'c', 2}, {'d', 3},
                                    {'e', 4}, {'f', 5}, {'g', 6}, {'h', 7} };

// Function to print the current state of the chess board
void print_board() {
    cout << "   a  b  c  d  e  f  g  h\n";
    for (int i = 0; i < 8; i++) {
        cout << " " << 8 - i << " ";
        for (int j = 0; j < 8; j++) {
            cout << "|" << board[i][j] << "|";
        }
        cout << " " << 8 - i << endl;
    }
    cout << "   a  b  c  d  e  f  g  h\n";
}

// Function to move a chess piece
void move_piece(string move) {
    int start_row = 8 - (move[1] - '0');
    int start_col = col_map[move[0]];
    int end_row = 8 - (move[3] - '0');
    int end_col = col_map[move[2]];

    board[end_row][end_col] = board[start_row][start_col];
    board[start_row][start_col] = ' ';
}


bool isValidMove(char board[8][8], int startX, int startY, int endX, int endY, bool whiteTurn) {
    cout << startX << startY << endX << endY << endl;
    // Check if start and end positions are within the board boundaries
    if (startX < 0 || startX > 7 || startY < 0 || startY > 7 || endX < 0 || endX > 7 || endY < 0 || endY > 7) {
        return false;
    }

    // Check if the starting piece is of the same color as the current turn
    if ((whiteTurn && islower(board[startX][startY])) || (!whiteTurn && isupper(board[startX][startY]))) {
        return false;
    }

    // Check if the end position contains a piece of the same color as the starting piece
    if ((whiteTurn && isupper(board[endX][endY])) || (!whiteTurn && islower(board[endX][endY]))) {
        return false;
    }

    // Check if the piece is moving in the correct direction
    if (tolower(board[startX][startY]) == 'p') {
        // Pawn
        int forwardDir = whiteTurn ? -1 : 1;
        int startRow = whiteTurn ? 6 : 1;
        int endRow = whiteTurn ? 0 : 7;
        int maxForwardDist = startX == startRow ? 2 : 1;
        if (startY == endY) {
            // Moving forward
            int forwardDist = endX - startX;
            if (forwardDist * forwardDir <= 0 || forwardDist * forwardDir > maxForwardDist) {
                return false;
            }
            if (board[endX][endY] != ' ') {
                return false;
            }
            if (endX == endRow) {
                // Promotion
                return (tolower(board[startX][startY]) == 'p');
            }
        }
        else {
            // Capturing
            if (abs(startY - endY) != 1 || endX != startX + forwardDir) {
                return false;
            }
            if (board[endX][endY] == ' ' || (whiteTurn && islower(board[endX][endY])) || (!whiteTurn && isupper(board[endX][endY]))) {
                return false;
            }
        }
    }
    else if (tolower(board[startX][startY]) == 'r') {
        // Rook
        if (startX != endX && startY != endY) {
            return false;
        }
        if (startX == endX) {
            int step = (startY < endY) ? 1 : -1;
            for (int j = startY + step; j != endY; j += step) {
                if (board[startX][j] != ' ') {
                    return false;
                }
            }
        }
        else {
            int step = (startX < endX) ? 1 : -1;
            for (int i = startX + step; i != endX; i += step) {
                if (board[i][startY] != ' ') {
                    return false;
                }
            }
        }
    }
    else if (tolower(board[startX][startY]) == 'n') {
        // Knight logic
        int dx = abs(endX - startX);
        int dy = abs(endY - startY);
        if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2)) {
            // The knight move is valid
            return true;
        }
        else {
            // The knight move is invalid
            return false;
        }
    }
    else if (tolower(board[startX][startY]) == 'b') {
        // Bishop
        if (abs(startX - endX) != abs(startY - endY)) {
            return false;
        }
        int xstep = (startX < endX) ? 1 : -1;
        int ystep = (startY < endY) ? 1 : -1;
        int j = startY + ystep;
        for (int i = startX + xstep; i != endX; i += xstep) {
            if (board[i][j] != ' ') {
                return false;
            }
            j += ystep;
        }
    }
    else if (tolower(board[startX][startY]) == 'q') {
        // Queen
        if (startX == endX || startY == endY) {
            // Rook move
            int step;
            if (startX == endX) {
                step = (startY < endY) ? 1 : -1;
                for (int j = startY + step; j != endY; j += step) {
                    if (board[startX][j] != ' ') {
                        return false;
                    }
                }
            }
            else {
                step = (startX < endX) ? 1 : -1;
                for (int i = startX + step; i != endX; i += step) {
                    if (board[i][startY] != ' ') {
                        return false;
                    }
                }
            }
        }
        else if (abs(startX - endX) == abs(startY - endY)) {
            // Bishop move
            int xstep = (startX < endX) ? 1 : -1;
            int ystep = (startY < endY) ? 1 : -1;
            int j = startY + ystep;
            for (int i = startX + xstep; i != endX; i += xstep) {
                if (board[i][j] != ' ') {
                    return false;
                }
                j += ystep;
            }
        }
        else {
            return false;
        }
    }
    else if (tolower(board[startX][startY]) == 'k') {
        // King
        int dx = abs(endX - startX);
        int dy = abs(endY - startY);
        if (!((dx == 1 && dy == 0) || (dx == 0 && dy == 1) || (dx == 1 && dy == 1))) {
            return false;
        }
    }
    return true;
}

int main() {
    string input_file_name;
    cout << "Enter input file name: ";
    cin >> input_file_name;

    ifstream input_file(input_file_name);
    if (!input_file.is_open()) {
        cout << "Unable to open input file.\n";
        return 1;
    }

    string move;
    bool whiteTurn = true;

    while (getline(input_file, move)) {
        // Print the current turn
        if (whiteTurn) {
            cout << "White's turn.\n";
        }
        else {
            cout << "Black's turn.\n";
        }

        // Check for valid input
        if (move.length() != 4) {
            cout << "Invalid move: incorrect length.\n";
            continue;
        }
        if (col_map.count(move[0]) == 0 || col_map.count(move[2]) == 0) {
            cout << "Invalid move: invalid column.\n";
            continue;
        }
        if (move[1] < '1' || move[1] > '8' || move[3] < '1' || move[3] > '8') {
            cout << "Invalid move: invalid row.\n";
            continue;
        }

        // Convert the move to start and end positions
        int startX = move[0] - 'a';
        int startY = move[1] - '1';
        int endX = move[2] - 'a';
        int endY = move[3] - '1';

        // Check if the move is valid
        if (!isValidMove(board, startX, startY, endX, endY, whiteTurn)) {
            cout << "Invalid move: not a valid move.\n";
            continue;
        }

        // Move the piece
        move_piece(move);

        // Switch the turn
        whiteTurn = !whiteTurn;
        print_board();
    }

    // Print the final board
    print_board();

    return 0;
}
