#ifndef VERTEX_H
#define VERTEX_H

#include <string>
#include <queue>
#include <unordered_map>
using namespace std;

class Vertex
{
private:
	int _id;
	static int _id_counter;
	unordered_map<Vertex *, int> _edges;

public:

	Vertex()
	{
		_id_counter++;
		_id = _id_counter;
	}

	Vertex(int id)
	{
		if (id >= _id_counter)
		{
			_id_counter = id + 1;
		}
		_id = id;
	}

	int getId() const
	{
		return _id;
	}

	void addEdge(Vertex *vertex, int weight)
	{
		_edges[vertex] = weight;
	}

	int getEdgeWeight(Vertex *edge)
	{
		return _edges[edge];
	}

	unordered_map<Vertex *, int> &getEdges()
	{
		return _edges;
	}

	void removeEdge(Vertex *vertex)
	{
		_edges.erase(vertex);
	}
};

int operator==(const Vertex &lhs, const Vertex &rhs)
{
	return lhs.getId() == rhs.getId();
}

bool operator<(const Vertex &lhs, const Vertex &rhs)
{
	return lhs.getId() < rhs.getId();
}

bool operator>(const Vertex &lhs, const Vertex &rhs)
{
	return lhs.getId() > rhs.getId();
}

int Vertex::_id_counter = 0;
#endif
#ifndef PATH_H
#define PATH_H
#include <vector>

using namespace std;
class Path
{
private:
	vector<Vertex*> _visited;
	int _weight = 0;

public:
	void markVisited(Vertex *next)
	{
		_visited.push_back(next);
	}

	void setWeight(int w)
	{
		_weight = w;
	}

	int getWeight() const
	{
		return _weight;
	}

	Vertex *getLastVisited()
	{
		return _visited[_visited.size() - 1];
	}

	bool operator()(const Path &lhs, const Path &rhs)
	{
		return (lhs.getWeight() > rhs.getWeight());
	}
};

#endif // !PATH_H

#ifndef GRAPH_H
#define GRAPH_H
#include <unordered_map>
#include <queue>
#include <functional>


//Jack Daniel Kinne
//CS 212 
//MA 2

using namespace std;
class Graph
{
	unordered_map<int, Vertex*> _vertices;

public:

	void addVertex(Vertex *vertex)
	{
		_vertices[vertex->getId()] = vertex;
	}

	//wrote this to check for bad inputs, and do a vertex add. note: can't use on hackerrank.
	Vertex* helpfulVertex(int id)
	{

		auto object = _vertices.find(id);
		if (object != _vertices.end())  // if object exists
		{
			return object->second;
		}
		else
		{ //create a new object
			Vertex* bob = new Vertex(id);
			this->addVertex(bob); //add it
			return bob; //return
		}

	}

	// the main assignment
	unordered_map<Vertex*, Path> computeShortestPath(Vertex *start)
	{
		//given
		unordered_map<Vertex *, Path> result{};
		priority_queue<Path, vector<Path>, Path> dijkstra_queue{};

		Path startingPath = Path(); //create pathing start object

		startingPath.markVisited(start); // mark the first node as visited
		startingPath.setWeight(0);  // set weight to zero
		dijkstra_queue.push(startingPath);

		//keep track of visited SOMEWHERE
		unordered_map<int, int> trackVisitedIds;

		//grab all our edges
		unordered_map<Vertex *, int> edges = start->getEdges();

		//walk the edges while the PQ isn't empty
		while (!dijkstra_queue.empty())
		{
			startingPath = dijkstra_queue.top(); // path = top of PQ
			result.emplace(startingPath.getLastVisited(), startingPath); // add to result last visited, path
																		 //result.at(n.first) = dijkstra_queue.top();
			dijkstra_queue.pop(); //pop the PQ
			Vertex* vertexCheckCondition = startingPath.getLastVisited();

			if (trackVisitedIds.find(vertexCheckCondition->getId()) == trackVisitedIds.end())
			{
				trackVisitedIds.emplace(vertexCheckCondition->getId(), 1); // set visited tracker

				if (result.find(vertexCheckCondition) != result.end())
				{ //if vertex is connected via edge, create a pathing object for each connect
				  //check what the current vertex is connected too by walking edges
					for (auto i : edges)
					{
						Path tempPath;
						//set to visited
						tempPath.markVisited(i.first);
						// set path weight to vertex weight
						tempPath.setWeight(tempPath.getWeight() + i.second);
						//push it into the PQ with the edge weight.
						dijkstra_queue.push(tempPath);
					}//end for (edges)
				}//end if (vertexCheck != null)

			}//end if (tracked...)
		}//end while (PQ not empty)		

		return result;
	}//end computeShortestPath()


};

