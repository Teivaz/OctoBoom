//
//  Tests.cpp
//  OctoBoom
//
//  Created by teivaz on 2/10/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#include <stdio.h>
#include "Tests/TestManager.h"


int main(int argc, const char * argv[])
{
    TestManager::GetInstance().RunAll();
}
