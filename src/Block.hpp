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
        const int numCells;
        int x, y;
        unique_ptr<SDL_FRect[]> cellRects;
        int dimension;
        char type;

        void reverseRows();
    public:
        Block(char type);
        void move(int dx, int dy);
        void rotate(bool clockwise);
        void draw(SDL_Renderer *renderer);
        int getX();
        int getY();
        vector<pair<int, int>> getBlockCells();
};
