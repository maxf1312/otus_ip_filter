#pragma once
#ifndef __IP_FILTER_TYPES_H__
#define __IP_FILTER_TYPES_H__

#include <iostream>
#include <vector>
#include <string>
#include <array>

namespace ip_filter{

enum class sort_order_t
{
    ascending,
    descending
};

const short NO_FILTER_BYTE = -1;

using strings_t = std::vector<std::string>; 
using ip_value_t = std::array<short, 4>; 
using ip_pool_t = std::vector<ip_value_t>; 

}; // ip_filter
#endif //__IP_FILTER_TYPES_H__
