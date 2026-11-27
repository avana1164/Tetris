#include "Block.hpp"

Block::Block(char type): CELL_WIDTH{25.0f}, numCells{4}, y{150}, cellRects{make_unique<SDL_FRect[]>(numCells)}, type{type} {
    switch (type) {
        case 'I':
            blockCells = {{1, 0}, {1, 1}, {1, 2}, {1, 3}};
            dimension = 4;
            x = 325;
            break;
        case 'J':
            blockCells = {{0, 0}, {1, 0}, {1, 1}, {1, 2}};
            dimension = 3;
            x = 350;
            break;      
        case 'L':
            blockCells = {{0, 2}, {1, 0}, {1, 1}, {1, 2}};
            dimension = 3;
            x = 350;
            break;
        case 'O':
            blockCells = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
            dimension = 2;
            x = 375;
            break; 
        case 'S':
            blockCells = {{0, 1}, {0, 2}, {1, 0}, {1, 1}};
            dimension = 3;
            x = 350;
            break;   
        case 'T':
            blockCells = {{0, 1}, {1, 0}, {1, 1}, {1, 2}};
            dimension = 3;
            x = 350;
            break; 
        case 'Z':
            blockCells = {{0, 0}, {0, 1}, {1, 1}, {1, 2}};
            dimension = 3;
            x = 350;
            break; 
        default:
            break;   
    }

    for (int i = 0; i < numCells; ++i) {
        cellRects[i] = SDL_FRect{blockCells[i].second * CELL_WIDTH + x, blockCells[i].first * CELL_WIDTH + y, CELL_WIDTH, CELL_WIDTH};
    }
}

void Block::move(int dx, int dy) {
    x += (dx * CELL_WIDTH);
    y += (dy * CELL_WIDTH);
}

void Block::rotate(bool clockwise) {
    if (type == 'O') return;

    if (!clockwise) {
        reverseRows();
    }

    //take transpose
    for (int i = 0; i < numCells; ++i) {
        swap(blockCells[i].first, blockCells[i].second);
    }

    if (clockwise) {
        reverseRows();
    }
}

void Block::reverseRows() {
    for (int i = 0; i < numCells; ++i) {
        blockCells[i].second = dimension - blockCells[i].second - 1;
    }
}

void Block::draw(SDL_Renderer *renderer) {
    for (int i = 0; i < numCells; ++i) {
        cellRects[i].x = (blockCells[i].second * CELL_WIDTH + x);
        cellRects[i].y = (blockCells[i].first * CELL_WIDTH + y);
    }

    SDL_RenderFillRects(renderer, cellRects.get(), numCells);
}

int Block::getX() {
    return x;
}

int Block::getY() {
    return y;
}

vector<pair<int, int>> Block::getBlockCells() {
    return blockCells;
}
