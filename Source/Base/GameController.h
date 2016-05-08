//
//  GameController.h
//  OctoBoom
//
//  Created by teivaz on 2/10/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#pragma once
#include "Common.h"
#include "ImageCache.h"
#include "InputController.h"

class Node;
class Renderer;

class GameController
{
    GameController();
public:
    static GameController& GetInstance();
    static void DestroyInstance();
    
    ~GameController();
    
    void Init(
              int width, int height,
              SDL_Window* window
              );
    
    void SetRootNode(Node* root);
    
    void Tick(int32_t time);
    void Draw();
    void HandleEvent(const SDL_Event& event);
    
    bool IsRunning() const {return m_isRunning;}
    void SetFPS(float fps);
    bool ShouldTick(int32_t deltaTime) const {return deltaTime >= m_tickInterval;}
    
    ImageCache& GetImageCache() {return m_imageCache;}
    
    void UnsbscribeAll(Node*);
    
    void SubscribeInput(Node*);
    void UnsubscribeInput(Node*);
    
private:
    int m_screenWidth;
    int m_screenHeight;
    
    SDL_Surface* m_surface;
    int32_t m_timeSinceLastUpdate;
    int32_t m_tickInterval;
    
    Node* m_rootNode;
    Renderer* m_renderer;
    
    bool m_isRunning;
    
    ImageCache m_imageCache;
    InputController m_inputController;
    
    static GameController* s_instance;
};
