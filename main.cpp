#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

struct SDLState {
    SDL_Window *window;
    SDL_Renderer *renderer;
};

void cleanup(SDLState &state);

int main(int argc, char *argv[]) {

    SDLState state;

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

    // start the game loop
    const bool *key_states = SDL_GetKeyboardState(nullptr);
    const double displacement = 50.0f;
    const double fallThreshold = 2.0f;
    const double moveThreshold = 0.125f;
    const float BLOCK_WIDTH = 50.0f;


    SDL_FRect *rect = new SDL_FRect{150, 0, BLOCK_WIDTH, BLOCK_WIDTH};

    double movementTimer = 0;
    double fallTimer = 0;

    //initial direction movement
    bool initialLeft = false;
    bool initialRight = false;
    bool initialDown = false;
    bool running = true;
    Uint64 prevTime = SDL_GetPerformanceCounter();
    while (running) {
        Uint64 currTime = SDL_GetPerformanceCounter();
        Uint64 frequency = SDL_GetPerformanceFrequency();
        double dt = (double(currTime) - double(prevTime))/double(frequency);
        bool initialMove = false;
        SDL_Event event{0};
        //handle events
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    initialMove = initialLeft || initialRight || initialDown;
                    switch (event.key.scancode) { 
                        case SDL_SCANCODE_A:
                            if (!initialMove && rect->x > 0) {
                                rect->x -= displacement;
                                initialLeft = true;
                                movementTimer = 0;
                            }
                            break;
                        case SDL_SCANCODE_D:
                            if (!initialMove && rect->x < 750) {
                                rect->x += displacement;
                                initialRight = true;
                                movementTimer = 0;
                            }
                            break;
                        case SDL_SCANCODE_S:
                            if (!initialMove && rect->y < 550) {
                                rect->y += displacement;
                                initialDown = true;
                                movementTimer = 0;
                            } else if (rect->y >= 550) {
                                rect->y = 0;
                            }
                        default:
                            break;
                    }
                    break;
                case SDL_EVENT_KEY_UP:
                    if (event.key.scancode == SDL_SCANCODE_A) {
                        initialLeft = false;
                    }
                        
                    if (event.key.scancode == SDL_SCANCODE_D) {
                        initialRight = false;
                    }

                    if (event.key.scancode == SDL_SCANCODE_S) {
                        initialDown = false;
                    }
                    break;
                default:
                    break;
            }
        }

        //update state

        initialMove = initialLeft || initialRight || initialDown;

        if (initialMove) movementTimer += dt;

        if (!initialDown) fallTimer += dt;

        if ((initialLeft || initialRight) && movementTimer >= moveThreshold) {
            if (key_states[SDL_SCANCODE_A] && initialLeft && rect->x > 0) {
                rect->x -= displacement;
            } else if (key_states[SDL_SCANCODE_D] && initialRight && rect->x < 750) {
                rect->x += displacement;
            }

            movementTimer -= moveThreshold;
        } else if (initialDown && movementTimer >= moveThreshold && key_states[SDL_SCANCODE_S]) {
            if (rect->y < 550) {
                rect->y += displacement;
                movementTimer -= moveThreshold;
            } else {
                rect->y = 0;
                movementTimer = 0;
            }
        }

        //automated falling
        if (!initialDown && fallTimer >= fallThreshold) {
            if (rect->y < 550) {
                rect->y += displacement;
                fallTimer -= fallThreshold;
            } else {
                rect->y = 0;
                fallTimer = 0;
            }
        }

        //clear screen
        SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
        SDL_RenderClear(state.renderer);

        //redraw
        SDL_SetRenderDrawColor(state.renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(state.renderer, rect);

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

