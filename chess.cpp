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

map<char, vector<Direction>> getPieceDirections() {
    map<char, vector<Direction>> output;
    
    output['p'] = {{1, 0}}; // remove later, pawn move is specific.
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

set<Position> getMoves(Position piecesPosition, vector<Direction> pieceDirections, map<Position, char> pieces) {
    set<Position> output;
    char piece = pieces[piecesPosition];
    cout << piece << "\n";
    cout << piecesPosition.row << ", " << piecesPosition.col << "\n";
    cout << pieceDirections.size() << "\n";
    cout << pieces.size() << "\n";
    return output;
}

int main() {
    cout << "I am shazam!\n";
    
    map<Position, char> pieces = fenToMap();
    cout << pieces.size() << "\n";
    
    map<char, vector<Direction>> directions = getPieceDirections();
    cout << directions.size() << "\n";

    Position position = {1, 0};
    char piece = pieces[position];
    set<Position> moves = getMoves(position, directions[tolower(piece)], pieces);
    cout << moves.size() << "\n";

    return 1;
}