//
//  TestManager.cpp
//  OctoBoom
//
//  Created by teivaz on 2/10/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#include "TestManager.h"
#include "BaseTest.h"
#include <iostream>

TestManager* TestManager::s_instance = nullptr;

void TestManager::RunAll()
{
    std::vector<std::string> failedTests;
    for(auto& testCategory : Tests)
    {
        std::cout << "Starting category " << testCategory.first << std::endl;
        for(auto test : testCategory.second)
        {
            if(!test) continue;
            std::cout << "\t<" << test->Name << ">";
            TestResult r = test->Execute();
            if(r.Result)
            {
                std::cout << " OK";
            }
            else
            {
                std::cout << " FAIL";
                failedTests.push_back(testCategory.first +
                                      ": <" + test->Name + "> " +
                                      "Error at line " + std::to_string(r.Line) + ": " + 
                                      "`" + r.Error + "`");
            }
            std::cout << std::endl;
        }
    }
    
    std::cout << std::endl;
    
    if(failedTests.size() == 0)
    {
        std::cout << "All tests passed." << std::endl;
    }
    else
    {
        std::cout << "Failed tests:" << std::endl;
        for(auto& test : failedTests)
        {
            std::cout << "\t" << test << std::endl;
        }
    }
    std::cout << std::endl;
}

TestManager& TestManager::GetInstance()
{
    if(s_instance == nullptr)
    {
        s_instance = new TestManager();
    }
    return *s_instance;
}
