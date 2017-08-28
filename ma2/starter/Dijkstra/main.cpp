#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include "StringSplitter.h"
#include "Graph.h"
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
		getline (cin, input);
		if (input != "-1")
		{
			vector<string> temp = StringSplitter::split(input, " ") ; //split input into groups of three

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
	unordered_map<Vertex *, Path> result = g.computeShortestPath( vertexStart );
	//unordered_map<Vertex *, Path> result = g.computeShortestPath( g.helpfulVertex(startId) ); doesn't work in hackerrank


	//TO DO: OUTPUT
	string space = " ";
	char colon = ':';




	for (auto i : result)
	{

		int firstOut = i.first->getId();
		int secondOut = i.second.getWeight();



		cout << firstOut << colon << space << secondOut << endl;
	}


	//Output should be in the form of:
	//<Sink ID>: <Cost>
	//Vertices should be listed in order of vertex ID



}