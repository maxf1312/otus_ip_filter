#include <iostream>
#include "vers.h"
#include "ip_filter.h"

int main(int, char **) 
{
	using ip_filter::IPPool;
	try
	{
		IPPool ip_pool; 
		std::cin >> ip_pool;
		std::cout << ip_pool.sort();
		std::cout << ip_pool.filter(1) << ip_pool.filter(46, 70) << ip_pool.filter_any(46);	
	}	
	catch(const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
