//
//  FieldStates.cpp
//  OctoBoom
//
//  Created by teivaz on 2/14/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#include "FieldStates.h"
#include "../Base/Input.h"
#include "CellContainer.h"

// time cells will swap
const float k_swapTime = 400;

// time cells will fall, ms per cell
const float k_dropTime = 150;

// time to fall from spawn position
const float k_spawnDropTime = 250;


// Initial state
FieldState* FieldState::Create(Field* f)
{
    return new FieldStateFill(f);
}

/*
 * FieldStateFill
 */

FieldStateFill::FieldStateFill(Field* f) : FieldState(f)
{
    float spawnHeight = m_field->GetPosition().y + m_field->GetCellStride().y;
    for(Node* n : m_field->GetChildren())
    {
        CellContainer* c = (CellContainer*)n;
        if(c->GetCell() == nullptr)
        {
            c->CreateCellWithMoveMatchProbablity(m_field->GetDifficulty(), m_field->MinimalMatch());
            Cell* cell = c->GetCell();
            vec2f spawnPoint(0, -c->GetPosition().y - spawnHeight);
            cell->SetPosition(spawnPoint);
            m_tweens.push_back(MoveTween([=](const vec2f& p){cell->SetPosition(p);}, spawnPoint, vec2f(), k_spawnDropTime));
        }
    }
}

void FieldStateFill::Tick(float dt)
{
    auto i = m_tweens.begin();
    auto end = m_tweens.end();
    while(i != end)
    {
        auto tween = i;
        ++i;
        tween->Tick(dt);
        if(tween->IsFinished())
            m_tweens.erase(tween);
    }
}

FieldState* FieldStateFill::NextState()
{
    if(m_tweens.size() > 0)
        return nullptr;
    else if(m_field->HasMatch())
        return new FieldStateDestroyMatch(m_field, nullptr, nullptr);
    else
        return new FieldStateEnsureSolvable(m_field);
}

/*
 * FieldStateEnsureSolvable
 */

FieldStateEnsureSolvable::FieldStateEnsureSolvable(Field* f)
: FieldState(f)
{}

void FieldStateEnsureSolvable::Tick(float dt)
{
    if(!m_field->HasOneMoveMatch())
    {
        m_field->MakeOneMoveMatch();
    }
}

FieldState* FieldStateEnsureSolvable::NextState()
{
    return new FieldStateIdle(m_field);
}

/*
 * FieldStateIdle
 */

FieldStateIdle::FieldStateIdle(Field* f)
: FieldState(f)
, m_first(nullptr)
{}

void FieldStateIdle::OnTouchBegin(const Input& i)
{
    for(Node* n : m_field->GetChildren())
    {
        if(n->IsPointInside(i.Start))
            m_first = (CellContainer*)n;
    }
}

FieldState* FieldStateIdle::NextState()
{
    if(m_first)
        return new FieldStateWaitForSwipe(m_field, m_first);
    else
        return nullptr;
}

/*
 * FieldStateWaitForSwipe
 */

FieldStateWaitForSwipe::FieldStateWaitForSwipe(Field* f, CellContainer* c)
: FieldState(f)
, m_first(c)
, m_second(nullptr)
, m_ended(false)
{}

void FieldStateWaitForSwipe::OnTouchMove(const Input& i)
{
    const float threshold = 10;
    vec2f delta = i.Current - i.Start;
    if(fabs(delta.x) > fabs(delta.y))
    {
        if(fabs(delta.x) > threshold)
            m_second = (delta.x > 0) ? m_first->m_right : m_first->m_left;
    }
    else
    {
        if(fabs(delta.y) > threshold)
            m_second = (delta.y > 0) ? m_first->m_down : m_first->m_up;
    }
}

void FieldStateWaitForSwipe::OnTouchEnd(const Input&)
{
    m_ended = true;
}

FieldState* FieldStateWaitForSwipe::NextState()
{
    if(m_second)
        return new FieldStateSwap(m_field, m_first, m_second);
    else if(m_ended)
        return new FieldStateWaitForOther(m_field, m_first);
    else
        return nullptr;
}

/*
 * FieldStateWaitForOther
 */

FieldStateWaitForOther::FieldStateWaitForOther(Field* f, CellContainer* c)
: FieldState(f)
, m_first(c)
, m_second(nullptr)
{
    m_first->HighlightCell(true);
}

void FieldStateWaitForOther::OnTouchBegin(const Input& i)
{
    if(m_first == nullptr)
        return;
    if(m_first->m_left && m_first->m_left->IsPointInside(i.Start))
        m_second = m_first->m_left;
    else if(m_first->m_right && m_first->m_right->IsPointInside(i.Start))
        m_second = m_first->m_right;
    else if(m_first->m_up && m_first->m_up->IsPointInside(i.Start))
        m_second = m_first->m_up;
    else if(m_first->m_down && m_first->m_down->IsPointInside(i.Start))
        m_second = m_first->m_down;
    else
    {
        m_first->HighlightCell(false);
        m_first = nullptr;
    }
}

FieldState* FieldStateWaitForOther::NextState()
{
    if(m_second && m_first)
        return new FieldStateValidateOther(m_field, m_first, m_second);
    else if(m_first)
        return nullptr;
    else
        return new FieldStateIdle(m_field);}

/*
 * FieldStateValidateOther
 */

FieldStateValidateOther::FieldStateValidateOther(Field* f, CellContainer* c1, CellContainer* c2)
: FieldState(f)
, m_first(c1)
, m_second(c2)
, m_confirm(false)
{}

