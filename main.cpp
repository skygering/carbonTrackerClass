#include "carbonTracker.hpp"
#include <gtest/gtest.h>

TEST(ExampleTest, AddTest)
{
    EXPECT_EQ(false, false);
}

int main(int argc, char* argv[]){
    std::cout << "hello world!" << std::endl;
    InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

