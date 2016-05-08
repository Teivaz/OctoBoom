//
//  Renderer.h
//  OctoBoom
//
//  Created by teivaz on 2/10/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#pragma once
#include "Common.h"

class Image;
typedef std::shared_ptr<Image> ImagePtr;

class Renderer
{
public:
    Renderer(SDL_Window*);
    
    void BeginRender();
    void EndRender();
    
    void DrawImage(ImagePtr image, const mat3f& transformation, bool scaled = true);
    
private:
    SDL_Window* m_window;
    SDL_Surface* m_surface;
};