void FieldStateValidateOther::OnTouchEnd(const Input& i)
{
    if(m_second && m_second->IsPointInside(i.Current))
    {
        m_confirm = true;
    }
    else
    {
        m_second = nullptr;
    }
}

FieldState* FieldStateValidateOther::NextState()
{
    if(m_second == nullptr)
        return new FieldStateWaitForOther(m_field, m_first);
    else if(m_confirm)
    {
        m_first->HighlightCell(false);
        return new FieldStateSwap(m_field, m_first, m_second);
    }
    else
        return nullptr;
}

FieldStateSwap::FieldStateSwap(Field* f, CellContainer* c1, CellContainer* c2)
: FieldState(f)
, m_first(c1)
, m_second(c2)
, m_tweens()
{
    m_first->SwapCells(m_second);
    vec2f delta = m_first->GetPosition() - m_second->GetPosition();
    
    MoveCell(m_first->GetCell(), -delta, vec2f());
    MoveCell(m_second->GetCell(), delta, vec2f());
}

void FieldStateSwap::MoveCell(Cell* c, const vec2f& from, const vec2f& to)
{
    if(c == nullptr)
        return;
    c->SetPosition(from);
    m_tweens.push_back(MoveTween([=](const vec2f& p){c->SetPosition(p);}, from, to, k_swapTime));
}

void FieldStateSwap::Tick(float dt)
{
    auto i = m_tweens.begin();
    auto end = m_tweens.end();
    while(i != end)
    {
        auto tween = i;
        ++i;
        tween->Tick(dt);
        if(tween->IsFinished())
            m_tweens.erase(tween);
    }
}

bool FieldStateSwap::HasMatch(){
    if(m_first)
    {
        return (m_field->HasMatch(m_first) || m_field->HasMatch(m_second));
    }
    else
    {
        return m_field->HasMatch();
    }
    return false;
}

FieldState* FieldStateSwap::NextState()
{
    if(m_tweens.size() > 0)
    {
        return nullptr;
    }
    else if(HasMatch())
    {
        return new FieldStateDestroyMatch(m_field, m_first, m_second);
    }
    else
    {
        return new FieldStateSwapBack(m_field, m_first, m_second);
    }
}

/*
 * FieldStateSwapBack
 */

FieldStateSwapBack::FieldStateSwapBack(Field* f, CellContainer* c1, CellContainer* c2)
: FieldStateSwap(f, c1, c2)
{}

FieldState* FieldStateSwapBack::NextState()
{
    if(m_tweens.size() > 0)
    {
        return nullptr;
    }
    else
    {
        return new FieldStateIdle(m_field);
    }
}

/*
 * FieldStateDestroyMatch
 */

FieldStateDestroyMatch::FieldStateDestroyMatch(Field* f, CellContainer* c1, CellContainer* c2)
: FieldState(f)
, m_first(c1)
, m_second(c2)
{}

void FieldStateDestroyMatch::Tick(float dt)
{
    std::set<CellContainer*> toDestroy;
    if(m_first && m_second)
    {
        m_field->FindMatch(m_first, toDestroy);
        m_field->FindMatch(m_second, toDestroy);
    }
    else
    {
        m_field->FindMatch(toDestroy);
    }
    
    m_field->OnCellsDestroyed((uint32_t)toDestroy.size());
    
    for(CellContainer* c : toDestroy)
    {
        c->DestroyCell();
    }
}

FieldState* FieldStateDestroyMatch::NextState()
{
    return new FieldStateDrop(m_field);
}

/* 
 * FieldStateDrop
 */

FieldStateDrop::FieldStateDrop(Field* f)
: FieldState(f)
, m_tweens()
{
    // Grab bottom left
    CellContainer* lower = m_field->At(0, m_field->GetHeight()-1);
    if(lower == nullptr)
        return;
    
    while(lower)
    {
        DropColumn(lower);
        lower = lower->m_right;
    }
}

void FieldStateDrop::Swap(CellContainer* c1, CellContainer* c2)
{
    c1->SwapCells(c2);
    vec2f delta = c1->GetPosition() - c2->GetPosition();
    
    MoveCell(c1->GetCell(), -delta, vec2f());
    MoveCell(c2->GetCell(), delta, vec2f());
}

void FieldStateDrop::MoveCell(Cell* c, const vec2f& from, const vec2f& to)
{
    if(c == nullptr)
        return;
    c->SetPosition(from);
    float cellsToMove = fabs(from.y - to.y) / m_field->GetCellStride().y;
    cellsToMove = sqrtf(cellsToMove);
    float dropTime = k_dropTime * cellsToMove;
    m_tweens.push_back(MoveTween([=](const vec2f& p){c->SetPosition(p);}, from, to, dropTime));
}

CellContainer* FieldStateDrop::FindUpperNonEmpty(CellContainer* c)
{
    while(c)
    {
        if(c->GetCell())
            return c;
        c = c->m_up;
    }
    return nullptr;
}

void FieldStateDrop::DropColumn(CellContainer* lower)
{
    while(lower)
    {
        if(lower->GetCell() == nullptr)
        {
            CellContainer* upper = FindUpperNonEmpty(lower);
            if(upper)
                Swap(lower, upper);
            else
            {
                return; // everything is empty
            }
        }
        lower = lower->m_up;
    }
}

void FieldStateDrop::Tick(float dt)
{
    auto i = m_tweens.begin();
    auto end = m_tweens.end();
    while(i != end)
    {
        auto tween = i;
        ++i;
        tween->Tick(dt);
        if(tween->IsFinished())
            m_tweens.erase(tween);
    }
}

FieldState* FieldStateDrop::NextState()
{
    if(m_tweens.size() > 0)
        return nullptr;
    else
        return new FieldStateFill(m_field);
}
