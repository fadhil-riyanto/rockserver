#include "gtest/gtest.h"
#include "parse.c"


TEST(test_parse_complete_operand_set_FIRST, string_equal)
{
        char setcommand[] = "set \"testbigvalueisset\" \"nowisvalue\"";

        struct parse_res res;
        alloc_parse(&res);
        parse(setcommand, strlen(setcommand), &res);

        // sdd(res.op_code);
        
        EXPECT_STREQ(res.op1, "testbigvalueisset");
        free_parse(&res);
}


int main() 
{
        testing::InitGoogleTest();
        return RUN_ALL_TESTS();
}