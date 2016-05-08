//
//  Renderer.cpp
//  OctoBoom
//
//  Created by teivaz on 2/10/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#include "Renderer.h"
#include "Image.h"

//TDOD: Use renderer

Renderer::Renderer(SDL_Window* window)
: m_window(window)
{
    m_surface = SDL_GetWindowSurface(m_window);
    //SDL_SetSurfaceBlendMode(m_surface, SDL_BLENDMODE_BLEND);
}

void Renderer::BeginRender()
{
    // clear screen
    SDL_FillRect(m_surface, nullptr, 0xff000000);
}

void Renderer::DrawImage(ImagePtr image, const mat3f& transformation, bool scaled /*= true*/)
{
    if(!image)
        return;
    vec2f size = vectry::TransformOuter(transformation, vec2f(image->GetSize()));
    vec2f position = transformation.GetTranslationPart();
    SDL_Rect dest;
    dest.x = roundf(position.x);
    dest.y = roundf(position.y);
    dest.w = roundf(size.x);
    dest.h = roundf(size.y);
    if(scaled)
        SDL_BlitScaled(image->GetSurface(), image->GetRect(), m_surface, &dest);
    else
        SDL_BlitSurface(image->GetSurface(), image->GetRect(), m_surface, &dest);
}

void Renderer::EndRender()
{
    // present the surface
    SDL_UpdateWindowSurface(m_window);
}
