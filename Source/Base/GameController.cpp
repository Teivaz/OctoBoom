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
{}

GameController::~GameController()
{
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

void GameController::Init(
          int width, int height,
          SDL_Window* window
          )
{
    delete m_renderer;
    m_renderer = new Renderer(window);
}

void GameController::SetFPS(float fps)
{
    if(fps <= 0)
        fps = 30;
    m_tickInterval = 1000/fps;
}

void GameController::HandleEvent(const SDL_Event& event)
{
    if(event.type == SDL_QUIT)
    {
        // TODO: notify exit
        m_isRunning = false;
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        m_inputController.OnMouseButtonDown(&event.button);
    }
    else if(event.type == SDL_MOUSEMOTION)
    {
        m_inputController.OnMouseMove(&event.motion);
    }
    else if(event.type == SDL_MOUSEBUTTONUP)
    {
        m_inputController.OnMouseButtonUp(&event.button);
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
