#include <gtest/gtest.h>
#include <sstream>
#include "ip_filter_impl.h"

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(ip_filterg, test_split)
{
	EXPECT_EQ(ip_filter::split("", '.'), ip_filter::strings_t{""});
	EXPECT_EQ(ip_filter::split("11", '.'), ip_filter::strings_t{"11"});
	EXPECT_EQ(ip_filter::split("..", '.'), (ip_filter::strings_t{"", "", ""}));
	EXPECT_EQ(ip_filter::split("..", '!'), (ip_filter::strings_t{".."}));
	EXPECT_EQ(ip_filter::split("11.", '.'), (ip_filter::strings_t{"11", ""}));
	EXPECT_EQ(ip_filter::split(".12", '.'), (ip_filter::strings_t{"", "12"}));
	EXPECT_EQ(ip_filter::split("11.12", '.'), (ip_filter::strings_t{"11", "12"}));
}

TEST(ip_filterg, test_load)
{
	ip_filter::ip_pool_t pool_etalon{{1, 2, 3, 4}, {4, 3, 1, 4}};
    std::istringstream iss{"1.2.3.4\t23\t33\n4.3.1.4\t23\t33\n"};
	ip_filter::ip_pool_t pool = ip_filter::load_ip_pool(iss);
    
    EXPECT_EQ(pool, pool_etalon);
}

TEST(ip_filterg, test_out)
{
	ip_filter::ip_pool_t pool{{1, 2, 3, 4}, {4, 3, 1, 4}};
    std::ostringstream oss; 
	ip_filter::out_ip_pool(oss, pool);
    
    EXPECT_EQ(oss.str(), "1.2.3.4\n4.3.1.4\n");
}

TEST(ip_filterg, test_sort)
{
    std::string orig_pool_s{"1.2.3.4\t\t\n4.3.1.4\t\t\n"};
    std::string sorted_pool_s{"4.3.1.4\t\t\n1.2.3.4\t\t\n"};
    std::istringstream iss{orig_pool_s};
    ip_filter::ip_pool_t orig_pool = ip_filter::load_ip_pool(iss);
    ip_filter::sort_ip_pool(orig_pool);
    iss.clear();
    iss.str(sorted_pool_s);
    ip_filter::ip_pool_t sorted_pool = ip_filter::load_ip_pool(iss);
    EXPECT_EQ(orig_pool, sorted_pool);
}

TEST(ip_filterg, test_filter)
{
    std::string pool_s{"1.2.3.4\t\t\n4.3.1.4\t\t\n4.3.123.45\t\t\n"};
    std::istringstream iss{pool_s};
    ip_filter::ip_pool_t pool = ip_filter::load_ip_pool(iss);
    std::string filt1_pool_s{"1.2.3.4\t\t\n"};
    std::string filt2_pool_s{"4.3.1.4\t\t\n"};
    std::string filt3_pool_s{"4.3.1.4\t\t\n4.3.123.45\t\t\n"};
    std::string filt4_pool_s{"1.2.3.4\t\t\n4.3.1.4\t\t\n"};

    iss.clear();
    iss.str(filt1_pool_s);
    ip_filter::ip_pool_t filt_pool_et = ip_filter::load_ip_pool(iss);
    ip_filter::ip_pool_t filt_pool = ip_filter::filter(pool, 1);
    EXPECT_EQ(filt_pool, filt_pool_et);

    iss.clear();
    iss.str(filt2_pool_s);
    filt_pool_et = ip_filter::load_ip_pool(iss);
    filt_pool = ip_filter::filter(pool, ip_filter::NO_FILTER_BYTE, ip_filter::NO_FILTER_BYTE, 1);
    EXPECT_EQ(filt_pool, filt_pool_et);

    iss.clear();
    iss.str(filt3_pool_s);
    filt_pool_et = ip_filter::load_ip_pool(iss);
    filt_pool = ip_filter::filter(pool, ip_filter::NO_FILTER_BYTE, 3);
    EXPECT_EQ(filt_pool, filt_pool_et);

    iss.clear();
    iss.str(filt4_pool_s);
    filt_pool_et = ip_filter::load_ip_pool(iss);
    filt_pool = ip_filter::filter(pool, ip_filter::NO_FILTER_BYTE, ip_filter::NO_FILTER_BYTE, ip_filter::NO_FILTER_BYTE, 4);
    EXPECT_EQ(filt_pool, filt_pool_et);

    filt_pool_et.clear();
    filt_pool = ip_filter::filter(pool, 25, ip_filter::NO_FILTER_BYTE, ip_filter::NO_FILTER_BYTE, 4);
    EXPECT_EQ(filt_pool, filt_pool_et);
}

TEST(ip_filterg, test_filter_any)
{
    std::string pool_s{"1.2.3.4\t\t\n4.3.1.4\t\t\n4.3.123.45\t\t\n"};
    std::istringstream iss{pool_s};
    ip_filter::ip_pool_t pool = ip_filter::load_ip_pool(iss);
    std::string filt1_pool_s{"1.2.3.4\t\t\n4.3.1.4\t\t\n"};
    std::string filt2_pool_s{"1.2.3.4\t\t\n"};
    std::string filt3_pool_s{"4.3.123.45\t\t\n"};
    std::string filt4_pool_s{"1.2.3.4\t\t\n4.3.1.4\t\t\n4.3.123.45\t\t\n"};

    iss.clear();
    iss.str(filt1_pool_s);
    ip_filter::ip_pool_t filt_pool_et = ip_filter::load_ip_pool(iss);
    ip_filter::ip_pool_t filt_pool = ip_filter::filter_any(pool, 1);
    EXPECT_EQ(filt_pool, filt_pool_et);

    iss.clear();
    iss.str(filt2_pool_s);
    filt_pool_et = ip_filter::load_ip_pool(iss);
    filt_pool = ip_filter::filter_any(pool, 2);
    EXPECT_EQ(filt_pool, filt_pool_et);

    iss.clear();
    iss.str(filt3_pool_s);
    filt_pool_et = ip_filter::load_ip_pool(iss);
    filt_pool = ip_filter::filter_any(pool, 123);
    EXPECT_EQ(filt_pool, filt_pool_et);

    iss.clear();
    iss.str(filt4_pool_s);
    filt_pool_et = ip_filter::load_ip_pool(iss);
    filt_pool = ip_filter::filter_any(pool, 4);
    EXPECT_EQ(filt_pool, filt_pool_et);

    filt_pool_et.clear();
    filt_pool = ip_filter::filter_any(pool, 254);
    EXPECT_EQ(filt_pool, filt_pool_et);
}