#endif

#ifndef STRINGSPLITTER_H
#define STRINGSPLITTER_H

#include <string>
#include <vector>

using namespace std;

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

#endif //STRINGSPLITTER_H


#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

//Jack Daniel Kinne
//CS 212 
//MA 2

int main(int argc, char* argv[])
{
	Graph g;
	string input;
	//becuase we can't access _vertices and my helper won't work in hackerrank
	unordered_map<int, Vertex*> _verticesCheck;


	while (true)
	{
		getline(cin, input);
		if (input != "-1")
		{
			vector<string> temp = StringSplitter::split(input, " "); //split input into groups of three

			int conversion = stoi(temp[0]); //int object for adding to vertices
											//Vertex* nodeFirst = g.helpfulVertex( conversion );  //doesn't work in hackerrank

											//check if vertex exists in _verticesCheck, 
			Vertex* nodeFirst = new Vertex(conversion); //create vertex
			auto object = _verticesCheck.find(conversion); // create object for check
			if (object == _verticesCheck.end())  // if object doesn't exist
			{
				g.addVertex(nodeFirst); //add the object to g
				_verticesCheck[conversion] = nodeFirst; //add object to checker
			} // if it does, push the edge only.		

			int conversion2 = stoi(temp[1]); //int object for adding to vertices
											 //Vertex* nodeSecond = g.helpfulVertex(conversion2);  //doesn't work in hackerrank

											 //check if vertex exists in _verticesCheck,
			Vertex* nodeSecond = new Vertex(conversion2); //create vertex
			auto object2 = _verticesCheck.find(conversion2); // create object for check
			if (object2 == _verticesCheck.end())  // if object doesn't exist
			{
				g.addVertex(nodeSecond); //add the object to g
				_verticesCheck[conversion2] = nodeSecond; //add object to checker
			} // if it does, push the edge only.

			int weight = stoi(temp[2]);  //convert to int
			nodeFirst->addEdge(nodeSecond, weight); //add 1 to 2, with weight

		}//end if
		else
		{ //stop loop and ask starting node
			break;
		}
	}//end while(true)

	int startId; //gets the starting node id
	cin >> startId;
	Vertex* vertexStart;

	auto object = _verticesCheck.find(startId);
	if (object != _verticesCheck.end())  // if object exists
	{
		vertexStart = object->second;
	}
	else
	{//break program, bad input.	
		return -1;
	}

	//store a umap of pointer/path of the shortest path of our starting vertex
	unordered_map<Vertex *, Path> result = g.computeShortestPath(vertexStart);
	//unordered_map<Vertex *, Path> result = g.computeShortestPath( g.helpfulVertex(startId) ); doesn't work in hackerrank


	//TO DO: OUTPUT
	string space = " ";

	for (auto i : result)
	{

		int firstOut = i.first->getId();

		int secondOut = i.second.getWeight();

		cout << firstOut << space << secondOut << endl;
	}


	//Output should be in the form of:
	//<Sink ID>: <Cost>
	//Vertices should be listed in order of vertex ID

	//remove for hackerrank, pause for VS
	int pause;
	cin >> pause;

}
