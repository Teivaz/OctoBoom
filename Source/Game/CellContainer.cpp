//
//  CellContainer.cpp
//  OctoBoom
//
//  Created by teivaz on 2/11/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#include "CellContainer.h"

CellContainer::CellContainer()
: m_left(nullptr)
, m_right(nullptr)
, m_up(nullptr)
, m_down(nullptr)
, m_cell(nullptr)
{
}

bool CellContainer::Match(const CellContainer* other) const
{
    return (other && (other != this) && other->HasCell() && HasCell() && m_cell->Match(other->m_cell));
}
void CellContainer::CreateRandomCell()
{
    DestroyCell();
    m_cell = new Cell();
    AddChild(m_cell);
}

void CellContainer::CreateCellWithMoveMatchProbablity(float p, ushort minimalMatch)
{
    SDL_assert(m_cell == nullptr);
    std::set<Cell::Color> colorsToAvoid;
    bool shouldMatchInOneMove = rand() % 100 < p * 100;
    if(!shouldMatchInOneMove)
    {
        // TODO: check if can create matching structure:
        /*
         *      X
         *  X X 0 X X
         *      X
         */
        minimalMatch -= 1;
    }
    CalculateColorsToAvoid(colorsToAvoid, minimalMatch);
    m_cell = new Cell(colorsToAvoid);
    AddChild(m_cell);
}

void CellContainer::CreateNonMatchingCell(ushort minimalMatch)
{
    SDL_assert(m_cell == nullptr);
    std::set<Cell::Color> colorsToAvoid;
    CalculateColorsToAvoid(colorsToAvoid, minimalMatch);
    m_cell = new Cell(colorsToAvoid);
    AddChild(m_cell);
}

void CellContainer::CreateCell(Cell::Color color)
{
    DestroyCell();
    m_cell = new Cell(color);
    AddChild(m_cell);
}

void CellContainer::DestroyCell()
{
    if(m_cell)
    {
        RemoveChild(m_cell);
        delete m_cell;
        m_cell = nullptr;
    }
}

void CellContainer::CalculateColorsToAvoid(std::set<Cell::Color>& colorsToAvoid, ushort minimalMatch)
{
    minimalMatch -= 1; // decrease minimum so we don't match it
    
    // Left and right
    if(m_left && m_left->Match(m_right) && (m_left->GetNumLeftMatch() + m_right->GetNumRightMatch() + 2) >= minimalMatch)
    {
        colorsToAvoid.insert(m_left->GetColor());
    }
    else
    {
        // Left only
        if(m_left && (m_left->GetNumLeftMatch()+1) >= minimalMatch)
            colorsToAvoid.insert(m_left->GetColor());
        // Right only
        if(m_right && (m_right->GetNumRightMatch()+1) >= minimalMatch)
            colorsToAvoid.insert(m_right->GetColor());
    }
    
    // Up and down
    if(m_up && m_up->Match(m_down) && (m_up->GetNumUpMatch() + m_down->GetNumDownMatch() + 2) >= minimalMatch)
    {
        colorsToAvoid.insert(m_up->GetColor());
    }
    else
    {
        // Up only
        if(m_up && (m_up->GetNumUpMatch()+1) >= minimalMatch)
            colorsToAvoid.insert(m_up->GetColor());
        // Down only
        if(m_down && (m_down->GetNumDownMatch()+1) >= minimalMatch)
            colorsToAvoid.insert(m_down->GetColor());
    }
}

bool CellContainer::HasMatch(ushort minimalMatch) const
{
    if(1 + GetNumLeftMatch() + GetNumRightMatch() >= minimalMatch)
    {
        return true;
    }
    if(1 + GetNumUpMatch() + GetNumDownMatch() >= minimalMatch)
    {
        return true;
    }
    return false;
}

void CellContainer::FindMatch(ushort minimalMatch, std::set<CellContainer*>& outMatch)
{
    short left = GetNumLeftMatch();
    short right = GetNumRightMatch();
    short up = GetNumUpMatch();
    short down = GetNumDownMatch();
    
    CellContainer* i = this;
    
    if(1 + left + right >= minimalMatch)
    {
        outMatch.insert(this);
        i = m_left;
        while(--left >= 0 && i)
        {
            outMatch.insert(i);
            i = i->m_left;
        }
        i = m_right;
        while(--right >= 0 && i)
        {
            outMatch.insert(i);
            i = i->m_right;
        }
    }
    if(1 + up + down >= minimalMatch)
    {
        outMatch.insert(this);
        i = m_up;
        while(--up >= 0 && i)
        {
            outMatch.insert(i);
            i = i->m_up;
        }
        i = m_down;
        while (--down >= 0 && i)
        {
            outMatch.insert(i);
            i = i->m_down;
        }
    }
}

