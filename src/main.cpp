#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <list>

const int WINDOW_WIDTH {180*4};
const int WINDOW_HEIGHT {180 *3};


class Sprite {
public:
    Sprite();
    Sprite(SDL_Renderer* p_renderer, const char* p_fileName, SDL_Rect p_textureRect,
           int p_numFrames ,int p_scale=1);
    ~Sprite();

    SDL_Texture* getTexture();
    void sliceSpriteSheet();

    friend std::ostream& operator<<( std::ostream& os, const Sprite& s);

    //int scale;
    int scale;
private:
    SDL_Rect spriteRect;
    SDL_Texture* spriteTexture;
    int numOfFrames;
    std::list<SDL_Rect> frameList;
};

// Default Constructor
Sprite::Sprite() {}

/* 
    params:
        p_renderer: 
            the renderer sprite will be drawn to.
        
        p_fileName:
            filename of the texture

        p_rect: 
            The size of the loaded texture

        p_numFrames:
            The number of frames in texture

        p_scale:
        The scale of the sprite. Default value is 1.
*/
Sprite::Sprite(SDL_Renderer* p_renderer, const char* p_fileName, SDL_Rect p_rect,
                int p_numFrames, int p_scale) {
    numOfFrames = p_numFrames;
    scale = p_scale;
    spriteRect = p_rect;
    spriteTexture = IMG_LoadTexture(p_renderer, p_fileName);
    spriteRect = {0, 0, p_rect.w * scale, p_rect.h * scale};
    sliceSpriteSheet();

}

// Returns Sprites's texture
SDL_Texture* Sprite::getTexture() {
    return spriteTexture;
}

void Sprite::sliceSpriteSheet() {


    for (int i = 0; i < numOfFrames; i++) {
        SDL_Rect newRect {i * spriteRect.w / numOfFrames,
                          spriteRect.y,
                          spriteRect.w,
                          spriteRect.h};

        frameList.emplace_back(newRect);
    }
}

// Displays Sprite information in a readable format
std::ostream& operator<<( std::ostream &os,const Sprite &s) {
    os << "=================================" <<  
            "\nScale: " << s.scale <<  "\nNumber of Frames: " << s.numOfFrames << std::endl;
            std::cout << "\nFrame Information";
            int i = 0;
            for (auto item : s.frameList) {
                std::cout << "\nFrame " << i << " Origin: (" << item.x << ", " << item.y << ") Rect Size: (" << item.w << ", " << item.h << ")";
                i++;
            }
            std::cout << "\n=================================" << std::endl;
    return os;
}


Sprite::~Sprite() {}




bool successfulInit();


int main() {
    
    if(!successfulInit()) {
        std::cout << "Initialization has failed somewhere." << std::endl;
    }
    else {
        std::cout << "SDL has been initialized." << std::endl;
    }

    SDL_Window* window {nullptr};
    SDL_Rect windowRect {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_Renderer* renderer {nullptr};

    window = SDL_CreateWindow("SDL Template",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    
    renderer = SDL_CreateRenderer(window, -1, 0);


    SDL_Rect testRect {32, 32, 32, 32};  // Ended up being destination source

    Sprite* simonSprite = new Sprite(renderer, "../res/simon-sprites.png", {0, 0, 160, 32}, 5, 3);
    std::cout << *simonSprite;


    bool isRunning {true};
    SDL_Event event;

    while (isRunning) {
        // Getting Input
        SDL_PollEvent(&event);

        if (event.type == SDL_QUIT) {
            isRunning = false;
        }

        // Draw to screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        
        SDL_RenderCopy(renderer, simonSprite->getTexture(), NULL, &testRect);
        //SDL_RenderCopy(renderer, spriteTexture, &spriteFrame1 , &testRect);


        SDL_RenderPresent(renderer);
    }


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}

bool successfulInit() {
    bool success {true};

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Failed to initialize SDL Video." << std::endl;
        std::cout << "SDL Error: " << SDL_GetError() << std::endl;

        success = false;
    }

    if (IMG_Init(IMG_INIT_JPG) == 0) {
        std::cout << "Failed to initialize Image module." << std::endl;
        std::cout << "IMG Error: " << IMG_GetError() << std::endl;

        success = false;
    }

    return success;
}