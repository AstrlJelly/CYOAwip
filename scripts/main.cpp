#include "main.hpp"

int main(int argc, char* argv[])
{
    std::string beginNode = EpicContext::loadSavedNode();

    // if save file didn't exist, check cli args
    // if not cli args, get default beginning node
    if (beginNode.length() <= 0) {
        beginNode = argc > 1 ? std::string(argv[1]) : BEGIN_NODE;
    }

    fs::path epicPath = fs::path(EPICS_PATH);
    epicPath += START_EPIC;
    epicPath += NODES_PATH;

    Epic* epic = new Epic(epicPath,beginNode);

    SDL_Window* window = nullptr;
    initSDL(window);

    //while(true)
    //{
    //    bool keepRunning = epic->executeCurrentNode();
    //    if(!keepRunning)
    //    {
    //        break;
    //    }
    //}

    while(true)
    {
        update();
    }

    return 0;
}

void initSDL(SDL_Window* window)
{
    SDL_SetMainReady();

    bool initSuccess = SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_GAMEPAD);
    if(!initSuccess) {
        const char* error = SDL_GetError();
        std::cerr << "SDL_Init failed: \n" << error << '\n';
        SDL_Quit();
        exit(0);
    }
    
    window = SDL_CreateWindow(WINDOW_TITLE.c_str(),WINDOW_WIDTH,WINDOW_HEIGHT,SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
    if(window == NULL) {
        const char* error = SDL_GetError();
        std::cerr << "Couldn't set video mode: \n" << error << '\n';
        exit(0);
    }
}

// runs every frame
void update()
{
    SDL_Event e;
    if(SDL_PollEvent(&e))
    {
        if(e.key.key == SDLK_P)
        {
            exit(0);
        }
    }
}