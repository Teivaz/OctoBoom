//
//  BaseTest.h
//  OctoBoom
//
//  Created by teivaz on 2/10/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#pragma once
#include <string>
#include <memory>
#include <functional>
#include <string>
#include "TestManager.h"


// #define TEST_CATEGORY "myTest"

struct TestResult{
    TestResult(bool result, int line, const std::string& string = "")
    : Result(result), Error(string), Line(line) {}
    bool Result;
    std::string Error;
    int Line;
};

struct Test{
    Test(const std::string& name, const std::function<TestResult()>& execute)
    : Name(name), Execute(execute)
    {
    }
    
    virtual ~Test() {}
    
    std::string Name;
    std::function<TestResult()> Execute;
    
};

typedef std::shared_ptr<Test> TestPtr;

struct TestHelper
{
    TestHelper(){}
    TestHelper(const std::string& category, std::vector<TestPtr> tests)
    {
        TestManager::GetInstance().Tests[category] = tests;
    }
};

#define STR(X) #X

#define FAIL_IF_FALSE(test__) if((test__) == false) \
    return TestResult(false, __LINE__, STR(test__) + std::string(" should be true" ))
#define FAIL_IF_TRUE(test__) if((test__) == true) \
    return TestResult(false, __LINE__, STR(test__) + std::string(" should be false"))
#define FAIL() return TestResult(false, __LINE__)
#define PASS() return TestResult(true, __LINE__)

#define BEGIN_TEST_CATEGORY(category__)\
        static TestHelper helper( (category__), {{
#define END_TEST_CATEGORY() }});

#define ADD_TEST(name__, fun__)\
    TestPtr(new Test( (name__), ([]()->TestResult fun__) )),
