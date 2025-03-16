#pragma once
#ifndef __IP_FILTER_H__
#define __IP_FILTER_H__

#include <iostream>
#include <vector>
#include <string>
#include <array>


namespace ip_filter{


using strings_t = std::vector<std::string>; 
using ip_value_t = std::array<short, 4>; 
using ip_pool_t = std::vector<ip_value_t>; 
enum class sort_order_t
{
    ascending,
    descending
};
const short NO_FILTER_BYTE = -1;

strings_t split(const std::string &str, char d);
ip_pool_t load_ip_pool( std::istream& is );
std::ostream& out_ip_pool(std::ostream& os, const ip_pool_t& pool);
ip_pool_t& sort_ip_pool(ip_pool_t& pool, sort_order_t order = sort_order_t::descending);
ip_pool_t filter(ip_pool_t const& ip_pool, short b0, short b1 = NO_FILTER_BYTE, short b2 = NO_FILTER_BYTE, short b3 = NO_FILTER_BYTE);
ip_pool_t filter_any(ip_pool_t const& ip_pool, short b_any);

}; // ip_filter
#endif //__IP_FILTER_H__
