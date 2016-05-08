//
//  Field.hpp
//  OctoBoom
//
//  Created by teivaz on 2/11/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#pragma once
#include "../Base/Node.h"
#include <vector>
#include <set>

class CellContainer;
class FieldState;
class TextNode;

class Field : public Node
{
public:
    Field();
    ~Field();
    
    void InitWithSize(ushort width, ushort height, const vec2f& cellSize, const vec2f& cellMargin);
    
    CellContainer* At(ushort x, ushort y);
    
    virtual void OnTouchBegin(const Input&) override;
    virtual void OnTouchMove(const Input&) override;
    virtual void OnTouchEnd(const Input&) override;
    
    ushort GetWidth() const {return m_width;}
    ushort GetHeight() const {return m_height;}
    
    // Probability to create cells that can match in one move
    void SetDifficulty(float value) {m_difficulty = value;}
    float GetDifficulty() const {return m_difficulty;}
    
    void Tick(float dt) override;
    
    ushort MinimalMatch() const;
    const vec2f& GetCellStride() const {return m_cellStride;}
    
    bool HasMatch(CellContainer*) const;
    bool HasMatch() const;
    void FindMatch(std::set<CellContainer*>& outMatch) const;
    void FindMatch(CellContainer*, std::set<CellContainer*>& outMatch) const;
    
    bool HasOneMoveMatch() const;
    bool HasOneMoveMatch(CellContainer*) const;
    void MakeOneMoveMatch();
    
    uint32_t GetScore() const {return m_score;}
    
    void OnCellsDestroyed(uint32_t cells);
    
private:
    void PurgeField();
    
private:
    typedef std::vector<CellContainer*> CellContainerColumn;
    typedef std::vector<CellContainerColumn> CellContainerField;
    ushort m_width;
    ushort m_height;
    
    CellContainerField m_field;
    float m_difficulty; // Probability of generating a one move match
    FieldState* m_state;
    vec2f m_cellStride;
    uint32_t m_score;
};
