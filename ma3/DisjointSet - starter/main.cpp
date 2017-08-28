#include "DisjointSet.h"
//REMOVE INCLUDE "name.h" FOR HACKERRANK
#include <iostream>
#include <string>
using namespace std;
//Jack Daniel Kinne
//MA3 
int main(void)
{
	DisjointSet<char> resultsSet{};
	string unionInput;
	while (true)
	{
		getline(cin, unionInput);
		if (unionInput != "-1")
		{
			char node1 = unionInput[0];
			char node2 = unionInput[2];
			resultsSet.union_with(node1, node2); // union 
		}//end if
		else
		{ //stop while loop
			break;
		}
	}//end while(true)

	string findInput;
	while (true)
	{
		getline(cin, findInput);
		if (findInput != "-1")
		{
			char node1 = findInput[0];
			char node2 = findInput[2];
			cout << (*resultsSet.find(node1) == *resultsSet.find(node2)) << endl;
		}//end if
		else
		{ //stop while loop
			break;
		}
	}//end while(true)

	/*
	//remove for hackerrank, pause for VS
	int pause;
	cin >> pause;
	*/
	return 1;
}