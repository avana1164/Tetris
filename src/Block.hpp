#pragma once

#include <vector>
#include <algorithm>

using namespace std;

class Block {
    private:
        vector<pair<int, int>> grid;
        const double displacement = 50.0f;
        const int numCells = 4;
        int dimension;
        int x, y;
        char type;

        void reverseRows();
    public:
        Block(char type);
        void move(int dx, int dy);
        void rotate(bool clockwise);
        vector<pair<int, int>> getGrid();
};
