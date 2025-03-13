#include <iostream>
#include "vers.h"
#include "ip_filter.h"

int main(int, char **) 
{
	using ip_filter::ip_pool_t;
	try
	{
		ip_pool_t ip_pool = ip_filter::load_ip_pool(std::cin);
		ip_filter::sort_ip_pool(ip_pool);
		ip_filter::out_ip_pool(std::cout, ip_pool);	
		ip_filter::out_ip_pool(std::cout, ip_filter::filter(ip_pool, 1));	
		ip_filter::out_ip_pool(std::cout, ip_filter::filter(ip_pool, 46, 70));	
		ip_filter::out_ip_pool(std::cout, ip_filter::filter_any(ip_pool, 46));	
	}	
	catch(const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
