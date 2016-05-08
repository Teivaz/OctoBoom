//
//  ImageCache.h
//  OctoBoom
//
//  Created by teivaz on 2/13/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#pragma once
#include "Common.h"
#include <map>
#include <string>

class Image;
typedef std::shared_ptr<Image> ImagePtr;

class ImageCache
{
public:
    ImageCache();
    
    void SetResourcesPath(const std::string& path) {m_resourcesPath = path;}
    void LoadAtlas(const std::string& config);
    ImagePtr LoadImage(const std::string& name);
    
    void FreeUnused();
private:
    ImagePtr LoadImageFromFile(const std::string& file);
    
private:
    std::map<std::string, ImagePtr> m_cache;
    std::string m_resourcesPath;
};
