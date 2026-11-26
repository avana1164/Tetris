#include "Block.hpp"

Block::Block(char type): WIDTH{25.0f}, numCells{4}, x{400}, y{0}, cells{make_unique<SDL_FRect[]>(numCells)}, type{type} {
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

    for (int i = 0; i < numCells; ++i) {
        cells[i] = SDL_FRect{grid[i].second * WIDTH + x, grid[i].first * WIDTH + y, WIDTH, WIDTH};
    }
}

void Block::move(int dx, int dy) {
    x += (dx * WIDTH);
    y += (dy * WIDTH);
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

void Block::reverseRows() {
    for (int i = 0; i < numCells; ++i) {
        grid[i].second = dimension - grid[i].second - 1;
    }
}

void Block::draw(SDL_Renderer *renderer) {
    for (int i = 0; i < numCells; ++i) {
        cells[i].x = (grid[i].second * WIDTH + x);
        cells[i].y = (grid[i].first * WIDTH + y);
    }

    SDL_RenderFillRects(renderer, cells.get(), numCells);
}

int Block::getX() {
    return x;
}

int Block::getY() {
    return y;
}
