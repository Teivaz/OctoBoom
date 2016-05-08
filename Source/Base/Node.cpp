//
//  Node.cpp
//  OctoBoom
//
//  Created by teivaz on 2/9/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#include "Node.h"
#include "GameController.h"

Node::Node()
: m_children()
, m_parent(nullptr)
, m_position()
, m_rotation(0)
, m_scale(1)
, m_size()
, m_transformationDirty(true)
, m_transformation()
, m_enabled(true)
{}

Node::~Node()
{
    GameController::GetInstance().UnsbscribeAll(this);
    for(Node* c : m_children)
    {
        delete c;
    }
}

void Node::Tick(float dt)
{
    for(auto child : m_children)
    {
        child->Tick(dt);
    }
}

void Node::AddChild(Node* child)
{
    if(child == this)
        return;
    
    if(child == nullptr)
        return;
    
    if(child->m_parent)
    {
        SDL_assert(false);
        return;
    }
    child->SetParent(this);
    m_children.push_back(child);
}

void Node::RemoveChild(Node* child)
{
    if(child)
    {
        m_children.remove(child);
        child->SetParent(nullptr);
    }
}

void Node::SetParent(Node* parent)
{
    m_parent = parent;
}

void Node::Draw(Renderer& r)
{
    for(auto child : m_children)
    {
        child->Draw(r);
    }
}

void Node::SetPosition(float x, float y)
{
    SetPosition({x, y});
}

void Node::SetPosition(const vec2f& position)
{
    if(m_position != position)
    {
        m_position = position;
        m_transformationDirty = true;
    }
}

void Node::SetScale(const vec2f& scale)
{
    if(m_scale != scale)
    {
        m_scale = scale;
        m_transformationDirty = true;
    }
}

void Node::SetScale(float scale)
{
    SetScale(vec2f(scale));
}

void Node::SetRotation(float degrees)
{
    if(m_rotation != degrees)
    {
        m_rotation = degrees;
        m_transformationDirty = true;
    }
}

void Node::SetSize(const vec2f& size)
{
    m_size = size;
}

void Node::SetSize(float w, float h)
{
    SetSize({w, h});
}

bool Node::IsTransformationDirty() const
{
    return m_transformationDirty;
}

void Node::UpdateTransformation()
{
    mat3f translation;
    translation.SetTranslation(m_position);
    mat3f rot;
    rot.SetRotatation(m_rotation);
    mat3f scale;
    scale.SetScale(m_scale);
    m_transformation = translation * rot * scale;
}

const mat3f& Node::GetTransformation()
{
    if(IsTransformationDirty())
    {
        UpdateTransformation();
        m_transformationDirty = false;
    }
    return m_transformation;
}

mat3f Node::GetGlobalTransformation()
{
    mat3f result = GetTransformation();
    Node* parent = m_parent;
    while(parent)
    {
        result = parent->GetTransformation() * result;
        parent = parent->m_parent;
    }
    return result;
}

bool Node::IsPointInside(const vec2f& point)
{
    mat3f t = GetGlobalTransformation();
    vec2f a = t.GetTranslationPart();
    vec2f b = GetSize();
    b = vectry::TransformOuter(t, b) + a;
    return (point.x >= a.x && point.x <= b.x && point.y >= a.y && point.y <= b.y);
    
}
