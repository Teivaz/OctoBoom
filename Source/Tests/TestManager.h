//
//  TestManager.h
//  OctoBoom
//
//  Created by teivaz on 2/10/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#pragma once
#include <vector>
#include <map>
#include <memory>

class Test;
typedef std::shared_ptr<Test> TestPtr;

class TestManager
{
public:
    
    static TestManager& GetInstance();
    
    std::map<std::string, std::vector<TestPtr> > Tests;
    
    void RunAll();
    
private:
    static TestManager* s_instance;
};