bool CellContainer::ChangeColorToSolveInOneMove(ushort minimalMatch)
{
    bool success = false;
    
    // Keep the cell in case we fail
    Cell* cell = m_cell;
    RemoveChild(cell);
    m_cell = nullptr;
    
    if(m_left)
    {
        CreateCell(m_left->GetColor());
        success = HasOneMoveMatch(minimalMatch) && !HasMatch(minimalMatch);
    }
    if(!success && m_right)
    {
        CreateCell(m_right->GetColor());
        success = HasOneMoveMatch(minimalMatch) && !HasMatch(minimalMatch);
    }
    if(!success && m_up)
    {
        CreateCell(m_up->GetColor());
        success = HasOneMoveMatch(minimalMatch) && !HasMatch(minimalMatch);
    }
    if(!success && m_down)
    {
        CreateCell(m_down->GetColor());
        success = HasOneMoveMatch(minimalMatch) && !HasMatch(minimalMatch);
    }
    
    if(success)
    {
        delete cell;
        return true;
    }
    else
    {
        // put cell back and fail
        DestroyCell();
        m_cell = cell;
        AddChild(m_cell);
        return false;
    }
}


/* If we are in the offset (O) of one-move-match
 *
 *      O
 *  B B   B B B B
 *      O
 */
bool CellContainer::HasOneMoveOffsetMatch(ushort minimalMatch) const
{
    if(m_left) // swap left
    {
        short verticalMatch = 1; // this
        // G
        // B G
        // G
        if(Match(m_left->m_up))
        {
            verticalMatch += m_left->m_up->GetNumUpMatch() + 1;
        }
        if(Match(m_left->m_down))
        {
            verticalMatch += m_left->m_down->GetNumDownMatch() + 1;
        }
        if(verticalMatch >= minimalMatch)
            return true;
    }
    if(m_right)
    {
        short verticalMatch = 1;
        if(Match(m_right->m_up))
        {
            verticalMatch += m_right->m_up->GetNumUpMatch() + 1;
        }
        if(Match(m_right->m_down))
        {
            verticalMatch += m_right->m_down->GetNumDownMatch() + 1;
        }
        if(verticalMatch >= minimalMatch)
            return true;
    }
    if(m_up)
    {
        short horizontalMatch = 1;
        if(Match(m_up->m_left))
        {
            horizontalMatch += m_up->m_left->GetNumLeftMatch() + 1;
        }
        if(Match(m_up->m_right))
        {
            horizontalMatch += m_up->m_right->GetNumRightMatch() + 1;
        }
        if(horizontalMatch >= minimalMatch)
            return true;
    }
    if(m_down)
    {
        short horizontalMatch = 1;
        if(Match(m_down->m_left))
        {
            horizontalMatch += m_down->m_left->GetNumLeftMatch() + 1;
        }
        if(Match(m_down->m_right))
        {
            horizontalMatch += m_down->m_right->GetNumRightMatch() + 1;
        }
        if(horizontalMatch >= minimalMatch)
            return true;
    }
    return false;
}

/* If we are in the offset (O) of one-move-match
 *
 *      O
 *  B B   B B B B
 *      O
 */
