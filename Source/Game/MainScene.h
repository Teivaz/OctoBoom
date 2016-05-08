//
//  MainScene.hpp
//  OctoBoom
//
//  Created by teivaz on 2/11/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#pragma once

#include "../Base/Node.h"

class TextNode;
class Field;

class MainScene : public Node
{
public:
    MainScene();
    
    void Tick(float dt) override;
    void OnTouchEnd(const Input&) override;
    
    void Start();
    void EndGame();
private:
    void UpdateTimeNode();
    void UpdateScoreNode();
private:
    uint32_t m_score;
    TextNode* m_scoreNode;
    int32_t m_time;
    int32_t m_timeSeconds;
    TextNode* m_timeNode;
    TextNode* m_gameOverNode;
    Field* m_field;
    TextNode* m_restartNode;
};
