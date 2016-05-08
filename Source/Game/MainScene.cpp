//
//  MainScene.cpp
//  OctoBoom
//
//  Created by teivaz on 2/11/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#include "MainScene.h"
#include "Field.h"
#include "../Base/GameController.h"
#include "../Base/Sprite.h"
#include "../Base/TextNode.h"

const vec2f k_cellSize(35, 35);
const vec2f k_cellMargin(7, 7);
const ushort k_fieldWidth = 8;
const ushort k_fieldHeight = 8;
const uint32_t k_roundTimeMs = 60000;

MainScene::MainScene()
: m_score(0)
, m_scoreNode(nullptr)
, m_time(0)
, m_timeSeconds(0)
, m_timeNode(nullptr)
, m_gameOverNode(nullptr)
, m_field(nullptr)
, m_restartNode(nullptr)
{
    GameController::GetInstance().GetImageCache().SetResourcesPath("Resources");
    GameController::GetInstance().GetImageCache().LoadAtlas("config.json");
    
    Sprite* background = new Sprite();
    background->LoadImage("BackGround.jpg");
    AddChild(background);
    
    GameController::GetInstance().SubscribeInput(this);
    
    Color3c fontColor(0xff, 0xcc, 0);
    
    m_scoreNode = new TextNode("teletype.bmp");
    AddChild(m_scoreNode);
    m_scoreNode->SetPosition(80, 95);
    m_scoreNode->SetColor(fontColor);
    m_scoreNode->SetText("score\n00000");
    m_scoreNode->SetScale(0.5);
    
    m_timeNode = new TextNode("teletype.bmp");
    AddChild(m_timeNode);
    m_timeNode->SetPosition(80, 195);
    m_timeNode->SetText("00:00");
    m_timeNode->SetColor(fontColor);
    m_timeNode->SetScale(0.5);
    
    m_gameOverNode = new TextNode("teletype.bmp");
    AddChild(m_gameOverNode);
    m_gameOverNode->SetPosition(380, 220);
    m_gameOverNode->SetColor(fontColor);
    m_gameOverNode->SetScale(0.5);
    
    m_restartNode = new TextNode("teletype.bmp");
    AddChild(m_restartNode);
    m_restartNode->SetText("Restart");
    m_restartNode->SetPosition(60, 160);
    m_restartNode->SetScale(0.5);
    m_restartNode->SetColor(fontColor);
    
    Start();
}

void MainScene::EndGame()
{
    RemoveChild(m_field);
    delete m_field;
    m_field = nullptr;
    
    m_timeNode->SetEnabled(false);
    m_scoreNode->SetEnabled(false);
    m_gameOverNode->SetEnabled(true);
    m_restartNode->SetEnabled(true);
    
    char buffer[6];
    sprintf(buffer, "%05d", m_score);
    std::string gameOver = "Game Over\n  Score\n  ";
    gameOver += buffer;
    m_gameOverNode->SetText(gameOver);
}

void MainScene::Start()
{
    RemoveChild(m_field);
    delete m_field;
    
    m_timeNode->SetEnabled(true);
    m_scoreNode->SetEnabled(true);
    m_gameOverNode->SetEnabled(false);
    m_restartNode->SetEnabled(false);
    
    m_field = new Field();
    AddChild(m_field);
    m_field->SetPosition(330, 105);
    m_field->InitWithSize(k_fieldWidth, k_fieldHeight, k_cellSize, k_cellMargin);
    m_time = k_roundTimeMs;
    
    UpdateTimeNode();
    UpdateScoreNode();
}

void MainScene::UpdateTimeNode()
{
    int32_t time = m_time / 1000;
    if(m_timeSeconds != time)
    {
        m_timeSeconds = time;
        char buffer[6];
        sprintf(buffer, "%02d:%02d", time / 60, time % 60);
        m_timeNode->SetText(buffer);
    }
}

void MainScene::UpdateScoreNode()
{
    if(m_field)
    {
        if(m_field->GetScore() == m_score)
            return;
        m_score = m_field->GetScore();
        
        char buffer[30];
        sprintf(buffer, "score\n%05d", m_score);
        
        m_scoreNode->SetText(buffer);
    }
}

void MainScene::Tick(float dt)
{
    if(m_time > 0)
    {
        m_time -= dt;
        if(m_time <= 0)
        {
            m_time = 0;
            EndGame();
        }
        UpdateTimeNode();
        UpdateScoreNode();
    }
    Node::Tick(dt);
}

void MainScene::OnTouchEnd(const Input& i)
{
    if(m_time == 0 && m_restartNode->IsPointInside(i.Current))
    {
        Start();
    }
}
