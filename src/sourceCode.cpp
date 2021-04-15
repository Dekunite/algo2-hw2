/* @Author
Student Name: <Muhammet Derviş Kopuz>
Student ID : <504201531>
Date: <06/04/2021> 
Please compile the project using -std=c++11 flag
<g++ sourceCode.cpp -o hw1 -std=c++11>
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <list>
#include <map>

using namespace std;
#define INF 0x3f3f3f3f

class Building {
	private:
		string name;
		int number;

	public:
		Building(string name, int number);
		string getname();
		int getnumber();
};

Building::Building(string name, int number) {
	this->name = name;
	this->number = number;
}

string Building::getname() {
	return name;
}

int Building::getnumber() {
	return number;
}

class Graph {
	private:
		int numberOfVertices;
		// In a weighted graph, we need to store vertex
    // and weight pair for every edge
		list< pair<Building, int> > *adj;
		
	public:
		// Graph specific methods
		Graph(int numberOfVertices);
		void AddEdge(Building source, Building destination, int weight);
		void FindPrimMST(Building startVertex, vector<Building>* buildings);
		
};

// Constructor only sets the number of vertices
Graph::Graph(int numberOfVertices) {
  this->numberOfVertices = numberOfVertices;
  adj = new list< pair<Building, int> >[numberOfVertices];
 }


// Adds an edge to the graph
void Graph::AddEdge(Building source, Building destination, int weight) {
	// Converts the letter of a vertice to a numeric representation: (source - 'A')
	//source ve dest için int koyulması lazım
  adj[source.getnumber()].push_back(make_pair(destination, weight));
	adj[destination.getnumber()].push_back(make_pair(source, weight));
}

void Graph::FindPrimMST(Building startvertex, vector<Building>* buildings) {
	priority_queue< pair<int, int>, vector< pair<int, int> >, greater< pair<int, int> > > pq;

	Building src = startvertex;

	// Create a vector for keys and initialize all
    // keys as infinite (INF)
	vector<int> key(numberOfVertices, INF);
	vector<int>::iterator i;
	int iCounter = 0;
	for (i = key.begin(); i != key.end(); ++i){
		cout << iCounter << ": " << *i <<endl;
		iCounter++;
	}

	// To store parent array which in turn store MST
  vector<int> parent(numberOfVertices, -1);
	vector<int>::iterator is;
	iCounter = 0;
	for (is = parent.begin(); is != parent.end(); ++is){
		cout << iCounter << ": " << *is <<endl;
		iCounter++;
	}

	// To keep track of vertices included in MST
  vector<bool> inMST(numberOfVertices, false);
	vector<bool>::iterator id;
	iCounter = 0;
	for (id = inMST.begin(); id != inMST.end(); ++id){
		cout << iCounter << ": " << *id <<endl;
		iCounter++;
	}

	// Insert source itself in priority queue and initialize
  // its key as 0.
	//uzaklık, vertexNum 
  pq.push(make_pair(0, startvertex.getnumber()));
  key[src.getnumber()] = 0;

	/* Looping till priority queue becomes empty */
  while (!pq.empty()) {
		// The first vertex in pair is the minimum key
		// vertex, extract it from priority queue.
		// vertex label is stored in second of pair (it
		// has to be done this way to keep the vertices
		// sorted key (key must be first item
		// in pair)
		int sourceNumber = pq.top().second;
		pq.pop();

		inMST[sourceNumber] = true;  // Include vertex in MST
		int iCounter = 0;
		cout << "-------inMST-------" << endl;
		for (id = inMST.begin(); id != inMST.end(); ++id){
		cout << iCounter << ": " << *id <<endl;
		iCounter++;
		}

		// 'i' is used to get all adjacent vertices of a vertex
		list< pair<Building, int> >::iterator i;
		for (i = adj[sourceNumber].begin(); i != adj[sourceNumber].end(); ++i)
		{
				// Get vertex label and weight of current adjacent
				// of u.
				Building destination = (*i).first;
				int destinationWeight = (*i).second;

				//  If v is not in MST and weight of (u,v) is smaller
				// than current key of v
				if (inMST[destination.getnumber()] == false && key[destination.getnumber()] > destinationWeight)
				{
					// Updating key of v
					key[destination.getnumber()] = destinationWeight;
					vector<int>::iterator it;
					int iCounter = 0;
					cout << "-------key-------" << endl;
					for (it = key.begin(); it != key.end(); ++it){
						cout << iCounter << ": " << *it <<endl;
						iCounter++;
					}

					pq.push(make_pair(key[destination.getnumber()], destination.getnumber()));
					parent[destination.getnumber()] = sourceNumber;
					iCounter = 0;
					cout << "-------parent-------" << endl;
					for (is = parent.begin(); is != parent.end(); ++is){
						cout << iCounter << ": " << *is <<endl;
						iCounter++;
					}
				}
		}

	}

	// Print edges of MST using parent array
	for (int i = 1; i < numberOfVertices; ++i) {
		int buildingNum = parent[i];
		//uzaklığı elde edip printle
		cout << buildings->at(buildingNum).getname() << " " << buildings->at(i).getname() <<endl;
		printf("%d - %d\n", buildings->at(buildingNum).getnumber(), i);

	}


}


int main() {

  string fname;
  fname = "city_plan_1.txt";
  //cin >> fname;
  ifstream city_plan(fname);

  string source;
  string dest;
  string weight;
  string line;

	int buildingCounter = 0;
	vector<Building>* buildings = new vector<Building>;
	Graph g(6);

  while (getline(city_plan, line)) {
    stringstream ss(line);
    getline(ss, source, ',');
    getline(ss, dest, ',');
    getline(ss, weight, ',');

/*
		if(buildingCounter == 0) {
			Building* newBuildingS = new Building(source, buildingCounter);
			buildingNames->push_back(*newBuildingS);
			buildingCounter++;
			Building* newBuildingD = new Building(dest, buildingCounter);
			buildingNames->push_back(*newBuildingD);
			buildingCounter++;

			g.AddEdge(*newBuildingS, *newBuildingD, stoi(weight));
		} else {

		}
*/

    cout << source << " " << dest << " " << weight <<endl;
  }
	// Create the graph with the number of vertices that it will contain
	
  Building* startVertex = new Building("GP",0);
  Building* hipp = new Building("hipp",1);
  Building* ch1 = new Building("ch1",2);
  Building* ch2 = new Building("ch2",3);
  Building* hp1 = new Building("hp1",4);
  Building* hp2 = new Building("hp2",5);

	buildings->push_back(*startVertex);
	buildings->push_back(*hipp);
	buildings->push_back(*ch1);
	buildings->push_back(*ch2);
	buildings->push_back(*hp1);
	buildings->push_back(*hp2);
  //Building* startVertex = new Building("GP",0);
  //Building* startVertex = new Building("GP",0);
	// Add all of the edges to the graph
	g.AddEdge(*startVertex,*hipp, 1);
	g.AddEdge(*startVertex, *ch1, 9);
	g.AddEdge(*startVertex, *ch2, 10);
	g.AddEdge(*startVertex, *hp1, 1);
	g.AddEdge(*ch1, *hp1, 1);
	g.AddEdge(*ch1, *hp2, 1);
	g.AddEdge(*ch2, *hp1, 100);
	g.AddEdge(*ch2, *hp2, 100);
	// Print all the edges in the graph
	g.FindPrimMST(*startVertex, buildings);
	
	return 0;
}