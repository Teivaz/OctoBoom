//
//  Cell.h
//  OctoBoom
//
//  Created by teivaz on 2/11/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#pragma once
#include "../Base/Sprite.h"
#include "../Base/Tween.h"
#include <string>
#include <set>

class Cell : public Sprite
{
public:
    enum class Color{
        None = -1,
        Red,
        Green,
        Blue,
        Purple,
        Yellow,
        
        NUM
    };
    
    Cell(Color color);
    
    // Random color
    Cell();
    
    // Random with colors that don't match
    Cell(const std::set<Color>& colorsToAvoid);
    
    bool Match(Cell* other) const;
    
    Color GetColor() const {return m_color;}
    
    void Highlight(bool enable);
    
    void Tick(float dt) override;
private:
    Color m_color;
    static std::string s_files[(uint)Color::NUM];
    bool m_highlight;
    float m_time;
};
