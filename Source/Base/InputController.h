//
//  InputController.h
//  OctoBoom
//
//  Created by teivaz on 2/14/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#pragma once
#include "Common.h"
#include "Input.h"
#include <set>

class Node;

class InputController
{
public:
    InputController();
    ~InputController();
    
    void OnMouseButtonDown(const SDL_MouseButtonEvent* button);
    void OnMouseMove(const SDL_MouseMotionEvent* motion);
    void OnMouseButtonUp(const SDL_MouseButtonEvent* button);
  
    void Subscribe(Node*);
    void Unsubscribe(Node*);
    
private:
    void LockListeners();
    void UnlockListeners();
    
private:
    bool m_capturing;
    Input m_input;
    
    std::set<Node*> m_listeners;
    std::set<Node*> m_pendingAddListeners;
    std::set<Node*> m_pendingRemoveListeners;
    bool m_listeneresLocked;
};
