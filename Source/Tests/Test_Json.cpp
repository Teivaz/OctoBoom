//
//  Test_Json.cpp
//  OctoBoom
//
//  Created by teivaz on 2/12/16.
//  Copyright © 2016 teivaz. All rights reserved.
//

#include "BaseTest.h"
#include "../Base/JsonParser.h"

BEGIN_TEST_CATEGORY("JSON")


ADD_TEST("empty string", {
    JsonParser p;
    FAIL_IF_TRUE( p.Parse("") );
    PASS();
})

ADD_TEST("empty string spaces", {
    JsonParser p;
    FAIL_IF_TRUE( p.Parse("     ") );
    PASS();
})

ADD_TEST("invalid syntax", {
    JsonParser p;
    FAIL_IF_TRUE( p.Parse("osdhjf[oaidsj") );
    PASS();
})

ADD_TEST("empty object", {
    JsonParser p;
    FAIL_IF_FALSE( p.Parse("{}") );
    PASS();
})

ADD_TEST("empty array", {
    JsonParser p;
    FAIL_IF_FALSE( p.Parse("[]") );
    PASS();
})

ADD_TEST("simple object", {
    JsonParser p;
    FAIL_IF_FALSE(p.Parse("{\"a\": 10}"));
    JsonValueRef value = p.GetRootObject();
    FAIL_IF_FALSE( (bool)value );
    FAIL_IF_FALSE( value->IsObject() );
    const JsonValue::Object& object = value->GetObject();
    FAIL_IF_FALSE( object.size() == 1 );
    JsonValueRef ten = object.at("a");
    FAIL_IF_FALSE(ten->IsNumber());
    FAIL_IF_FALSE(ten->GetNumber() == 10);
    PASS();
})

ADD_TEST("complex object", {
    JsonParser p;
    FAIL_IF_FALSE(p.Parse("{\"a\": {}, \"b\": [], \"c\"  : {\"d\": [1, .3, \"aff\", {}, {}]}}"));
    JsonValueRef value = p.GetRootObject();
    FAIL_IF_FALSE( (bool)value );
    FAIL_IF_FALSE( value->IsObject() );
    const JsonValue::Object& object = value->GetObject();
    JsonValueRef valueC = object.at("c");
    FAIL_IF_FALSE(valueC->IsObject());
    const JsonValue::Object& c = valueC->GetObject();
    JsonValueRef valueD = c.at("d");
    FAIL_IF_FALSE(valueD->IsArray());
    const JsonValue::Array& d = valueD->GetArray();
    FAIL_IF_FALSE(d.size() == 5);
    FAIL_IF_FALSE(d.at(1)->IsNumber());
    FAIL_IF_FALSE(d.at(1)->GetNumber() == 0.3);
    FAIL_IF_FALSE(d.at(2)->IsString());
    FAIL_IF_FALSE(d.at(2)->GetString() == "aff");
    PASS();
})


END_TEST_CATEGORY()
