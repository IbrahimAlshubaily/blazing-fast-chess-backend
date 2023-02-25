#include <iostream>
#include <ctype.h>
#include <map>
using namespace std;

struct Position { 
    int row, col;
    bool const operator<(const Position &other) const {
        return row < other.row || (row == other.row && col < other.col);
    }
};

map<Position, string> fenToMap() {
    map<Position, string> output;
    string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    int row = 0, col = 0;
    for (char &ch : fen) {
        if (ch == '/') {
                row++;
                col = 0;
        } else if (isdigit(ch)) {
                col += (int)ch;
        } else {
                Position position = { row, col };
                output[position] = ch;
                col++;
        }
    }
    return output;
}
int main() {
    cout << "I am shazam!\n";
    map<Position, string> pieces = fenToMap();
    cout << pieces.size() << "\n";
    return 1;
}