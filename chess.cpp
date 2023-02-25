#include <iostream>
#include <ctype.h>

#include <map>
#include <set>
#include <vector>
using namespace std;

struct Position { 
    int row, col;
    bool const operator<(const Position &other) const {
        return row < other.row || (row == other.row && col < other.col);
    }
};

struct Direction {
    int rowOffset, colOffset;
};

map<Position, char> fenToMap() {
    map<Position, char> output;
    string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    int row = 0, col = 0;
    for (char &ch : fen) {
        if (ch == '/') {
                row++;
                col = 0;
        } else if (isdigit(ch)) {
                col += (int)ch;
        } else {
                output[{ row, col }] = ch;
                col++;
        }
    }
    return output;
}

map<char, int> getPieceNumSteps() {
    map<char, int> output;
    output['p'] = 1; // not used, pawn move is specific.
    output['n'] = 1;
    output['k'] = 1;
    output['q'] = 7;
    output['b'] = 7;
    output['r'] = 7;
    return output;
}

map<char, vector<Direction>> getPieceDirections() {
    map<char, vector<Direction>> output;
    
    output['p'] = {}; // not used, pawn move is specific.
    output['n'] = {{1, 2}, {-1, 2}, {1, -2}, {-1, -2}, {2, 1}, {-2, 1}, {2, -1}, {-2, -1}};

    output['r'] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    output['b'] = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
    
    vector<Direction> queenDirections;
    queenDirections.insert(queenDirections.end(), output['r'].begin(), output['r'].end());
    queenDirections.insert(queenDirections.end(), output['b'].begin(), output['b'].end());
    output['q'] = queenDirections;
    output['k'] = queenDirections;
    return output;
}
set<Position> getPawnMoves(Position piecesPosition, vector<Direction> pieceDirections, int numSteps, map<Position, char> pieces) {
    set<Position> output;
    char piece = pieces[piecesPosition];
    
    int rowOffset = 1;
    if (!islower(piece)) {
        rowOffset = -1;
    }
    // might need to deep copy positions
    Position currPosition = {piecesPosition.row + rowOffset, piecesPosition.col};
    if (!pieces.contains(currPosition)){
        output.insert(currPosition);
        if ((piecesPosition.row == 1 && islower(piece)) ||
            (piecesPosition.row == 6 && !islower(piece))) {
                // pass;
                currPosition.row += rowOffset;
                if (!pieces.contains(currPosition)){
                    output.insert(currPosition);
                }
        }
    }
    currPosition.col += 1;
    if (pieces.contains(currPosition)){
        output.insert(currPosition);
    }
    currPosition.col -= 2;
    if (pieces.contains(currPosition)){
        output.insert(currPosition);
    }
    return output;
}

set<Position> getMoves(Position piecesPosition, vector<Direction> pieceDirections, int numSteps, map<Position, char> pieces) {
    set<Position> output;
    char piece = pieces[piecesPosition];
    if (tolower(piece) == 'p') {
        return getPawnMoves(piecesPosition, pieceDirections, numSteps, pieces);
    }
    Position currPosition;
    for (Direction direction: pieceDirections) {
        currPosition = {piecesPosition.row, piecesPosition.col};
        for (int i = 1; i < numSteps + 1; i++) {
            currPosition.row += direction.rowOffset;
            currPosition.col += direction.colOffset;
            if ( min(currPosition.row, currPosition.col) < 0 
                || max(currPosition.row, currPosition.col) > 7) 
                break;
            
            if (pieces.contains(currPosition)){
                if (islower(piece) != islower(pieces[currPosition])) {
                    output.insert(currPosition);
                }
                break;
            }
            output.insert(currPosition);
        }
    }
    return output;
}

int main() {
    map<Position, char> pieces = fenToMap();
    map<char, int> piecesNumSteps = getPieceNumSteps();
    map<char, vector<Direction>> directions = getPieceDirections();

    Position position = {1, 0};
    char piece = tolower(pieces[position]);
    set<Position> moves = getMoves(position, directions[piece], piecesNumSteps[piece], pieces);
    for (Position position: moves) {
        cout << "Possible Move (row, col) : " << position.row << ", " << position.col << "\n";
    }
    return 1;
}