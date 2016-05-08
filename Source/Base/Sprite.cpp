//
//  Sprite.cpp
//  OctoBoom
//
//  Created by teivaz on 2/10/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#include "Sprite.h"
#include "Renderer.h"
#include "Image.h"
#include "GameController.h"

Sprite::Sprite()
{}

bool Sprite::LoadImage(const std::string& image)
{
    m_image = GameController::GetInstance().GetImageCache().LoadImage(image);
    if(m_image)
    {
        m_size = m_image->GetSize();
        return true;
    }
    else
    {
        m_size = vec2f();
        return false;
    }
}

void Sprite::Draw(Renderer& renderer)
{
    if(!IsEnabled())
        return;
    
    mat3f t = GetGlobalTransformation();
    bool scaled = GetScale() != vec2f(1);
    renderer.DrawImage(m_image, t, scaled);
    
    Node::Draw(renderer);
}
