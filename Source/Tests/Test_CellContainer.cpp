//
//  Test_CellContainer.cpp
//  OctoBoom
//
//  Created by teivaz on 2/14/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#include "BaseTest.h"
#include "../Game/CellContainer.h"
#include "../Game/Field.h"


BEGIN_TEST_CATEGORY("CellContainer")

ADD_TEST("cell arrangement and connections", {
    Field f;
    /*
        r g
        b y
     */
    
    f.InitWithSize(2, 2, 1, 1);
    f.At(0, 0)->CreateCell(Cell::Color::Red);
    f.At(1, 0)->CreateCell(Cell::Color::Green);
    f.At(0, 1)->CreateCell(Cell::Color::Blue);
    f.At(1, 1)->CreateCell(Cell::Color::Yellow);

    CellContainer* r = f.At(0, 0);
    CellContainer* g = f.At(1, 0);
    CellContainer* b = f.At(0, 1);
    CellContainer* y = f.At(1, 1);
    
    FAIL_IF_FALSE(f.At(0, 0)->m_left == nullptr);
    FAIL_IF_FALSE(f.At(0, 0)->m_right == g);
    FAIL_IF_FALSE(f.At(0, 0)->m_up == nullptr);
    FAIL_IF_FALSE(f.At(0, 0)->m_down == b);

    FAIL_IF_FALSE(f.At(1, 0)->m_left == r);
    FAIL_IF_FALSE(f.At(1, 0)->m_right == nullptr);
    FAIL_IF_FALSE(f.At(1, 0)->m_up == nullptr);
    FAIL_IF_FALSE(f.At(1, 0)->m_down == y);
    
    FAIL_IF_FALSE(f.At(0, 1)->m_left == nullptr);
    FAIL_IF_FALSE(f.At(0, 1)->m_right == y);
    FAIL_IF_FALSE(f.At(0, 1)->m_up == r);
    FAIL_IF_FALSE(f.At(0, 1)->m_down == nullptr);
    
    FAIL_IF_FALSE(f.At(1, 1)->m_left == b);
    FAIL_IF_FALSE(f.At(1, 1)->m_right == nullptr);
    FAIL_IF_FALSE(f.At(1, 1)->m_up == g);
    FAIL_IF_FALSE(f.At(1, 1)->m_down == nullptr);

    PASS();
})

ADD_TEST("count neighbours", {
    
    /*
        r r r r
        g g r r
        b b g r
        r r r b
     */
    
    Field f;
    f.InitWithSize(4, 4, 1, 1);
    f.At(0, 0)->CreateCell(Cell::Color::Red);
    f.At(1, 0)->CreateCell(Cell::Color::Red);
    f.At(2, 0)->CreateCell(Cell::Color::Red);
    f.At(3, 0)->CreateCell(Cell::Color::Red);
    f.At(0, 1)->CreateCell(Cell::Color::Green);
    f.At(1, 1)->CreateCell(Cell::Color::Green);
    f.At(2, 1)->CreateCell(Cell::Color::Red);
    f.At(3, 1)->CreateCell(Cell::Color::Red);
    f.At(0, 2)->CreateCell(Cell::Color::Blue);
    f.At(1, 2)->CreateCell(Cell::Color::Blue);
    f.At(2, 2)->CreateCell(Cell::Color::Green);
    f.At(3, 2)->CreateCell(Cell::Color::Red);
    f.At(0, 3)->CreateCell(Cell::Color::Red);
    f.At(1, 3)->CreateCell(Cell::Color::Red);
    f.At(2, 3)->CreateCell(Cell::Color::Red);
    f.At(3, 3)->CreateCell(Cell::Color::Blue);
    
    FAIL_IF_FALSE(f.At(0, 0)->GetNumLeftMatch() == 0);
    FAIL_IF_FALSE(f.At(0, 0)->GetNumRightMatch() == 3);
    FAIL_IF_FALSE(f.At(2, 1)->GetNumLeftMatch() == 0);
    FAIL_IF_FALSE(f.At(2, 1)->GetNumRightMatch() == 1);
    FAIL_IF_FALSE(f.At(2, 1)->GetNumUpMatch() == 1);
    FAIL_IF_FALSE(f.At(2, 1)->GetNumDownMatch() == 0);
    
    PASS();
})


