//
//  Cell.cpp
//  OctoBoom
//
//  Created by teivaz on 2/11/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#include "Cell.h"
#include <vector>

const float s_wiggleRadius = 0.6;
const float s_wiggleFrequency = 0.02;

std::string Cell::s_files[(uint)Color::NUM] =
{
    "Red.png",
    "Green.png",
    "Blue.png",
    "Purple.png",
    "Yellow.png",
};

Cell::Cell(Cell::Color color)
: m_color(color)
, m_highlight(false)
, m_time(0)
{
    LoadImage(s_files[(int)m_color]);
}

Cell::Cell()
: m_color(Color::None)
, m_highlight(false)
, m_time(0)
{
    m_color = (Color)(rand() % (int)Color::NUM);
    LoadImage(s_files[(int)m_color]);
}

Cell::Cell(const std::set<Color>& colorsToAvoid)
: m_color(Color::None)
, m_highlight(false)
, m_time(0)
{
    int32_t colors = (int32_t)Color::NUM - (int32_t)colorsToAvoid.size();

    // If no colors to avoid create any color
    // If all colors should be avoided screw them and create any color
    if(colorsToAvoid.size() == 0 || colors <= 0)
    {
        m_color = (Color)(rand() % (int)Color::NUM);
        LoadImage(s_files[(int)m_color]);
        return;
    }
    
    std::vector<Color> validColors;
    validColors.reserve(colors);
    for(int32_t c = 0; c < int32_t(Color::NUM); ++c)
    {
        if(colorsToAvoid.find((Color)c) == colorsToAvoid.cend())
            validColors.push_back((Color)c);
    }
    int32_t color = rand() % colors;
    m_color = validColors[color];
    LoadImage(s_files[(int)m_color]);
}

bool Cell::Match(Cell *other) const
{
    return m_color != Color::None && m_color == other->m_color;
}

void Cell::Highlight(bool enable)
{
    m_highlight = enable;
    if(!m_highlight)
        SetPosition(0, 0);
}

void Cell::Tick(float dt)
{
    if(m_highlight)
    {
        m_time += dt;
        SetPosition(
                    s_wiggleRadius*sin(4.1*s_wiggleFrequency*m_time/M_PI),
                    s_wiggleRadius*cos(3.3*s_wiggleFrequency*m_time/M_PI));
    }
}
