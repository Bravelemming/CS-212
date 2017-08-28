//Jack Daniel Kinne
//MA 4

#include <unordered_map>
#include <utility>
#include <iterator>
#include <cmath>
#include <vector>
#include <iostream>
#include <string>
#include <queue>
#include <functional>
#include <iterator>
#include <algorithm>

#ifndef DISJOINT_SET_H
#define DISJOINT_SET_H

using namespace std;

#pragma region classes

template <typename T>
class DisjointSet
{
private:
	unordered_map<T, pair<int, T>> _sets;


	T& pathCompressionHelper(T &start)
	{
		auto result = _sets.find(start);
		if (result != _sets.end() && result->second.first > 0)
		{
			T&root = pathCompressionHelper(result->second.second);
			result->second.second = root;
			return root;
		}
		else
		{
			return start;
		}
	}

public:

	//does this exist?
	bool exists(int checkNode)
	{
		return  (_sets.find(checkNode) != _sets.end() ) ;
	}

	//returns the total number of unique sets
	int getNumberOfSets() const
	{
		int counter = 0;
		for (auto item : _sets)
		{
			if (item.second.first < 0)
			{
				counter++;
			}
		}
		return counter;
	}

	bool use_path_compression = false;

	void union_with(T &&first, T &&second)
	{
		T f = first;
		T s = second;
		return union_with(first, second);
	}

	void union_with(T &first, T &second)
	{
		T *first_root = find(first);
		T *second_root = find(second);

		if ((*first_root) != (*second_root))
		{
			//union by size
			pair<int, T> &first_pair = _sets[*first_root];
			pair<int, T> &second_pair = _sets[*second_root];

			if (first_pair.first < 0 || second_pair.first < 0)
			{
				pair<int, T> *largest = &first_pair;
				pair<int, T> *smallest = &second_pair;
				int first_size = first_pair.first;
				int second_size = second_pair.first;

				first_size = abs(first_size);
				second_size = abs(second_size);

				if (first_size < second_size)
				{
					largest = &second_pair;
					smallest = &first_pair;
				}

				largest->first = -(first_size + second_size);
				smallest->second = largest->second;
				smallest->first = 1;
			}
		}
		else
		{
			int foo = 5;
		}
	}

	T *find(const T &&start)
	{
		T item = start;
		return find(item);
	}

	T *find(const T &start)
	{
		auto result = _sets.find(start);
		while (result != _sets.end() && result->second.first > 0)
		{
			result = _sets.find(result->second.second);
		}
		if (use_path_compression == true)
		{
			auto compression_result = _sets.find(start);
			while (compression_result != _sets.end() && compression_result->second.second != result->second.second)
			{
				compression_result->second.second = result->second.second;
				compression_result = _sets.find(compression_result->second.second);
			}
			//pathCompressionHelper(start);
		}

		//no result
		if (result == _sets.end())
		{
			_sets[start] = pair<int, T>{};
			_sets[start].first = -1;
			_sets[start].second = start;
			result = _sets.find(start);
		}

		return &(result->second.second);
	}

};
#endif

class Edge 
{
private:
	int sourceNode;
	int destinationNode;
	int weight;

public:

	void setEdge(int source, int destination, int wei) 
	{	
		sourceNode = source;
		destinationNode = destination;
		weight = wei;
	}

	int getNode() const 
	{

		return sourceNode;
	}

	int getDest() const
	{
		return destinationNode;
	}

	int getWeight() const
	{
		return weight;
	}


};

bool operator>(const Edge &lhs, const Edge &rhs)
{
	return lhs.getWeight() > rhs.getWeight();
}


class StringSplitter
{
public:

	//Breaks apart the supplied text based on the given delimiter
	static vector<string> split(string text, string delimiter)
	{
		//vectors are dynamically expanding arrays
		vector<string> pieces;

		//find the first delimiter
		int location = text.find(delimiter);

		//we are starting at the beginning of our string
		int start = 0;

		//go until we have no more delimiters
		while (location != string::npos)
		{
			//add the current piece to our list of pieces
			string piece = text.substr(start, location - start);
			pieces.push_back(piece);

			//update our index markers for the next round
			start = location + 1;
			location = text.find(delimiter, start);
		}

		//at the end of our loop, we're going to have one trailing piece to take care of.
		//handle that now.
		string piece = text.substr(start, location - start);
		pieces.push_back(piece);

		//now, return the completed vector
		return pieces;
	}
};


#pragma endregion

//Jack Daniel Kinne
//MA4
int main(void)
{	//set of results
	DisjointSet<int> resultsSet{};

	//Min PQ	object it takes		way it stores them		comparitor
	priority_queue<Edge, vector<Edge>, greater<Edge> > edge_min_queue{};
	
	//input
	string unionInput;


	while (true)
	{
		getline(cin, unionInput);
		if (unionInput != "-1")
		{	
			//split input on space
			vector<string> splitPiece = StringSplitter::split(unionInput, " "); 

			//grab nodes
			int int_node1 = stoi(splitPiece[0]);
			int int_node2 = stoi(splitPiece[1]);
			//grab weight
			int weight = stoi(splitPiece[2] );

			//make a node - src, id, weight
			Edge Edge;
			Edge.setEdge(int_node1, int_node2, weight);

			//enqueue the object
			edge_min_queue.push(Edge);

		}//end if
		else
		{ //stop loop on -1
			break;
		}
	}//end while(true)

	// root object, for comparison.
	Edge root = edge_min_queue.top(); 
	int rootDest = root.getDest(); // store destination
	int  * rootDestination = &rootDest; // as pointer

	//memory for unique sets
	vector<Edge> setMemory;
	//seen edges
	vector<int> seenEdges;

	while (!edge_min_queue.empty() ) //while we have things in the PQ
	{
		//top pq
		Edge foremost = edge_min_queue.top();

		//grab values
		int node = foremost.getNode();
		int dest = foremost.getDest();
		int wei = foremost.getWeight();
		
		//compare root to current destination. different set? 
		if (resultsSet.find(dest) != rootDestination) 
		{ //if not the same, union.
			resultsSet.union_with(node, dest);

			//check if its in seen; 
			vector<int>::iterator it;
			it = std::find(seenEdges.begin(), seenEdges.end(), dest);
			if (it == seenEdges.end()) { //if it isn't
				cout << node << "->" << dest << "(" << wei << ")" << endl; //output object
				seenEdges.push_back(dest); //save in seen.

				//then remember the edge for later.
				setMemory.push_back(foremost);
			}
	
		}

		//pop queue
		edge_min_queue.pop();
	}
	
	 //remove for hackerrank, pause for VS
	 int pause;
	 cin >> pause;
	 // el fin!

	return 1;
}