bool CellContainer::HasOneMoveBaseMatch(ushort minimalMatch) const
{
    ushort horizontalMatch =  1 + GetNumLeftMatch() + GetNumRightMatch();
    if(horizontalMatch >= minimalMatch)
        return true;
    
    ushort verticalMatch =  1 + GetNumUpMatch() + GetNumDownMatch();
    if(verticalMatch >= minimalMatch)
        return true;
    
    const CellContainer* left = GetLeftmostMatch()->m_left;
    if(left)
    {
        if(Match(left->m_up) || Match(left->m_down))
        {
            // we can swap left of the leftmost up or down to match
            if(horizontalMatch + 1 >= minimalMatch)
                return true;
            
            // check if the left can match furter
            if(Match(left->m_left))
            {
                if(horizontalMatch + left->m_left->GetNumLeftMatch() + 2 >= minimalMatch)
                    return true;
            }
        }
        else if(Match(left->m_left))
        {
            // we can only swap with left:
            //  G B G G -> B G G G
            if(horizontalMatch + 1 >= minimalMatch)
                return true;
        }
    }
    
    const CellContainer* right = GetRightmostMatch()->m_right;
    if(right)
    {
        if(Match(right->m_up) || Match(right->m_down))
        {
            if(horizontalMatch + 1 >= minimalMatch)
                return true;
            if(Match(right->m_right))
            {
                if(horizontalMatch + right->m_right->GetNumRightMatch() + 2 >= minimalMatch)
                    return true;
            }
        }
        else if(Match(right->m_right))
        {
            if(horizontalMatch + 1 >= minimalMatch)
                return true;
        }
    }
    
    const CellContainer* up = GetUpmostMatch()->m_up;
    if(up)
    {
        if(Match(up->m_left) || Match(up->m_right))
        {
            if(verticalMatch + 1 >= minimalMatch)
                return true;
            if(Match(up->m_up))
            {
                if(verticalMatch + up->m_up->GetNumUpMatch() + 2 >= minimalMatch)
                    return true;
            }
        }
        else if(Match(up->m_up))
        {
            if(verticalMatch + 1 >= minimalMatch)
                return true;
        }
    }
    
    const CellContainer* down = GetDownmostMatch()->m_down;
    if(down)
    {
        if(Match(down->m_left) || Match(down->m_right))
        {
            if(verticalMatch + 1 >= minimalMatch)
                return true;
            if(Match(down->m_down))
            {
                if(verticalMatch + down->m_down->GetNumDownMatch() + 2 >= minimalMatch)
                    return true;
            }
        }
        else if(Match(down->m_down))
        {
            if(verticalMatch + 1 >= minimalMatch)
                return true;
        }
    }
    return false;
}

bool CellContainer::HasOneMoveMatch(ushort minimalMatch) const
{
    if(HasMatch(minimalMatch))
        return true;
    if(HasOneMoveBaseMatch(minimalMatch))
        return true;
    if(HasOneMoveOffsetMatch(minimalMatch))
        return true;
    return false;
}

void CellContainer::HighlightCell(bool enable)
{
    if(m_cell)
        m_cell->Highlight(enable);
}

Cell::Color CellContainer::GetColor() const
{
    if(m_cell)
        return m_cell->GetColor();
    return Cell::Color::None;
}

// Reuse code by pointer to member template
template<CellContainer* CellContainer::* NEXT>
int32_t GetNumNextMatch(const CellContainer* self)
{
    int32_t result = 0;
    const CellContainer* next = self->*NEXT;
    while(self->Match(next))
    {
        next = next->*NEXT;
        ++result;
    }
    return result;
}

int32_t CellContainer::GetNumLeftMatch() const
{
    return GetNumNextMatch<&CellContainer::m_left>(this);
}
int32_t CellContainer::GetNumRightMatch() const
{
    return GetNumNextMatch<&CellContainer::m_right>(this);
}
int32_t CellContainer::GetNumUpMatch() const
{
    return GetNumNextMatch<&CellContainer::m_up>(this);
}
int32_t CellContainer::GetNumDownMatch() const
{
    return GetNumNextMatch<&CellContainer::m_down>(this);
}

template<CellContainer* CellContainer::* NEXT>
const CellContainer* GetNextmostMatch(const CellContainer* self)
{
    const CellContainer* next = self->*NEXT;
    const CellContainer* match = self;
    while(self->Match(next))
    {
        next = next->*NEXT;
        match = match->*NEXT;
    }
    return match;
}

const CellContainer* CellContainer::GetLeftmostMatch() const
{
    return ::GetNextmostMatch<&CellContainer::m_left>(this);
}
const CellContainer* CellContainer::GetRightmostMatch() const
{
    return ::GetNextmostMatch<&CellContainer::m_right>(this);
}
const CellContainer* CellContainer::GetUpmostMatch() const
{
    return ::GetNextmostMatch<&CellContainer::m_up>(this);
}
const CellContainer* CellContainer::GetDownmostMatch() const
{
    return ::GetNextmostMatch<&CellContainer::m_down>(this);
}


void CellContainer::SwapCells(CellContainer *other)
{
    if(other)
    {
        Cell* c1 = m_cell;
        Cell* c2 = other->m_cell;
        m_cell = c2;
        other->m_cell = c1;
        RemoveChild(c1);
        other->RemoveChild(c2);
        AddChild(c2);
        other->AddChild(c1);
    }
}
