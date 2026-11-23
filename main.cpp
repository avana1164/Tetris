#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>


using namespace std;

struct SDLState {
    SDL_Window *window;
    SDL_Renderer *renderer;
};

void cleanup(SDLState &state);
int direction_user_should_move(SDL_FRect *rect);

int main(int argc, char *argv[]) {

    SDLState state;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error initializing SDL3", nullptr);
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
    bool running = true;
    int fall_rate = 200;
    int curr_counter = 0;
    SDL_FRect *rect = new SDL_FRect{150, -50, 50, 50};
    while (running) {
        SDL_Event event{0};
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                {
                    running = false;
                    break;
                }
            } 
        }

        //perform drawing commands
        SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
        SDL_RenderClear(state.renderer);
        
        
        // SDL_SetRenderDrawColor(state.renderer, 0, 255, 0, 255);
        // SDL_RenderRect(state.renderer, rect);
        SDL_SetRenderDrawColor(state.renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(state.renderer, rect);
        // swap buffers and present
        SDL_RenderPresent(state.renderer);
        if (curr_counter == fall_rate - 1) {
            rect->y += 50;
            direction_user_should_move(rect);
        }
        if (rect->y >= 600) rect->y = -50;
        
        curr_counter = (curr_counter + 1)%fall_rate;

    }
    cleanup(state);
    return 0;
}

int direction_user_should_move(SDL_FRect *rect) {
    const bool *key_states = SDL_GetKeyboardState(NULL);
    int direction = 0;

    if (key_states[SDL_SCANCODE_A]) {
        rect->x -= 50;
    }

    if (key_states[SDL_SCANCODE_D]) {
        rect->x += 50;
    }

    return direction;
}

void cleanup(SDLState &state) {
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    SDL_Quit();
}

