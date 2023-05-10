#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

bool isValidMove(int startRow, int startCol, int endRow, int endCol);
void initBoard();
void printBoard();
bool movePiece(int startRow, int startCol, int endRow, int endCol);


// Define the chess board as a 2D vector of characters
vector<vector<char>> board(8, vector<char>(8, ' '));


int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " input_file output_file" << std::endl;
        return 1;
    }

    std::string input_file_name = argv[1];
    std::string output_file_name = argv[2];

    std::ifstream input_file("/Users/student/CLionProjects/Chess/inputfile.txt");
    if (!input_file) {
        std::cerr << "Error: could not open input file '" << input_file_name << "'" << std::endl;
        return 1;
    }
    std::string move_str;
    while (input_file >> move_str) {
        bool success = movePiece(move_str[0],move_str[1],move_str[2],move_str[3]);
        if (!success) {
            std::cerr << "Error: invalid move '" << move_str << "'" << std::endl;
            return 1;
        }
    }

    printBoard();

    return 0;
}


// Function to initialize the chess board with all the pieces
void initBoard() {
    // Place the pawns
    for (int col = 0; col < 8; col++) {
        board[1][col] = 'P';
        board[6][col] = 'p';
    }

    // Place the rooks
    board[0][0] = 'R';
    board[0][7] = 'R';
    board[7][0] = 'r';
    board[7][7] = 'r';

    // Place the knights
    board[0][1] = 'N';
    board[0][6] = 'N';
    board[7][1] = 'n';
    board[7][6] = 'n';

    // Place the bishops
    board[0][2] = 'B';
    board[0][5] = 'B';
    board[7][2] = 'b';
    board[7][5] = 'b';

    // Place the queens
    board[0][3] = 'Q';
    board[7][3] = 'q';

    // Place the kings
    board[0][4] = 'K';
    board[7][4] = 'k';
}

// Function to print the chess board
 void printBoard() {
    cout << "  A B C D E F G H\n";
    for (int row = 0; row < 8; row++) {
        cout << 8 - row << " ";
        for (int col = 0; col < 8; col++) {
            cout << board[row][col] << " ";
        }
        cout << 8 - row << endl;
    }
    cout << "  A B C D E F G H\n";
}

// Function to check if a move is valid
bool isValidMove(int startRow, int startCol, int endRow, int endCol) {
    // Make sure the start and end positions are on the board
    if (startRow < 0 || startRow > 7 || startCol < 0 || startCol > 7 ||
        endRow < 0 || endRow > 7 || endCol < 0 || endCol > 7) {
        return false;
    }

    // Make sure there is a piece at the start position
    char piece = board[startRow][startCol];
    if (piece == ' ') {
        return false;
    }

    // Determine the color of the piece
    bool isWhite = (piece >= 'A' && piece <= 'Z');

    // Check if the move is valid for the piece
    switch (toupper(piece)) {
        case 'P': {
            // Check if the move is valid for a pawn
            int dir = (isWhite ? -1 : 1);
            if (endCol == startCol && board[endRow][endCol] == ' ' &&
                (endRow == startRow + dir || (startRow == 6 && endRow == startRow + 2 * dir && board[startRow + dir][endCol] == ' '))) {
                // Pawn is moving forward one or two spaces
                return true;
            } else if (abs(endCol - startCol) == 1 && board[endRow][endCol] != ' ' &&
                       ((isWhite && endRow == startRow - 1) || (!isWhite && endRow == startRow + 1))) {
                // Pawn is capturing diagonally
                return true;
            } else {
                return false;
            }
            break;
        }
        case 'R': {
            // Check if the move is valid for a rook
            if (startRow == endRow) {
                // Rook is moving horizontally
                int minCol = min(startCol, endCol);
                int maxCol = max(startCol, endCol);
                for (int col = minCol + 1; col < maxCol; col++) {
                    if (board[startRow][col] != ' ') {
                        return false;
                    }
                }
                return true;
            } else if (startCol == endCol) {
                // Rook is moving vertically
                int minRow = min(startRow, endRow);
                int maxRow = max(startRow, endRow);
                for (int row = minRow + 1; row < maxRow; row++) {
                    if (board[row][startCol] != ' ') {
                        return false;
                    }
                }
                return true;
            } else {
                return false;
            }
            break;
        }
        case 'N': {
            // Check if the move is valid for a knight
            int dRow = abs(endRow - startRow);
            int dCol = abs(endCol - startCol);
            if ((dRow == 1 && dCol == 2) || (dRow == 2 && dCol == 1)) {
                return true;
            } else {
                return false;
            }
            break;
        }
        case 'B': {
            // Check if the move is valid for a bishop
            if (abs(endRow - startRow) != abs(endCol - startCol)) {
                return false;
            }
            // Bishop is moving diagonally
            int minRow = min(startRow, endRow);
            int maxRow = max(startRow, endRow);
            int minCol = min(startCol, endCol);
            int maxCol = max(startCol, endCol);
            int dRow = (endRow < startRow ? -1 : 1);
            int dCol = (endCol < startCol ? -1 : 1);
            for (int i = 1; i < maxRow - minRow; i++) {
                if (board[minRow + i * dRow][minCol + i * dCol] != ' ') {
                    return false;
                }
            }
            return true;
            break;
        }
        case 'Q': {
            // Check if the move is valid for a queen
            if (startRow == endRow) {
                // Queen is moving horizontally
                int minCol = min(startCol, endCol);
                int maxCol = max(startCol, endCol);
                for (int col = minCol + 1; col < maxCol; col++) {
                    if (board[startRow][col] != ' ') {
                        return false;
                    }
                }
                return true;
            } else if (startCol == endCol) {
                // Queen is moving vertically
                int minRow = min(startRow, endRow);
                int maxRow = max(startRow, endRow);
                for (int row = minRow + 1; row < maxRow; row++) {
                    if (board[row][startCol] != ' ') {
                        return false;
                    }
                }
                return true;
            } else if (abs(endRow - startRow) == abs(endCol - startCol)) {
                // Queen is moving diagonally
                int minRow = min(startRow, endRow);
                int maxRow = max(startRow, endRow);
                int minCol = min(startCol, endCol);
                int maxCol = max(startCol, endCol);
                int dRow = (endRow < startRow ? -1 : 1);
                int dCol = (endCol < startCol ? -1 : 1);
                for (int i = 1; i < maxRow - minRow; i++) {
                    if (board[minRow + i * dRow][minCol + i * dCol] != ' ') {
                        return false;
                    }
                }
                return true;
            } else {
                return false;
            }
            break;
        }
        case 'K': {
            // Check if the move is valid for a king
            int dRow = abs(endRow - startRow);
            int dCol = abs(endCol - startCol);
            if ((dRow == 1 && dCol == 0) || (dRow == 0 && dCol == 1) || (dRow == 1 && dCol == 1)) {
                return true;
            } else {
                return false;
            }
            break;
        }
        default: {
            // Unknown piece type
            return false;
            break;
        }
    }
}



bool movePiece(int startRow, int startCol, int endRow, int endCol) {
    // Check if the move is valid
    if (!isValidMove(startRow, startCol, endRow, endCol)) {
        cout << "Invalid move." << endl;
        return false;
    }

    // Move the piece to the new position
    char piece = board[startRow][startCol];
    board[startRow][startCol] = ' ';
    board[endRow][endCol] = piece;
    return true;
}
