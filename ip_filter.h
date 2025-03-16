#pragma once
#ifndef __IP_FILTER_H__
#define __IP_FILTER_H__

#include "ip_filter_types.h"

namespace ip_filter{

class IPPool
{
public:
    explicit IPPool(): pool_{}{}
    explicit IPPool(const ip_pool_t& rhs): pool_(rhs){}
    IPPool(ip_pool_t&& rvl): pool_(rvl){}
    

    std::istream& load( std::istream& is );
    std::ostream& out(std::ostream& os) const;
    IPPool& sort(sort_order_t order = sort_order_t::descending);
    IPPool  filter(short b0, short b1 = NO_FILTER_BYTE, short b2 = NO_FILTER_BYTE, short b3 = NO_FILTER_BYTE) const;
    IPPool  filter_any(short b_any) const;
private:
    ip_pool_t  pool_;
};

inline std::istream& operator>>(std::istream& is, IPPool& pool){ return pool.load(is); }
inline std::ostream& operator<<(std::ostream& os, const IPPool& pool){ return pool.out(os); }
    

}; // ip_filter
#endif //__IP_FILTER_H__
