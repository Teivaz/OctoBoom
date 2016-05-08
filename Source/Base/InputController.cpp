//
//  InputController.cpp
//  OctoBoom
//
//  Created by teivaz on 2/14/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#include "InputController.h"
#include <iostream>
#include "Node.h"

InputController::InputController()
: m_capturing(false)
, m_input()
, m_listeners()
, m_pendingAddListeners()
, m_pendingRemoveListeners()
, m_listeneresLocked(false)
{}

InputController::~InputController()
{}

void InputController::OnMouseButtonDown(const SDL_MouseButtonEvent* button)
{
    if(button->button != SDL_BUTTON_LEFT)
    {
        return;
    }
    if(m_capturing)
    {
        // this shouldn't happen but anyways
        OnMouseButtonUp(button);
    }
    m_capturing = true;
    m_input.Start = m_input.Current = vec2f(button->x, button->y);
    
    LockListeners();
    for(Node* l : m_listeners)
    {
        l->OnTouchBegin(m_input);
    }
    UnlockListeners();
}

void InputController::OnMouseMove(const SDL_MouseMotionEvent* motion)
{
    if(!m_capturing)
    {
        return;
    }
    m_input.Current = vec2f(motion->x, motion->y);
    
    LockListeners();
    for(Node* l : m_listeners)
    {
        l->OnTouchMove(m_input);
    }
    UnlockListeners();
}

void InputController::OnMouseButtonUp(const SDL_MouseButtonEvent* button)
{
    if(button->button != SDL_BUTTON_LEFT)
    {
        return;
    }
    if(!m_capturing)
    {
        return;
    }
    m_capturing = false;
    m_input.Current = vec2f(button->x, button->y);
    
    LockListeners();
    for(Node* l : m_listeners)
    {
        l->OnTouchEnd(m_input);
    }
    UnlockListeners();
}

void InputController::LockListeners()
{
    m_listeneresLocked = true;
}

void InputController::UnlockListeners()
{
    m_listeneresLocked = false;
    for (Node* l : m_pendingAddListeners)
    {
        m_listeners.insert(l);
    }
    m_pendingAddListeners.clear();
    
    for(Node* l : m_pendingRemoveListeners)
    {
        m_listeners.erase(l);
    }
    m_pendingRemoveListeners.clear();
}

void InputController::Subscribe(Node* l)
{
    if(!m_listeneresLocked)
    {
        m_listeners.insert(l);
    }
    else
    {
        m_pendingRemoveListeners.erase(l);
        m_pendingAddListeners.insert(l);
    }
}

void InputController::Unsubscribe(Node* l)
{
    if(!m_listeneresLocked)
    {
        m_listeners.erase(l);
    }
    else
    {
        m_pendingAddListeners.erase(l);
        m_pendingRemoveListeners.insert(l);
    }
}
