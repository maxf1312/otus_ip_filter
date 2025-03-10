#include <gtest/gtest.h>
#include "lib.h"

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(gh_actions, test_versiong)
{
	EXPECT_GT(version(), 0);
}
