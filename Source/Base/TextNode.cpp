//
//  TextNode.cpp
//  OctoBoom
//
//  Created by teivaz on 2/16/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#include "TextNode.h"
#include "GameController.h"
#include "Renderer.h"
#include "Image.h"

static const uint32_t s_symbolWidth = 46;
static const uint32_t s_symbolHeight = 70;
static const uint32_t s_symbolsPerLine = 13;

TextNode::TextNode(const std::string& font)
: m_renderedText()
, m_text()
, m_font()
, m_textDirty(true)
, m_color()
{
    m_font = GameController::GetInstance().GetImageCache().LoadImage(font);
}

void TextNode::SetColor(const Color3c& color)
{
    if(m_color != color)
    {
        m_color = color;
        m_textDirty = true;
    }
}

void TextNode::SetText(const std::string& text)
{
    if(text != m_text)
    {
        m_text = text;
        m_textDirty = true;
    }
}

void TextNode::Draw(class Renderer& r)
{
    if(!IsEnabled())
        return;
    
    if(m_textDirty)
    {
        RenderText(m_text);
        m_textDirty = false;
    }
    if(m_renderedText)
    {
        mat3f t = GetGlobalTransformation();
        r.DrawImage(m_renderedText, t);
    }
    
    Node::Draw(r);
}

void CalculateSymboldRect(SDL_Rect* outRect, char a)
{
    outRect->x = 0;
    outRect->y = 0;
    outRect->w = 0;
    outRect->h = 0;
    char start = 0;
    int row = 0;
    int column = 0;
    if('a' <= a && a <= 'z')
    {
        start = 'a';
        column = (a - start) % s_symbolsPerLine;
        row = (a - start) / s_symbolsPerLine;
    }
    else if('A' <= a && a <= 'Z')
    {
        start = 'A';
        column = (a - start) % s_symbolsPerLine;
        row = (a - start) / s_symbolsPerLine + 2;
    }
    else if('0' <= a && a <= '9')
    {
        start = '0';
        column = (a - start) % s_symbolsPerLine;
        row = (a - start) / s_symbolsPerLine + 4;
    }
    if(start)
    {
        outRect->x = column * s_symbolWidth;
        outRect->y = row * s_symbolHeight;
        outRect->w = s_symbolWidth;
        outRect->h = s_symbolHeight;
    }
}

void CalculateSize(int32_t& w, int32_t& h, const std::string& s)
{
    int32_t currentWidth = 1;
    for(char i : s)
    {
        if(i == '\n')
        {
            w = fmax(currentWidth, w);
            currentWidth = 0;
            ++h;
        }
        else
        {
            ++currentWidth;
            w = fmax(currentWidth, w);
        }
    }
}

void TextNode::RenderText(const std::string& text)
{
    if(!m_font)
        return;
    // TODO: Optimize to use surface proportional to node scale
    // TODO: with OpenGL we can draw it in one pass directly to the scene
    int32_t width = 1;
    int32_t height = 1;
    CalculateSize(width, height, text);
    width *= s_symbolWidth;
    height *= s_symbolHeight;
    SetSize(width, height); // TODO: override GetSize to calculate it properly before text is updated
    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
    SDL_SetSurfaceColorMod(surface, m_color.x, m_color.y, m_color.z);
    SDL_Rect dest = {0, 0, s_symbolWidth, s_symbolHeight};
    SDL_Rect symbolRect;
    for(char i : text)
    {
        if(i == '\n')
        {
            dest.x = 0;
            dest.y += s_symbolHeight;
        }
        else
        {
            CalculateSymboldRect(&symbolRect, i);
            SDL_BlitSurface(m_font->GetSurface(), &symbolRect, surface, &dest);
            dest.x += s_symbolWidth;
        }
    }
    m_renderedText.reset(new Image(surface, vec2i(width, height)));
    SDL_FreeSurface(surface);
}
