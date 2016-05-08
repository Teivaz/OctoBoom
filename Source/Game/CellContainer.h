//
//  CellContainer.h
//  OctoBoom
//
//  Created by teivaz on 2/11/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#pragma once
#include "../Base/Node.h"
#include "Cell.h"
#include <set>

class CellContainer : public Node
{
public:
    CellContainer();
    
    // Will create cell that doesn't match neighbours
    void CreateNonMatchingCell(ushort minimalMatch);
    
    // Will create cell that doesn't match neigbours
    // but allows to match in one move with given probability
    void CreateCellWithMoveMatchProbablity(float p, ushort minimalMatch);
    
    void CreateRandomCell();
    void CreateCell(Cell::Color color);
    
    // Pointers to neigbours
    CellContainer* m_left;
    CellContainer* m_right;
    CellContainer* m_up;
    CellContainer* m_down;
    
    bool HasMatch(ushort minimalMatch) const;
    void FindMatch(ushort minimalMatch, std::set<CellContainer*>& outMatch);
    
    bool HasOneMoveMatch(ushort minimalMatch) const;
    bool ChangeColorToSolveInOneMove(ushort minimalMatch);
    
    // True if other is different container with matching existing cell
    bool Match(const CellContainer* other) const;
    bool HasCell() const {return m_cell != nullptr;}
    void DestroyCell();
    void SwapCells(CellContainer* other);
    Cell* GetCell() {return m_cell;}
    Cell::Color GetColor() const;
    
    void HighlightCell(bool enable);
    
    int32_t GetNumLeftMatch() const;
    int32_t GetNumRightMatch() const;
    int32_t GetNumUpMatch() const;
    int32_t GetNumDownMatch() const;
    
private:
    bool HasOneMoveOffsetMatch(ushort minimalMatch) const;
    bool HasOneMoveBaseMatch(ushort minimalMatch) const;
    
    const CellContainer* GetLeftmostMatch() const;
    const CellContainer* GetRightmostMatch() const;
    const CellContainer* GetUpmostMatch() const;
    const CellContainer* GetDownmostMatch() const;
    
    void CalculateColorsToAvoid(std::set<Cell::Color>& outColors, ushort minimalMatch);
    
private:
    // Contained cell
    Cell* m_cell;
    
};
