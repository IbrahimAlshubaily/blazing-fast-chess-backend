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
                col += int(ch) - 48;
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

map<Position, set<Position>> getAllMoves( map<Position, char> pieces, map<char, vector<Direction>> piecesDirections, map<char, int> piecesNumSteps) {
    map<Position, set<Position>> output;
    int numMoves = 0;
    for (auto const& [position, piece] : pieces){
        set<Position> moves = getMoves(position, 
        piecesDirections[tolower(piece)], 
        piecesNumSteps[tolower(piece)],
         pieces);

        output[position] = moves;
        numMoves += moves.size();
        // for (Position dest: moves) {
        //     cout << "Possible move (row, col) -> (row, col): " << position.row << ", " << position.col << " -> " << dest.row << ", " << dest.col << "\n";
        // }
    }
    cout << "NUM MOVES: " << numMoves << "\n";
    return output;
}

void display_board(map<Position, char> pieces) {
    for (int row = 0; row < 8; row++){
        cout << "\n";
        for (int col = 0; col < 8; col++) {
            if (pieces.contains({row, col})) {
               cout << " "  << pieces[{row, col}] << " ";
            } else {
               cout << " - ";
            }
        }
    }
    cout << "\n\n";
}

int main() {
    map<Position, char> pieces = fenToMap();
    map<char, int> piecesNumSteps = getPieceNumSteps();
    map<char, vector<Direction>> piecesDirections = getPieceDirections();

    while (true) {
        display_board(pieces);
        cout << "Enter Source (row col): ";
        string input;
        cin >> input;
        Position source = {int(input.at(0)) - 48, int(input.at(1)) - 48};
        cout << "Selected (row: " << source.row << ", col: " << source.col << ")\n";

        char piece = tolower(pieces[source]);
        set<Position> moves = getMoves(source, piecesDirections[piece], piecesNumSteps[piece], pieces);
        cout << "num possible moves: " << moves.size() << "\n";
        for (Position position: moves) {
            cout << "Possible move (row, col) : " << position.row << ", " << position.col << "\n";
        }
        cout << "Enter destination (row col):" << "\n";
        input = "";
        cin >> input;
        Position destination = {int(input.at(0)) - 48, int(input.at(1)) - 48};
        if (moves.contains(destination)) {
            pieces[destination] = pieces[source];
            pieces.erase(source);
        }
        getAllMoves(pieces, piecesDirections, piecesNumSteps);

    }
    return 1;
}