#include "Block.hpp"

Block::Block(char type): CELL_WIDTH{25.0f}, MIN_X{275}, MAX_X{525}, MIN_Y{150}, MAX_Y{650}, 
    numCells{4}, x{350}, y{150}, cellRects{make_unique<SDL_FRect[]>(numCells)}, dimension{3}, type{type} {
    switch (type) {
        case 'I':
            blockCells = {{1, 0}, {1, 1}, {1, 2}, {1, 3}};
            dimension += 1;
            break;
        case 'J':
            blockCells = {{0, 0}, {1, 0}, {1, 1}, {1, 2}};
            break;      
        case 'L':
            blockCells = {{0, 2}, {1, 0}, {1, 1}, {1, 2}};
            break;
        case 'O':
            blockCells = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
            dimension -= 1;
            x += 25;
            break; 
        case 'S':
            blockCells = {{0, 1}, {0, 2}, {1, 0}, {1, 1}};
            break;   
        case 'T':
            blockCells = {{0, 1}, {1, 0}, {1, 1}, {1, 2}};
            break; 
        case 'Z':
            blockCells = {{0, 0}, {0, 1}, {1, 1}, {1, 2}};
            break; 
        default:
            break;   
    }

    for (int i = 0; i < numCells; ++i) {
        cellRects[i] = SDL_FRect{blockCells[i].second * CELL_WIDTH + x, blockCells[i].first * CELL_WIDTH + y, CELL_WIDTH, CELL_WIDTH};
    }
}

bool Block::move(int dx, int dy, const vector<pair<int, int>> &filledCells) {
    int oldX = x;
    int oldY = y;
    x += (dx * CELL_WIDTH);
    y += (dy * CELL_WIDTH);
    for (int i = 0; i < numCells; ++i) {
        int cellX = blockCells[i].second * CELL_WIDTH + x;
        int cellY = blockCells[i].first * CELL_WIDTH + y;
        if (cellX < MIN_X || cellX > MAX_X - 25 || cellY < MIN_Y || cellY > MAX_Y - 25) {
            x = oldX;
            y = oldY;
            return false;
        }
    }

    return true;
}

bool Block::rotate(bool clockwise, const vector<pair<int, int>> &filledCells) {
    if (type == 'O') return false;

    vector<pair<int, int>> copyBlockCells = blockCells;

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

    for (int i = 0; i < numCells; ++i) {
        int cellX = blockCells[i].second * CELL_WIDTH + x;
        int cellY = blockCells[i].first * CELL_WIDTH + y;
        if (cellX < MIN_X || cellX > MAX_X - 25 || cellY < MIN_Y || cellY > MAX_Y - 25) {
            blockCells = copyBlockCells;
            return false;
        }
    }

    return true;
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
