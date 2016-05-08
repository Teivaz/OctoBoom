//
//  JsonParser.cpp
//  OctoBoom
//
//  Created by teivaz on 2/12/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#include "JsonParser.h"
#include <stdlib.h>

JsonValue::JsonValue()
: m_string()
, m_array()
, m_object()
, m_number(0)
, m_type(Type::None)
{}

void JsonValue::SetString()
{
    m_type = Type::String;
}

void JsonValue::SetArray()
{
    m_type = Type::Array;
}

void JsonValue::SetObject()
{
    m_type = Type::Object;
}

void JsonValue::SetNumber()
{
    m_type = Type::Number;
}

bool JsonParser::Parse(const std::string& jsonString)
{
    if(jsonString.size() == 0)
        return false;
    
    std::string::const_iterator it = jsonString.cbegin();
    m_end = jsonString.cend();
    
    m_root.reset(new JsonValue());
    
    bool rootSet = Parse(it, m_root);
    
    return rootSet;
}

bool JsonParser::Parse(std::string::const_iterator& i, JsonValueRef destination)
{
    trimSpaces(i);
    if(i == m_end) return false;
    bool parsed = false;
    if(*i == '{')
    {
        parsed = ParseObject(i, destination);
    }
    else if(*i == '[')
    {
        parsed = ParseArray(i, destination);
    }
    else if(*i == '"')
    {
        parsed = ParseString(i, destination);
    }
    else if(IsNumber(*i))
    {
        parsed = ParseNumber(i, destination);
    }
    return parsed;
}

bool JsonParser::IsNumber(char a)
{
    if( (a >= '0' && a <='9') ||
       (a == '.') ||
       (a == '+') ||
       (a == '-'))
        return true;
    return false;
}

void JsonParser::trimSpaces(std::string::const_iterator& i)
{
    while(i != m_end && (*i == ' ' || *i == '\t' || *i == '\n'))
    {
        ++i;
    }
}

bool JsonParser::ReadQuotedString(std::string::const_iterator& i, std::string& out)
{
    trimSpaces(i);
    if(i == m_end) return false;
    if(*i != '"') return false;
    ++i;
    if(i == m_end) return false;
    std::string::const_iterator end = i;
    while(end != m_end && *end != '"')
    {
        ++end;
    }
    if(end == m_end) return false;
    out = std::string(i, end);
    i = ++end;
    return true;
}

bool JsonParser::ParseArray(std::string::const_iterator& i, JsonValueRef destination)
{
    ++i; // [
    destination->SetArray();
    while(i != m_end)
    {
        trimSpaces(i);
        if(i == m_end) return false;
        if(*i == ']') {++i; return true;}
        JsonValueRef value(new JsonValue());
        if(!Parse(i, value)) return false;
        destination->m_array.push_back(value);
        trimSpaces(i);
        if(i == m_end) return false;
        if(*i == ',') ++i;
    }
    return true;
}

bool JsonParser::ParseObject(std::string::const_iterator& i, JsonValueRef destination)
{
    ++i; // {
    destination->SetObject();
    while(i != m_end)
    {
        trimSpaces(i);
        if(i == m_end) return false;
        if(*i == '}') {++i; return true;}
        std::string key;
        if(!ReadQuotedString(i, key) ) return false;
        trimSpaces(i);
        if(i == m_end) return false;
        if(*i != ':') return false;
        ++i;
        JsonValueRef value(new JsonValue());
        if(!Parse(i, value)) return false;
        destination->m_object[key] = value;
        trimSpaces(i);
        if(i == m_end) return false;
        if(*i == ',') ++i;
    }
    return true;
}

bool JsonParser::ParseString(std::string::const_iterator& i, JsonValueRef destination)
{
    destination->SetString();
    return ReadQuotedString(i, destination->m_string);
}

bool JsonParser::ParseNumber(std::string::const_iterator& i, JsonValueRef destination)
{
    char* end = nullptr;
    destination->SetNumber();
    destination->m_number = strtod(&(*i), &end);
    while(&(*++i) != end);
    return true;
}
