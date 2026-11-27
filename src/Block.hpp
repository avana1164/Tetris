#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <vector>
#include <algorithm>
#include <memory>

using namespace std;

class Block {
    private:
        vector<pair<int, int>> blockCells;
        const float CELL_WIDTH;
        const int MIN_X, MAX_X, MIN_Y, MAX_Y;
        const int numCells;
        int x, y;
        unique_ptr<SDL_FRect[]> cellRects;
        int dimension;
        char type;
        void reverseRows();
    public:
        Block(char type);
        bool move(int dx, int dy, const vector<pair<int, int>> &filledCells);
        bool rotate(bool clockwise, const vector<pair<int, int>> &filledCells);
        void draw(SDL_Renderer *renderer);
        int getX();
        int getY();
};
