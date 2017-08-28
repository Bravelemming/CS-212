#ifndef DISJOINT_SET_H
#define DISJOINT_SET_H

#include <unordered_map>
#include <utility>
#include <iterator>
#include <cmath>
#include <vector>

using namespace std;
//Jack Daniel Kinne
//MA3 
template <typename T>
class DisjointSet
{
private:
	unordered_map<T, pair<int, T>> _sets;

public:

	void union_with(T &&first, T &&second)
	{
		T f = first;
		T s = second;
		return union_with(first, second);
	}

	void union_with(T &first, T &second)
	{  
		//weights
		int weightFirst = 0;
		int weightSecond = 0;
		//roots
		T rootOfFirst = *find(first);
		T rootOfSecond = *find(second);
		
		//is first or second a root?	
		if (find(first) == &first) 
		{ //first is a root		
			weightFirst = _sets[rootOfFirst].first;
		}
		if (find(second) == &second) 
		{ //second is a root	
			weightSecond = _sets[rootOfSecond].first;
		}

		//which root is larger
		if ( weightFirst > 0 & weightFirst > weightSecond )
		{//if the first root one has more nodes
			_sets[rootOfFirst].first+=_sets[rootOfSecond].first; //increment weight of first node
			_sets[rootOfSecond].second = rootOfFirst;//set second node to connect to first
		}
		else if ( weightSecond > 0 & weightFirst <= weightSecond)
		{//if the second root has more nodes (or even)
			_sets[rootOfSecond].first+= _sets[rootOfFirst].first; //increment weight of second node
			_sets[rootOfFirst].second = rootOfSecond;//set first node to connect to second
		}
		else
		{//set connection, no roots.
			_sets[first].first++; //increment weight of first connecting node
			_sets[first].second = second; //connect the first node to the second node
		}
	}//end union_with

	T *find(T &&start)
	{
		T item = start;
		return find(item);
	}

	T *find(T &start)
	{
		//doesn't exist, create pair.
		if ( _sets.find(start) == _sets.end() ) 
		{
			_sets.emplace( start, make_pair( 1, start ) );
		}
		// base case, its a root.
		if ( _sets[start].second == start ) 
		{
			return &start;
		}
		//set current, store find's return.
		T *current = find( _sets[start].second ) ;
		//path compression 
		_sets[start] = make_pair( 1, *current );
		return current;
	}//end find
};

#endif