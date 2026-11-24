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
    int width = 800;
    int height = 600;
    state.window = SDL_CreateWindow("SDL3 Demo", width, height, 0);
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
    SDL_FRect *rect = new SDL_FRect{150, -50, 50, 50};
    const bool *key_states = SDL_GetKeyboardState(nullptr);
    const double displacement = 50.0f;
    const double fallThreshold = 2.0f;

    double movementTimer = 0;
    double fallTimer = 0;

    //initial direction movement
    bool initialMove = false;
    bool running = true;
    Uint64 prevTime = SDL_GetPerformanceCounter();
    while (running) {
        Uint64 currTime = SDL_GetPerformanceCounter();
        Uint64 frequency = SDL_GetPerformanceFrequency();
        double dt = (double(currTime) - double(prevTime))/double(frequency);
        SDL_Event event{0};
        //handle events
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    switch (event.key.scancode) {
                        case SDL_SCANCODE_A:
                            if (!initialMove) {
                                rect->x -= 50.0f;
                                initialMove = true;
                                movementTimer = 0;
                            }
                            break;
                        case SDL_SCANCODE_D:
                            if (!initialMove) {
                                rect->x += 50.0f;
                                initialMove = true;
                                movementTimer = 0;
                            }
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_EVENT_KEY_UP:
                    if (event.key.scancode == SDL_SCANCODE_A || event.key.scancode == SDL_SCANCODE_D) {
                        initialMove = false;
                    }
                    break;
                default:
                    break;
            }
        }

        //update state

        if (initialMove) {
            movementTimer += dt;
        }

        fallTimer += dt;

        if (initialMove && movementTimer >= 0.5f) {

            if (key_states[SDL_SCANCODE_A]) {
                rect->x -= displacement;
            } else if (key_states[SDL_SCANCODE_D]) {
                rect->x += displacement;
            }

            movementTimer -= 0.5f;
        }

        if (fallTimer >= fallThreshold) {
            if (rect->y < 550) {
                rect->y += displacement;
                fallTimer -= fallThreshold;
            } else {
                rect->y = -50;
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

