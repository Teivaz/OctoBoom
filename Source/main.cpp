//
//  main.cpp
//  OctoBoom
//
//  Created by teivaz on 2/9/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#include <iostream>
#include "Base/Common.h"
#include "Base/GameController.h"
#include "Game/MainScene.h"

const int SCREEN_WIDTH = 755;
const int SCREEN_HEIGHT = 600;
const int FPS = 30;

int main(int argc, const char * argv[])
{
    SDL_Window* window = nullptr;
    
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Failed to initialize SDL";
        return 0;
    }

    window = SDL_CreateWindow("Octo Boom", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == nullptr)
    {
        std::cout << "Failed to initialize SDL";
        return 0;
    }
    
    GameController& controller = GameController::GetInstance();
    
    controller.Init(SCREEN_WIDTH, SCREEN_HEIGHT, window);
    controller.SetFPS(FPS);
    controller.SetRootNode(new MainScene());
    SDL_Event event;
    uint32_t lastUpdateTime = 0;
    int32_t deltaTime = 0;
    
    while (controller.IsRunning()) {
        uint32_t currentTime = SDL_GetTicks();
        deltaTime = currentTime - lastUpdateTime;
        
        int32_t timeToSleep = controller.GetTickInterval() - deltaTime;
        if(timeToSleep > 0)
        {
            SDL_Delay(timeToSleep);
        }
        
        currentTime = SDL_GetTicks();
        deltaTime = currentTime - lastUpdateTime;
        
        while(SDL_PollEvent(&event))
        {
            controller.HandleEvent(event);
        }
        
        controller.Tick(deltaTime);
        lastUpdateTime = currentTime;
        controller.Draw();
    }
    GameController::DestroyInstance();
    SDL_Quit();
    return 0;
}
