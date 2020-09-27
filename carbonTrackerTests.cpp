#include <sstream>
#include <unordered_map> 
#include "carbonTracker.hpp"

TEST(MultiKeyTests, constructor){
    CarbonTracker::MultiKey soil(SOIL);
    EXPECT_EQ(Pool::SOIL, soil.key1);
}


int main(int argc, char* argv[]){
    InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}