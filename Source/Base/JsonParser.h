//
//  JsonParser.h
//  OctoBoom
//
//  Created by teivaz on 2/12/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#pragma once
#include <memory>
#include <vector>
#include <string>
#include <map>

class JsonParser;
class JsonValue;
typedef std::shared_ptr<JsonValue> JsonValueRef;

class JsonValue
{
public:
    typedef std::map<std::string, JsonValueRef> Object;
    typedef std::vector<JsonValueRef> Array;
    enum class Type
    {
        None,
        String,
        Array,
        Object,
        Number
    };
    
    JsonValue();
    
    const std::string& GetString() const {return m_string;}
    const Object& GetObject() const {return m_object;}
    const Array& GetArray() const {return m_array;}
    double GetNumber() const {return m_number;}
    
    bool IsString() const {return m_type == Type::String;}
    bool IsArray() const {return m_type == Type::Array;}
    bool IsObject() const {return m_type == Type::Object;}
    bool IsNumber() const {return m_type == Type::Number;}
    
private:
    friend JsonParser;
    
    void SetString();
    void SetArray();
    void SetObject();
    void SetNumber();
    
private:
    std::string m_string;
    Array m_array;
    Object m_object;
    double m_number;
    Type m_type;
};

class JsonParser
{
public:
    bool Parse(const std::string& jsonString);
    
    JsonValueRef GetRootObject() const {return m_root;}
    
private:
    
    bool IsNumber(char);
    void trimSpaces(std::string::const_iterator& i);
    bool ReadQuotedString(std::string::const_iterator& i, std::string& out);
    
    bool Parse(std::string::const_iterator& i, JsonValueRef destination);
    
    bool ParseArray(std::string::const_iterator& i, JsonValueRef destination);
    bool ParseObject(std::string::const_iterator& i, JsonValueRef destination);
    bool ParseString(std::string::const_iterator& i, JsonValueRef destination);
    bool ParseNumber(std::string::const_iterator& i, JsonValueRef destination);
    
private:
    JsonValueRef m_root;
    std::string::const_iterator m_end;
};
