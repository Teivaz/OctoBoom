//
//  Test_Node.cpp
//  OctoBoom
//
//  Created by teivaz on 2/13/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#include "BaseTest.h"
#include "../Base/Node.h"


BEGIN_TEST_CATEGORY("Node")

ADD_TEST("children nested transform", {
    std::unique_ptr<Node> root(new Node);
    Node* c1 = new Node;
    Node* c2 = new Node;
    Node* c3 = new Node;
    
    root->AddChild(c1);
    root->AddChild(c2);
    c2->AddChild(c3);
    
    vec2f pos(10, 10);
    vec2f pos2(41, 66);
    
    root->SetPosition(pos);
    c3->SetPosition(pos2);
    
    FAIL_IF_FALSE(root->GetGlobalTransformation().GetTranslationPart() == pos);
    FAIL_IF_FALSE(c1->GetGlobalTransformation().GetTranslationPart() == pos);
    FAIL_IF_FALSE(c3->GetGlobalTransformation().GetTranslationPart() == pos + pos2);
    
    root->SetScale(0.1);
    
    FAIL_IF_FALSE(root->GetGlobalTransformation().GetTranslationPart() == pos);
    FAIL_IF_FALSE(c1->GetGlobalTransformation().GetTranslationPart() == pos);
    FAIL_IF_FALSE(c3->GetGlobalTransformation().GetTranslationPart() == pos + pos2*0.1);
    
    
    
    PASS();
})

END_TEST_CATEGORY()
