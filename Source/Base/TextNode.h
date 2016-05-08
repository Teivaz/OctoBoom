//
//  TextNode.h
//  OctoBoom
//
//  Created by teivaz on 2/16/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#pragma once
#include "Common.h"
#include "Node.h"
#include <memory.h>
#include <string>

class Image;
typedef std::shared_ptr<Image> ImagePtr;

class TextNode : public Node
{
public:
    TextNode(const std::string& font);
    
    void SetText(const std::string& text);
    void SetColor(const Color3c& color);
    
    void Draw(class Renderer&) override;
    
private:
    void RenderText(const std::string& text);
    
    ImagePtr m_renderedText;
    std::string m_text;
    ImagePtr m_font;
    bool m_textDirty;
    Color3c m_color;
};
