//
//  Sprite.h
//  OctoBoom
//
//  Created by teivaz on 2/10/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#pragma once
#include "Common.h"
#include "Node.h"

class Image;
typedef std::shared_ptr<Image> ImagePtr;

class Sprite : public Node
{
public:
    Sprite();
    
    bool LoadImage(const std::string& image);
    virtual void Draw(class Renderer&) override;
    
private:
    ImagePtr m_image;
};
