//
//  GameController.cpp
//  OctoBoom
//
//  Created by teivaz on 2/10/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#include "GameController.h"
#include "Node.h"
#include "Renderer.h"

GameController* GameController::s_instance = nullptr;

GameController::GameController()
: m_screenWidth(320)
, m_screenHeight(240)
, m_surface(nullptr)
, m_timeSinceLastUpdate(0)
, m_tickInterval(33) // 30 fps
, m_rootNode(nullptr)
, m_renderer(nullptr)
, m_isRunning(true)
{
}

GameController::~GameController()
{
    SDL_SetEventFilter(nullptr, nullptr);
    delete m_renderer;
    delete m_rootNode;
}

GameController& GameController::GetInstance()
{
    if(s_instance == nullptr)
    {
        s_instance = new GameController();
    }
    return *s_instance;
}

void GameController::DestroyInstance()
{
    delete s_instance;
    s_instance = nullptr;
}

int GameController::EventFilter(void* instance, SDL_Event* event)
{
    GameController* self = (GameController*)instance;
    return (int)(self->m_eventFilter.find(event->type) != self->m_eventFilter.end());
}

void GameController::Init(
          int width, int height,
          SDL_Window* window
          )
{
    delete m_renderer;
    m_renderer = new Renderer(window);
    
    SetEventHandler(SDL_QUIT, [this](const SDL_Event& event){
        // TODO: notify exit
        m_isRunning = false;
    });
    SetEventHandler(SDL_MOUSEBUTTONDOWN, [this](const SDL_Event& event){
        m_inputController.OnMouseButtonDown(&event.button);
    });
    SetEventHandler(SDL_MOUSEMOTION, [this](const SDL_Event& event){
        m_inputController.OnMouseMove(&event.motion);
    });
    SetEventHandler(SDL_MOUSEBUTTONUP, [this](const SDL_Event& event){
        m_inputController.OnMouseButtonUp(&event.button);
    });
    
    SDL_SetEventFilter(&GameController::EventFilter, this);
}

void GameController::SetFPS(float fps)
{
    if(fps <= 0)
        fps = 30;
    m_tickInterval = 1000/fps;
}

void GameController::SetEventHandler(Uint32 type, const std::function<void(const SDL_Event&)>& handler)
{
    m_eventFilter.insert(type);
    m_eventHandlers[type] = handler;
}

void GameController::HandleEvent(const SDL_Event& event)
{
    auto iterator = m_eventHandlers.find(event.type);
    if(iterator != m_eventHandlers.end() && iterator->second)
    {
        iterator->second(event);
    }
}

void GameController::SetRootNode(Node* root)
{
    delete m_rootNode;
    m_rootNode = root;
}

void GameController::Tick(int32_t time)
{
    if(m_rootNode)
    {
        m_rootNode->Tick(time);
    }
}

void GameController::Draw()
{
    if(m_rootNode && m_renderer)
    {
        m_renderer->BeginRender();
        m_rootNode->Draw(*m_renderer);
        m_renderer->EndRender();
    }
}

void GameController::UnsbscribeAll(Node* n)
{ 
    UnsubscribeInput(n);
}

void GameController::SubscribeInput(Node* n)
{
    if(n == nullptr)
        return;
    
    m_inputController.Subscribe(n);
}

void GameController::UnsubscribeInput(Node* n)
{
    if(n == nullptr)
        return;
    
    m_inputController.Unsubscribe(n);
}
