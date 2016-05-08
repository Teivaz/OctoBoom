//
//  ImageCache.cpp
//  OctoBoom
//
//  Created by teivaz on 2/13/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#include "ImageCache.h"
#include "JsonParser.h"
#include "Image.h"

ImageCache::ImageCache()
{}

void ImageCache::LoadAtlas(const std::string &config)
{
    std::string configPath = m_resourcesPath + "/" + config;
    SDL_RWops* data = SDL_RWFromFile(configPath.c_str(), "r");
    if(data == nullptr)
    {
        return;
    }
    
    uint64_t size = data->seek(data, 0, RW_SEEK_END);
    data->seek(data, 0, RW_SEEK_SET);
    char* symbols = new char[size+1];
    size_t end = data->read(data, symbols, 1, size);
    data->close(data);
    symbols[end] = 0;
    
    JsonParser parser;
    bool result = parser.Parse(symbols);
    delete[] symbols;
    
    if(result == false)
    {
        return;
    }
    
    const JsonValue::Object& root = parser.GetRootObject()->GetObject();
    std::string atlasName = root.at("atlas")->GetString();
    
    ImagePtr atlas = LoadImageFromFile(atlasName);
    if(atlas == nullptr)
        return;
    
    const JsonValue::Array& sprites = root.at("sprites")->GetArray();
    
    for(auto& sprite : sprites)
    {
        const JsonValue::Object& s = sprite->GetObject();
        std::string name = s.at("name") ? s.at("name")->GetString() : "";
        if(name.empty())
            continue;
        int32_t x = s.at("x") ? s.at("x")->GetNumber() : 0;
        int32_t y = s.at("y") ? s.at("y")->GetNumber() : 0;
        int32_t w = s.at("w") ? s.at("w")->GetNumber() : 0;
        int32_t h = s.at("h") ? s.at("h")->GetNumber() : 0;
        m_cache[name] = ImagePtr(new Image(atlas->GetSurface(), vec2i(w, h), vec2i(x, y)));
    }
}

ImagePtr ImageCache::LoadImage(const std::string& name)
{
    auto found = m_cache.find(name);
    if(found != m_cache.end())
    {
        return found->second;
    }
    else
    {
        return LoadImageFromFile(name);
    }
}

ImagePtr ImageCache::LoadImageFromFile(const std::string& file)
{
    std::string filePath = m_resourcesPath + "/" + file;
    SDL_Surface* surf = SDL_LoadBMP(filePath.c_str());    
    if(surf == nullptr)
        return ImagePtr();
    
    ImagePtr result = m_cache[file] = ImagePtr(new Image(surf, vec2i(surf->w, surf->h)));
    SDL_FreeSurface(surf);
    return result;
}

void ImageCache::FreeUnused()
{
    for(auto i = m_cache.begin(), end = m_cache.end(); i != end;)
    {
        auto image = i++;
        if(image->second.unique())
            m_cache.erase(image);
    }
}
