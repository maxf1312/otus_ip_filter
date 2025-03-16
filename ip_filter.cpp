#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <tuple>
#include <array>
#include "ip_filter.h"


namespace ip_filter
{

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
strings_t split(const std::string &str, char d)
{
    strings_t r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

void load_ip_pool(std::istream& is, ip_pool_t& pool)
{
    for(std::string line; std::getline(is, line);)
    {
        auto v = split(line, '\t');
        auto ip_v = split(v.at(0), '.');
        ip_value_t ip{};
        //std::transform(ip_v.cbegin(), ip_v.cend(), ip.begin(), 
        //    [](const auto& s_ip_part) -> u_int8_t{ return std::stoi(s_ip_part); }
        //);
        std::transform(ip_v.cbegin(), ip_v.cend(), ip.begin(), 
            [](const auto& s_ip_part) -> u_int8_t{ return std::stoi(s_ip_part); }
        );
        pool.push_back(ip);
    }
}

ip_pool_t load_ip_pool(std::istream& is)
{
    ip_pool_t rv;
    load_ip_pool(is, rv);
    return rv;
}

std::ostream& out_ip_pool(std::ostream& os, const ip_pool_t& pool)
{
    for(const auto& ip: pool)
    {
        bool b_1st = true;
        for( const auto& part: ip )
        {
            if( b_1st )  b_1st = false;
            else         os << ".";
            os << part;
        }
        os << std::endl;     
    }
    return os;
}

ip_pool_t&  sort_ip_pool(ip_pool_t& pool, sort_order_t order)
{
    std::sort(std::begin(pool), std::end(pool),
            [order](const auto& v1, const auto& v2)->bool {
                auto t1 = std::tie(v1[0], v1[1], v1[2], v1[3]);
                auto t2 = std::tie(v2[0], v2[1], v2[2], v2[3]);
                return order == sort_order_t::ascending ? t1 < t2 : t2 < t1;
            }
    );
    return pool;
}

ip_pool_t filter(ip_pool_t const& ip_pool, short b0, short b1, short b2, short b3)
{
    auto byte_cnv = [](short b) { return b >= 0 && b <= 255 ? b : NO_FILTER_BYTE; };
    ip_value_t cmp_bytes = {byte_cnv(b0), byte_cnv(b1), byte_cnv(b2), byte_cnv(b3)};

    ip_pool_t filtered;
    std::copy_if(std::cbegin(ip_pool), std::cend(ip_pool), std::back_inserter(filtered), 
        [&cmp_bytes](const auto& ip) -> bool {
            size_t i = 0;
            size_t n_match = std::count_if(std::cbegin(ip), std::cend(ip), 
                                [&i, &cmp_bytes](const auto& ip_b) -> bool {
                                    bool r = NO_FILTER_BYTE == cmp_bytes[i] || cmp_bytes[i] == ip_b;
                                    ++i; 
                                    return r; 
                                }  
                            );
            return 4 == n_match;    
        }
    );
    return filtered;
}

ip_pool_t filter_any(ip_pool_t const& ip_pool, short b_any)
{
    auto b_any_v = b_any >= 0 && b_any <= 255 ? b_any : NO_FILTER_BYTE; 

    ip_pool_t filtered;
    std::copy_if(std::cbegin(ip_pool), std::cend(ip_pool), std::back_inserter(filtered), 
        [b_any_v](const auto& ip) -> bool {
            return std::cend(ip) != std::find(std::cbegin(ip), std::cend(ip), b_any_v);
        }
    );
    return filtered;
}



}; // ip_filter

