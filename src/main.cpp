#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <random>
#include "Block.hpp"

struct SDLState {
    SDL_Window *window;
    SDL_Renderer *renderer;
};

void cleanup(SDLState &state);
char generateBlock();
void drawGrid(SDL_Renderer *renderer);

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const double fallThreshold = 2.0f;
const double moveThreshold = 0.1f;
const double rotationThreshold = 0.25f;
const int GRID_WIDTH = 10;
const int GRID_HEIGHT = 20;
const Uint64 frequency = SDL_GetPerformanceFrequency();

int main(int argc, char *argv[]) {

    SDLState state{nullptr, nullptr};

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error initializing SDL3", state.window);
        return 1;
    }

    // create the window
    state.window = SDL_CreateWindow("Tetris", WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!state.window) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error creating window", state.window);
        cleanup(state);
        return 1;
    }

    // create the renderer
    state.renderer = SDL_CreateRenderer(state.window, nullptr);
    if (!state.renderer) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error creating renderer", state.window);
        cleanup(state);
        return 1;
    }

    SDL_SetRenderVSync(state.renderer, 1);

    // start the game loop

    double movementTimer = 0;
    double fallTimer = 0;
    double rotationTimer = 0;
    
    //0 -> left, 1 -> right, 2 -> down, 3 -> up, 4 -> z
    vector<bool> held{false, false, false, false, false};
    bool moveHeld = false;
    bool rotateHeld = false;
    bool running = true;
    char blockType = generateBlock();
    vector<pair<int, int>> filledCells;
    vector<pair<int, int>> blockCells;
    Block currBlock{blockType};
    Uint64 prevTime = SDL_GetPerformanceCounter();
    while (running) {
        Uint64 currTime = SDL_GetPerformanceCounter();
        double dt = double(currTime - prevTime)/frequency;
        SDL_Event event{0};
        //handle events
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    switch (event.key.scancode) { 
                        case SDL_SCANCODE_LEFT:
                            if (!held[0]) {
                                currBlock.move(-1, 0, filledCells);
                                held[0] = true;
                                movementTimer = 0;
                            }
                            break;
                        case SDL_SCANCODE_RIGHT:
                            if (!held[1]) {
                                currBlock.move(1, 0, filledCells);
                                held[1] = true;
                                movementTimer = 0;
                            }
                            break;
                        case SDL_SCANCODE_DOWN:
                            if (!held[2]) {
                                currBlock.move(0, 1, filledCells);
                                held[2] = true;
                                movementTimer = 0;
                            }
                            break;
                        case SDL_SCANCODE_UP:
                            if (!held[3]) {
                                currBlock.rotate(true, filledCells);
                                held[3] = true;
                                rotationTimer = 0;
                            }
                            break;
                        case SDL_SCANCODE_Z:
                            if (!held[4]) {
                                currBlock.rotate(false, filledCells);
                                held[4] = true;
                                rotationTimer = 0;
                            }
                            break;
                        case SDL_SCANCODE_SPACE:
                            while (currBlock.move(0, 1, filledCells)) {}
                        default:
                            break;
                    }
                    break;
                case SDL_EVENT_KEY_UP:
                    if (event.key.scancode == SDL_SCANCODE_LEFT) {
                        held[0] = false;
                    }
                        
                    if (event.key.scancode == SDL_SCANCODE_RIGHT) {
                        held[1] = false;
                    }

                    if (event.key.scancode == SDL_SCANCODE_DOWN) {
                        held[2] = false;
                    }

                    if (event.key.scancode == SDL_SCANCODE_UP) {
                        held[3] = false;
                    }

                    if (event.key.scancode == SDL_SCANCODE_Z) {
                        held[4] = false;
                    }
                    break;
                default:
                    break;
            }
        }

        //update state

        moveHeld = held[0] || held[1] || held[2];
        rotateHeld = held[3] || held[4]; 
        if (moveHeld) movementTimer += dt;

        if (rotateHeld) rotationTimer += dt;

        if (!held[2]) fallTimer += dt;

        if (moveHeld && movementTimer >= moveThreshold) {
            if (held[0]) {
                currBlock.move(-1, 0, filledCells);
            }

            if (held[1]) {
                currBlock.move(1, 0, filledCells);
            } 
            
            if (held[2]) {
                currBlock.move(0, 1, filledCells);
            }

            movementTimer -= moveThreshold;
        }

        //automated falling
        if (!held[2] && fallTimer >= fallThreshold) {
            currBlock.move(0, 1, filledCells);
            fallTimer -= fallThreshold;
        }

        if (rotateHeld && rotationTimer >= rotationThreshold) {
            if (held[3]) {
                currBlock.rotate(true, filledCells);
            }

            if (held[4]) {
                currBlock.rotate(false, filledCells);
            }

            rotationTimer -= rotationThreshold;
        }

        //clear screen
        SDL_SetRenderDrawColor(state.renderer, 255, 255, 255, 255);
        SDL_RenderClear(state.renderer);

        //redraw
        SDL_SetRenderDrawColor(state.renderer, 0, 0, 255, 255);
        currBlock.draw(state.renderer);
        drawGrid(state.renderer);

        //present
        SDL_RenderPresent(state.renderer);
        prevTime = currTime;

    }
    cleanup(state);
    return 0;
}

void drawGrid(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    float x = 275.0f;
    float y = 150.0f;

    for (int i = 0; i < GRID_WIDTH + 1; ++i) {
        SDL_RenderLine(renderer, x, 150, x, 650);
        x += 25.0f;
    }

    for (int i = 0; i < GRID_HEIGHT + 1; ++i) {
        SDL_RenderLine(renderer, 275, y, 525, y);
        y += 25.0f;
    }
}

char generateBlock() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> range(0, 6);

    switch (range(gen)) {
        case 0:
            return 'I';
        case 1:
            return 'J';
        case 2:
            return 'L';
        case 3:
            return 'O';
        case 4:
            return 'S';
        case 5:
            return 'T';
        case 6:
            return 'Z';
        default:
            return ' ';
    }
}

void cleanup(SDLState &state) {
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    SDL_Quit();
}

