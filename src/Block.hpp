#pragma once

#include <vector>
#include <algorithm>

using namespace std;

class Block {
    private:
        vector<vector<int>> grid;
        const double displacement = 50.0f;
        int x, y;
        char type;
    public:
        Block(char type);
        void move(int dx, int dy);
        void rotate(bool clockwise);
        vector<vector<int>> getGrid();
};
