//
//  FieldStates.h
//  OctoBoom
//
//  Created by teivaz on 2/14/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#pragma once
#include "Field.h"
#include "../Base/Tween.h"
#include <list>
#include <set>

class Cell;

class FieldState
{
public:
    // Will allocate memory
    static FieldState* Create(Field* f);
    
    FieldState(Field* f) : m_field(f) {}
    virtual ~FieldState() {}
    virtual void OnTouchBegin(const Input&) {}
    virtual void OnTouchMove(const Input&) {}
    virtual void OnTouchEnd(const Input&) {}
    virtual void Tick(float dt) {}
    
    // Will allocate memory if returns not nullptr;
    virtual FieldState* NextState() {return nullptr;}
    
protected:
    Field* m_field;
    
};

class FieldStateFill : public FieldState
{
public:
    typedef Tween<vec2f, EaseSinIn>  MoveTween;
    FieldStateFill(Field* f);
    virtual void Tick(float dt) override;
    virtual FieldState* NextState() override;
private:
    std::list<MoveTween> m_tweens;
};

class FieldStateEnsureSolvable : public FieldState
{
public:
    FieldStateEnsureSolvable(Field* f);
    virtual void Tick(float dt) override;
    virtual FieldState* NextState() override;
};

class FieldStateIdle : public FieldState
{
public:
    FieldStateIdle(Field* f);
    // Wait for user input
    virtual void OnTouchBegin(const Input&) override;
    virtual FieldState* NextState() override;
private:
    CellContainer* m_first;
};

class FieldStateWaitForSwipe : public FieldState
{
public:
    FieldStateWaitForSwipe(Field*, CellContainer*);
    virtual void OnTouchMove(const Input&) override;
    virtual void OnTouchEnd(const Input&) override;
    virtual FieldState* NextState() override;
private:
    CellContainer* m_first;
    CellContainer* m_second;
    bool m_ended;
};

class FieldStateWaitForOther : public FieldState
{
public:
    FieldStateWaitForOther(Field*, CellContainer*);
    virtual void OnTouchBegin(const Input&) override;
    virtual FieldState* NextState() override;
private:
    CellContainer* m_first;
    CellContainer* m_second;
};

class FieldStateValidateOther : public FieldState
{
public:
    FieldStateValidateOther(Field*, CellContainer*, CellContainer*);
    virtual void OnTouchEnd(const Input&) override;
    virtual FieldState* NextState() override;
private:
    CellContainer* m_first;
    CellContainer* m_second;
    bool m_confirm;
};

class FieldStateSwap : public FieldState
{
public:
    typedef Tween<vec2f, EaseSinInOut>  MoveTween;
    FieldStateSwap(Field*, CellContainer*, CellContainer*);
    virtual void Tick(float dt) override;
    virtual FieldState* NextState() override;
protected:
    void MoveCell(Cell* c, const vec2f& from, const vec2f& to);
    bool HasMatch();
    CellContainer* m_first;
    CellContainer* m_second;
    std::list<MoveTween> m_tweens;
};

class FieldStateSwapBack : public FieldStateSwap
{
public:
    FieldStateSwapBack(Field*, CellContainer*, CellContainer*);
    virtual FieldState* NextState() override;
};

class FieldStateDestroyMatch : public FieldState
{
public:
    FieldStateDestroyMatch(Field*, CellContainer*, CellContainer*);
    virtual void Tick(float dt) override;
    virtual FieldState* NextState() override;
private:
    CellContainer* m_first;
    CellContainer* m_second;
};


class FieldStateDrop : public FieldState
{
public:
    typedef Tween<vec2f, EaseSinIn>  MoveTween;
    FieldStateDrop(Field*);
    virtual void Tick(float dt) override;
    virtual FieldState* NextState() override;
private:
    void DropColumn(CellContainer* lower);
    CellContainer* FindUpperNonEmpty(CellContainer* c);
    void Swap(CellContainer*, CellContainer* c);
    void MoveCell(Cell* c, const vec2f& from, const vec2f& to);
    std::list<MoveTween> m_tweens;
};
