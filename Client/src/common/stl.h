#pragma once
#ifndef _STL_HEADER_
#define _STL_HEADER_

#include <stdint.h>
#include <cassert>

#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
using std::vector;
using std::string;
using std::map;
using std::unordered_map;

template <typename Map, typename F>
void map_erase_if1(Map& m, F pred)
{
	typename Map::iterator i = m.begin();
	while((i = std::find_if(i, m.end(), pred)) != m.end())
		m.erase(i++);
}

template <typename Map, typename F>
void map_erase_if2(Map& m, F pred)
{
	for(typename Map::iterator i = m.begin();
		(i = std::find_if(i, m.end(), pred)) != m.end();
		m.erase(i++));
}

#endif /* _STL_HEADER_ */