ADD_TEST("solvability check", {
    /*
        r g b y
        p r g b
        t p r g
        b y p r
     */
    Field f;
    f.InitWithSize(4, 4, 1, 1);
    f.At(0, 0)->CreateCell(Cell::Color::Red);
    f.At(1, 0)->CreateCell(Cell::Color::Green);
    f.At(2, 0)->CreateCell(Cell::Color::Blue);
    f.At(3, 0)->CreateCell(Cell::Color::Yellow);
    f.At(0, 1)->CreateCell(Cell::Color::Purple);
    f.At(1, 1)->CreateCell(Cell::Color::Red);
    f.At(2, 1)->CreateCell(Cell::Color::Green);
    f.At(3, 1)->CreateCell(Cell::Color::Blue);
    f.At(0, 2)->CreateCell(Cell::Color::Yellow);
    f.At(1, 2)->CreateCell(Cell::Color::Purple);
    f.At(2, 2)->CreateCell(Cell::Color::Red);
    f.At(3, 2)->CreateCell(Cell::Color::Green);
    f.At(0, 3)->CreateCell(Cell::Color::Blue);
    f.At(1, 3)->CreateCell(Cell::Color::Yellow);
    f.At(2, 3)->CreateCell(Cell::Color::Purple);
    f.At(3, 3)->CreateCell(Cell::Color::Red);    
    FAIL_IF_TRUE(f.HasOneMoveMatch());
    
    /*
        r g B y
        p B g B
        t p r g
        b y p r
     */
    f.At(1, 1)->CreateCell(Cell::Color::Blue);
    
    FAIL_IF_FALSE(f.At(1, 1)->HasOneMoveMatch(3));
    FAIL_IF_FALSE(f.At(2, 0)->HasOneMoveMatch(3));
    FAIL_IF_FALSE(f.At(3, 1)->HasOneMoveMatch(3));
    
    FAIL_IF_FALSE(f.HasOneMoveMatch());
    
    // Change back to unsolvable
    f.At(1, 1)->CreateCell(Cell::Color::Red);
    f.MakeOneMoveMatch();
    FAIL_IF_FALSE(f.HasOneMoveMatch());
    
    /*
        y b p
        b g b
        p b g
     */
    f.InitWithSize(3, 3, 1, 1);
    f.At(0, 0)->CreateCell(Cell::Color::Yellow);
    f.At(1, 0)->CreateCell(Cell::Color::Blue);
    f.At(2, 0)->CreateCell(Cell::Color::Purple);
    f.At(0, 1)->CreateCell(Cell::Color::Blue);
    f.At(1, 1)->CreateCell(Cell::Color::Green);
    f.At(2, 1)->CreateCell(Cell::Color::Blue);
    f.At(0, 2)->CreateCell(Cell::Color::Purple);
    f.At(1, 2)->CreateCell(Cell::Color::Blue);
    f.At(2, 2)->CreateCell(Cell::Color::Green);
    FAIL_IF_FALSE(f.HasOneMoveMatch());
    
    /*
     b g r
     g y b
     r y r
     */
    f.InitWithSize(3, 3, 1, 1);
    f.At(0, 0)->CreateCell(Cell::Color::Blue);
    f.At(1, 0)->CreateCell(Cell::Color::Green);
    f.At(2, 0)->CreateCell(Cell::Color::Red);
    f.At(0, 1)->CreateCell(Cell::Color::Green);
    f.At(1, 1)->CreateCell(Cell::Color::Yellow);
    f.At(2, 1)->CreateCell(Cell::Color::Blue);
    f.At(0, 2)->CreateCell(Cell::Color::Red);
    f.At(1, 2)->CreateCell(Cell::Color::Yellow);
    f.At(2, 2)->CreateCell(Cell::Color::Red);
    FAIL_IF_TRUE(f.HasOneMoveMatch());
    
    PASS();
})

END_TEST_CATEGORY()
