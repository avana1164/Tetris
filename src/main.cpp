#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <random>
#include "Block.hpp"

struct SDLState {
    SDL_Window *window;
    SDL_Renderer *renderer;
};

void cleanup(SDLState &state);

int main(int argc, char *argv[]) {

    SDLState state{nullptr, nullptr};

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error initializing SDL3", state.window);
        return 1;
    }

    // create the window
    const int WIDTH = 800;
    const int HEIGHT = 600;
    state.window = SDL_CreateWindow("Tetris", WIDTH, HEIGHT, 0);
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
    const double fallThreshold = 2.0f;
    const double moveThreshold = 0.1f;
    const double rotationThreshold = 0.1f;
    const Uint64 frequency = SDL_GetPerformanceFrequency();

    double movementTimer = 0;
    double fallTimer = 0;
    double rotationTimer = 0;
    
    //0 -> left, 1 -> right, 2 -> down, 3 -> up, 4 -> z
    vector<bool> held{false, false, false, false, false};
    bool moveHeld = false;
    bool rotateHeld = false;
    bool running = true;
    Block currBlock{'T'};
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
                            if (!held[0] && currBlock.getX() > 0) {
                                currBlock.move(-1, 0);
                                held[0] = true;
                                movementTimer = 0;
                            }
                            break;
                        case SDL_SCANCODE_RIGHT:
                            if (!held[1] && currBlock.getX() < 750) {
                                currBlock.move(1, 0);
                                held[1] = true;
                                movementTimer = 0;
                            }
                            break;
                        case SDL_SCANCODE_DOWN:
                            if (!held[2] && currBlock.getY() < 550) {
                                currBlock.move(0, 1);
                                held[2] = true;
                                movementTimer = 0;
                            }
                            break;
                        case SDL_SCANCODE_UP:
                            if (!held[3]) {
                                currBlock.rotate(true);
                                held[3] = true;
                                rotationTimer = 0;
                            }
                            break;
                        case SDL_SCANCODE_Z:
                            if (!held[4]) {
                                currBlock.rotate(false);
                                held[4] = true;
                                rotationTimer = 0;
                            }
                            break;
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

        if (held[3]) rotationTimer += dt;

        if (!held[2]) fallTimer += dt;

        if (moveHeld && movementTimer >= moveThreshold) {
            if (held[0] && currBlock.getX() > 0) {
                currBlock.move(-1, 0);
            }

            if (held[1] && currBlock.getX() < 750) {
                currBlock.move(1, 0);
            } 
            
            if (held[2] && currBlock.getY() < 550) {
                currBlock.move(0, 1);
            }

            movementTimer -= moveThreshold;
        }

        //automated falling
        if (!held[2] && fallTimer >= fallThreshold && currBlock.getY() < 550) {
            currBlock.move(0, 1);
            fallTimer -= fallThreshold;
        }

        if (rotateHeld && rotationTimer >= rotationThreshold) {
            if (held[3]) {
                currBlock.rotate(true);
            }

            if (held[4]) {
                currBlock.rotate(false);
            }

            rotationTimer -= rotationThreshold;
        }

        //clear screen
        SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
        SDL_RenderClear(state.renderer);

        //redraw
        SDL_SetRenderDrawColor(state.renderer, 0, 0, 255, 255);
        currBlock.draw(state.renderer);

        //present
        SDL_RenderPresent(state.renderer);
        prevTime = currTime;

    }
    cleanup(state);
    return 0;
}

void cleanup(SDLState &state) {
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    SDL_Quit();
}

