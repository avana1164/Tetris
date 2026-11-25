#include "Block.hpp"

Block::Block(char type) {
    x = 400;
    y = 0;
    this->type = type;
    switch (type) {
        case 'I':
            grid = {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}};
            break;
        case 'J':
            grid = {{1, 0, 0}, {1, 1, 1}, {0, 0, 0}};
            break;      
        case 'L':
            grid = {{0, 0, 1}, {1, 1, 1}, {0, 0, 0}};
            break;
        case 'O':
            grid = {{1, 1}, {1, 1}};
            break; 
        case 'S':
            grid = {{0, 1, 1}, {1, 1, 0}, {0, 0, 0}};
            break;   
        case 'T':
            grid = {{0, 1, 0}, {1, 1, 1}, {0, 0, 0}};
            break; 
        case 'Z':
            grid = {{1, 1, 0}, {0, 1, 1}, {0, 0, 0}};
            break; 
        default:
            break;   
    }
}

void Block::move(int dx, int dy) {
    x += (dx * displacement);
    y += (dy * displacement);
}

void Block::rotate(bool clockwise) {
    int gridSize = grid.size();

    if (!clockwise) {
        for (int i = 0; i < gridSize; ++i) {
            reverse(grid[i].begin(), grid[i].end());
        }
    }

    vector<vector<int>> copy = grid;

    //take transpose
    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            grid[j][i] = copy[i][j];
        }
    }

    if (clockwise) {
        for (int i = 0; i < gridSize; ++i) {
            reverse(grid[i].begin(), grid[i].end());
        }
    }
}

vector<vector<int>> Block::getGrid() {
    return grid;
}
