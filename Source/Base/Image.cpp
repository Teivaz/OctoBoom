//
//  Image.cpp
//  OctoBoom
//
//  Created by teivaz on 2/13/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#include "Image.h"

Image::Image(SDL_Surface* surface, const vec2i& size, const vec2i& offset /*= vec2i()*/)
: m_surf(surface)
, m_size(size)
, m_offset(offset)
{
    if(m_surf)
    {
        m_surf->refcount++;
    }
    m_rect.x = m_offset.x;
    m_rect.y = m_offset.y;
    m_rect.w = m_size.x;
    m_rect.h = m_size.y;
}

Image::~Image()
{
    SDL_FreeSurface(m_surf);
}
