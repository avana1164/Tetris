#include "Block.hpp"

Block::Block(char type) {
    x = 400;
    y = 0;
    this->type = type;
    switch (type) {
        case 'I':
            grid = {{1, 0}, {1, 1}, {1, 2}, {1, 3}};
            dimension = 4;
            break;
        case 'J':
            grid = {{0, 0}, {1, 0}, {1, 1}, {1, 2}};
            dimension = 3;
            break;      
        case 'L':
            grid = {{0, 2}, {1, 0}, {1, 1}, {1, 2}};
            dimension = 3;
            break;
        case 'O':
            grid = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
            dimension = 2;
            break; 
        case 'S':
            grid = {{0, 1}, {0, 2}, {1, 0}, {1, 1}};
            dimension = 3;
            break;   
        case 'T':
            grid = {{0, 1}, {1, 0}, {1, 1}, {1, 2}};
            dimension = 3;
            break; 
        case 'Z':
            grid = {{0, 0}, {0, 1}, {1, 1}, {1, 2}};
            dimension = 3;
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
    if (type == 'O') return;

    if (!clockwise) {
        reverseRows();
    }

    //take transpose
    for (int i = 0; i < numCells; ++i) {
        swap(grid[i].first, grid[i].second);
    }

    if (clockwise) {
        reverseRows();
    }
}

vector<pair<int, int>> Block::getGrid() {
    return grid;
}

void Block::reverseRows() {
    for (int i = 0; i < numCells; ++i) {
        grid[i].first = dimension - grid[i].first - 1;
    }
}
