//
//  Image.h
//  OctoBoom
//
//  Created by teivaz on 2/13/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#pragma once
#include "Common.h"

class Image
{
public:
    Image(SDL_Surface* surface, const vec2i& size, const vec2i& offset = vec2i());
    ~Image();
    
    SDL_Surface* GetSurface() {return m_surf;}
    const vec2i& GetOffset() const {return m_offset;}
    const vec2i& GetSize() const {return m_size;}
    const SDL_Rect* GetRect() const {return &m_rect;}
private:
    SDL_Surface* m_surf;
    SDL_Rect m_rect;
    vec2i m_offset;
    vec2i m_size;
};
