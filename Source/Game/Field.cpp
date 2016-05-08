//
//  Field.cpp
//  OctoBoom
//
//  Created by teivaz on 2/11/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#include "Field.h"
#include "CellContainer.h"
#include "../Base/Input.h"
#include "../Base/GameController.h"
#include "FieldStates.h"
#include "../Base/TextNode.h"

const ushort k_minimalMatch = 3;

Field::Field()
: m_width(0)
, m_height(0)
, m_field()
, m_difficulty(0.2)
, m_state(nullptr)
, m_score(0)
{}

Field::~Field()
{
    PurgeField();
    delete m_state;
}

void Field::InitWithSize(ushort width, ushort height, const vec2f& size, const vec2f& margin)
{
    GameController::GetInstance().SubscribeInput(this);
    
    m_width = width;
    m_height = height;
    
    m_cellStride = size + margin;
    
    // 1. Remove old field
    PurgeField();
    
    // 2. Create containers and place
    m_field.resize(width);
    for(ushort i = 0; i < width; ++i)
    {
        CellContainerColumn& column = m_field[i];
        column.resize(height);
        for(ushort j = 0; j < height; ++j)
        {
            CellContainer* cell = new CellContainer();
            column[j] = cell;
            cell->SetPosition(i*m_cellStride.x, j*m_cellStride.y);
            AddChild(cell);
            cell->SetSize(size);
        }
    }
    
    // 3. Link cell containers to eachother
    for(ushort i = 0; i < width; ++i)
    {
        CellContainerColumn& column = m_field[i];
        for(ushort j = 0; j < height; ++j)
        {
            CellContainer* c = column[j];
            if(i > 0)
                c->m_left = m_field[i-1][j];
            if(i < width-1)
                c->m_right = m_field[i+1][j];
            if(j > 0)
                c->m_up = column[j-1];
            if(j < height-1)
                c->m_down = column[j+1];
        }
    }
    delete m_state;
    m_state = FieldState::Create(this);
}

bool Field::HasMatch(CellContainer* c) const
{
    if(c == nullptr)
        return false;
    
    return c->HasMatch(MinimalMatch());
}

bool Field::HasMatch() const
{
    // Check all cells
    for(Node* n : m_children)
    {
        if( ((CellContainer*)n)->HasMatch(MinimalMatch()) )
            return true;
    }
    return false;
}

void Field::FindMatch(std::set<CellContainer*>& outMatch) const
{
    for(Node* n : m_children)
    {
        ((CellContainer*)n)->FindMatch(MinimalMatch(), outMatch);
    }
}

void Field::FindMatch(CellContainer* c, std::set<CellContainer*>& outMatch) const
{
    if(c == nullptr)
        return;
    c->FindMatch(MinimalMatch(), outMatch);
}

bool Field::HasOneMoveMatch(CellContainer* c) const
{
    if(c == nullptr)
        return false;
    return c->HasOneMoveMatch(MinimalMatch());
}

bool Field::HasOneMoveMatch() const
{
    // Check all cells
    for(Node* n : m_children)
    {
        if( ((CellContainer*)n)->HasOneMoveMatch(MinimalMatch()) )
            return true;
    }
    return false;
}

void Field::MakeOneMoveMatch()
{
    bool solvable = false;
    int MAX_ATTEMPTS = 20;
    while(!solvable && MAX_ATTEMPTS-- > 0)
    {
        int x = rand() % GetWidth();
        int y = rand() % GetHeight();
        CellContainer* c = At(x, y);
        solvable = c->ChangeColorToSolveInOneMove(MinimalMatch());
    }
    SDL_assert(solvable);
}

void Field::OnCellsDestroyed(uint32_t cells)
{
    m_score += cells * 20;
}

void Field::PurgeField()
{
    for(auto& column : m_field)
    {
        for(auto& cell : column)
        {
            RemoveChild(cell);
            delete cell;
        }
        column.resize(0);
    }
    m_field.resize(0);
}

CellContainer* Field::At(ushort x, ushort y)
{
    if(x < m_field.size())
    {
        auto& column = m_field[x];
        if(y < column.size())
        {
            return column[y];
        }
    }
    return nullptr;
}

void Field::Tick(float dt)
{
    if(m_state)
    {
        m_state->Tick(dt);
        FieldState* newState = m_state->NextState();
        if(newState)
        {
            delete m_state;
            m_state = newState;
        }
    }
    Node::Tick(dt);
}

ushort Field::MinimalMatch() const
{
    return k_minimalMatch;
}

void Field::OnTouchBegin(const Input& i)
{
    if(m_state)
        m_state->OnTouchBegin(i);
}

void Field::OnTouchMove(const Input& i)
{
    if(m_state)
        m_state->OnTouchMove(i);
}

void Field::OnTouchEnd(const Input& i)
{
    if(m_state)
        m_state->OnTouchEnd(i);
}
