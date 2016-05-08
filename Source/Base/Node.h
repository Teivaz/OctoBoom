//
//  Node.h
//  OctoBoom
//
//  Created by teivaz on 2/9/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#pragma once
#include "Common.h"
#include <list>

struct Input;

/* Node
 * Node is a basic object in the game.
 * Similar to cocos2dx node
 */
class Node{
public:
    
    // TODO: Improve memory model with refcounter, GC, etc
    Node();
    virtual ~Node();
    
    virtual void Tick(float dt);
    
// Node manipulation
    virtual void SetPosition(const vec2f& position);
    virtual void SetPosition(float x, float y);
    virtual void SetScale(const vec2f& scale);
    virtual void SetScale(float scale);
    virtual void SetRotation(float degrees);
    virtual void SetSize(const vec2f& size);
    virtual void SetSize(float w, float h);
    
    virtual const vec2f& GetPosition() const {return m_position;}
    virtual const vec2f& GetScale() const {return m_scale;}
    virtual float GetRotation() const {return m_rotation;}
    virtual const vec2f& GetSize() const {return m_size;}
    
    // Transformation from parent to object
    const mat3f& GetTransformation();
    mat3f GetGlobalTransformation();
    
// Node hierarchy
    virtual void AddChild(Node* child);
    virtual void RemoveChild(Node* child);
    virtual void SetParent(Node* parent);
    std::list<Node*>& GetChildren() {return m_children;}
    
// Rendering
    virtual void Draw(class Renderer&);
    
// Input
    virtual void OnTouchBegin(const Input&) {}
    virtual void OnTouchMove(const Input&) {}
    virtual void OnTouchEnd(const Input&) {}
    
    virtual bool IsPointInside(const vec2f& point);
    
    virtual bool IsEnabled() const {return m_enabled;}
    virtual void SetEnabled(bool v) {m_enabled = v;}
    
protected:
    virtual void UpdateTransformation();
    virtual bool IsTransformationDirty() const;
    
protected:
    
    std::list<Node*> m_children;
    Node* m_parent;
    
    vec2f m_position;
    float m_rotation;
    vec2f m_scale;
    vec2f m_size;
    
    bool m_transformationDirty;
    mat3f m_transformation;
    bool m_enabled;
};
