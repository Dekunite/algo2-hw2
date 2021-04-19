/* @Author
Student Name: <Muhammet Derviş Kopuz>
Student ID : <504201531>
Date: <20/04/2021> 
Question 1
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <list>
#include <algorithm>

using namespace std;
#define INF 0x6FFFFFFF

//Class to hold building names and assigned numbers
class Building {
	private:
		string name;
		int number;

	public:
		Building(string name, int number);
		Building();
		string getName();
		int getNumber();
};

//constructor
Building::Building(string name, int number) {
	this->name = name;
	this->number = number;
}

//empty constructor
Building::Building() {
	
}

//getter
string Building::getName() {
	return name;
}

//getter
int Building::getNumber() {
	return number;
}

//class to hold every edge
class Edge {
	private:
		Building source;
		Building dest;
		int weight;

	public:
		Edge(Building source, Building dest, int weight);
		Edge();
		Building getSource();
		Building getDest();
		int getWeight();

};

//constructor
Edge::Edge(Building source, Building dest, int weight) {
	this->source = source;
	this->dest = dest;
	this->weight = weight;
}

//empty constructor
Edge::Edge() {
	
}

//getter
Building Edge::getSource() {return source;}

//getter
Building Edge::getDest() {return dest;}

//getter
int Edge::getWeight() {return weight;}

class Graph {
	private:
		//number of vertices in the graph
		int numberOfVertices;

    //Every building and its weight is stored in an adjacency list
		list< pair<Building, int> > *adj;
		
	public:
		Graph(int numberOfVertices);
		void AddEdge(Building source, Building destination, int weight);
		void FindPrimMST(Building startVertex, vector<Building>* buildings, vector<Edge>* edges);
		
};

// constructor for the graph
Graph::Graph(int numberOfVertices) {
  this->numberOfVertices = numberOfVertices;
  adj = new list< pair<Building, int> >[numberOfVertices];
}

void Graph::AddEdge(Building source, Building destination, int weight) {
	//adds the corresponding building and weight pair to the adjacency list of both source and destination
  adj[source.getNumber()].push_back(make_pair(destination, weight));
	adj[destination.getNumber()].push_back(make_pair(source, weight));
}

//For rule 4 GP must be connected to a church
Building firstChurch(list< pair<Building, int> > *adj, vector<bool> MST, vector<int>* dist, int sourceNumber) {
	list< pair<Building, int> >::iterator i;
	Building destination;
	int destinationWeight;
	Building cheapestDest;
	int cheapestWeight = INF;
	//traverse adjacency list of source
	for (i = adj[sourceNumber].begin(); i != adj[sourceNumber].end(); ++i) {
		destination = (*i).first;
		destinationWeight = (*i).second;

		//If it isnt a Church continue
		if (!(destination.getName().compare(0,2,"Ch") == 0)) {
			continue;
		}

		//store the cheapest church
		if (destinationWeight < cheapestWeight) {
			cheapestWeight = destinationWeight;
			cheapestDest = destination;
		}
	}

	//check if destination is operated on before using the MST vector.
	//if the total distance can be reduced  
	if (MST[cheapestDest.getNumber()] == false && dist->at(cheapestDest.getNumber()) > cheapestWeight)
	{
		//update/reduce the distance
		dist->at(cheapestDest.getNumber()) = cheapestWeight;
	}
	return cheapestDest;
}

//Rule 5, GP and Hipp should be connected
Building firstHipp(list< pair<Building, int> > *adj, vector<bool> MST, vector<int>* dist, int sourceNumber) {
	list< pair<Building, int> >::iterator i;
	Building destination;
	int destinationWeight;
	Building cheapestDest;
	int cheapestWeight = INF;
	//traverse adjacency list of source
	for (i = adj[sourceNumber].begin(); i != adj[sourceNumber].end(); ++i) {
		destination = (*i).first;
		destinationWeight = (*i).second;

		//find the hipp
		if (!(destination.getName().compare(0,4,"Hipp") == 0)) {
			continue;
		}

		//keep track of cheapest hipp
		if (destinationWeight < cheapestWeight) {
			cheapestWeight = destinationWeight;
			cheapestDest = destination;
		}
	}

	//check if destination is operated on before using the MST vector.
	//if the total distance can be reduced  
	if (MST[cheapestDest.getNumber()] == false && dist->at(cheapestDest.getNumber()) > cheapestWeight)
	{
		//update/reduce the dist
		dist->at(cheapestDest.getNumber()) = cheapestWeight;

	}
	return cheapestDest;
}

//get building name by assigned building number
string getBuilding(int buildingNumber, vector<Building>* buildings) {
	vector<Building>::iterator it;
	//traverse buildings
	for (it = buildings->begin(); it != buildings->end(); ++it) {
		if((*it).getNumber() == buildingNumber) {
			return (*it).getName();
		}
	}
	return NULL;

}

void Graph::FindPrimMST(Building startvertex, vector<Building>* buildings, vector<Edge>* edges) {
	priority_queue< pair<int, int>, vector< pair<int, int> >, greater< pair<int, int> > > pq;

	Building src = startvertex;
	bool firstConnectionChurch = false;
	bool firstConnectionHipp = false;
	string addedChurch;

	//keep track of all distances for every vertex, initialize distances as infinite
	vector<int> dist(numberOfVertices, INF);

	//keep a parent array to indicate which vertex is connected to which vertex
	//no parent = -1
  vector<int> parent(numberOfVertices, -1);

	//keep track of vertices which are operated on, no need to operate twice
  vector<bool> MST(numberOfVertices, false);

	//uzaklık, vertexNum 
	//push source vertex in to priority queue
  pq.push(make_pair(0, startvertex.getNumber()));
	//initialize source's distance as 0 since it is the starting point
  dist[src.getNumber()] = 0;

	vector<string> doubleCheck;

	//while priority queue is not empty
  while (!pq.empty()) {
		//priority queue holds (distance, building number)
		//it is ordered by the first variable
		//get the second variable for the element which has the smallest distance
		int sourceNumber = pq.top().second;
		//get the name of the source
		string sourceName = getBuilding(sourceNumber, buildings);
		//pop the first element from queue
		pq.pop();

		//mark the source as operated on
		MST[sourceNumber] = true;

		//first connection rule bw GP and church
		if (!firstConnectionChurch) {
			Building destination = firstChurch(adj, MST, &dist, sourceNumber);
			addedChurch = destination.getName();

			pq.push(make_pair(dist[destination.getNumber()], destination.getNumber()));
			parent[destination.getNumber()] = sourceNumber;

			firstConnectionChurch = true;
		}

		//first connection rule bw GP and Hipp
		if (!firstConnectionHipp) {
			Building destination = firstHipp(adj, MST, &dist, sourceNumber);

			pq.push(make_pair(dist[destination.getNumber()], destination.getNumber()));
			parent[destination.getNumber()] = sourceNumber;

			firstConnectionHipp = true;
		}

		list< pair<Building, int> >::iterator i;
		//traverse over all adjacent buildings for the source
		for (i = adj[sourceNumber].begin(); i != adj[sourceNumber].end(); ++i)
		{
				//get destination building
				Building destination = (*i).first;
				//get destination distance
				int destinationWeight = (*i).second;

				//skip Hipp it is already connected to GP
				if ((destination.getName().compare("Hipp") == 0)) {
					doubleCheck.push_back(sourceName);
					continue;
				}

				//skip the first added church it is already connected to GP
				if ((destination.getName().compare(addedChurch) == 0)) {
					doubleCheck.push_back(sourceName);
					continue;
				}

				//hipp and bassilica not connceted rule. Rule 6.
				if ((sourceName.compare("Hipp")==0 && destination.getName().compare(0,3,"Bas")==0) || (sourceName.compare(0,3,"Bas")==0 && destination.getName().compare("Hipp")==0)) {
					continue;
				}

				//important people houses not connected. Rule 7
				if ((sourceName.compare(0,2,"Hp")==0 && destination.getName().compare(0,2,"Hp")==0) || (sourceName.compare(0,2,"Hp")==0 && destination.getName().compare(0,2,"Hp")==0)) {
					continue;
				}

				//since we dont change the parent of Hipp and FirstChurch be sure to check them as the parent node while traversing
				bool inList = ((find(doubleCheck.begin(), doubleCheck.end(), destination.getName()) != doubleCheck.end()) && sourceName.compare(addedChurch) == 0) ||
											((find(doubleCheck.begin(), doubleCheck.end(), destination.getName()) != doubleCheck.end()) && sourceName.compare("Hipp") == 0);
				//check if destination is operated on before using the MST vector.
				//if the total distance can be reduced  
				if ((MST[destination.getNumber()] == false && dist[destination.getNumber()] > destinationWeight) ||
				 (inList && dist[destination.getNumber()] > destinationWeight))
				{
					//update/reduce the dist
					dist[destination.getNumber()] = destinationWeight;

					//push the pair consisting of (new distance for destination, destination)
					pq.push(make_pair(dist[destination.getNumber()], destination.getNumber()));
					//assign source as parent to destination / connect destination and source
					parent[destination.getNumber()] = sourceNumber;
				}
		}

	}

	//create a sorte edges vector to print edges is ascending order using the parent vector
	vector<Edge> sortedEdges;
	for (int i = 1; i < numberOfVertices; ++i) {
		int buildingNum = parent[i];
		string sourceName = buildings->at(buildingNum).getName();
		string destName = buildings->at(i).getName();
		
		//iterate over all edges
		vector<Edge>::iterator edgeIt;
		for (edgeIt = edges->begin(); edgeIt != edges->end(); ++edgeIt) {
			Edge currentEdge = (*edgeIt);
			
			//push edges in to sorted edges vector
			if (currentEdge.getSource().getName() == sourceName && currentEdge.getDest().getName() == destName) {
				sortedEdges.push_back(currentEdge);
			} else if (currentEdge.getSource().getName() == destName && currentEdge.getDest().getName() == sourceName) {
				sortedEdges.push_back(currentEdge);
			}
		}

		
	}
		int totalWeight = 0;
		//while sortedEdges is not empty
		while (!sortedEdges.empty()) {
			//İnitialize as infinite
			int minWeight = INF;
			Building minSource;
			Building minDest;
			vector<Edge>::iterator minEdge;
			vector<Edge>::iterator edgeIt;
			//iterate over sortedEdges
			for (edgeIt = sortedEdges.begin(); edgeIt != sortedEdges.end(); ++edgeIt) {
				Edge currentEdge = *edgeIt;

				//keep track of smallest edge
				if (currentEdge.getWeight() < minWeight) {
					minWeight = currentEdge.getWeight();
					minSource = currentEdge.getSource();
					minDest = currentEdge.getDest();
					minEdge = edgeIt;
				}

			}
			cout << minSource.getName() << " " << minDest.getName() << " " << minWeight << "\n";
			totalWeight += minWeight;
			sortedEdges.erase(minEdge);
		}
		cout << totalWeight << "\n";
}


int main() {

  string fname;
  //fname = "city_plan_1.txt";
	//get file name
  cin >> fname;
  ifstream city_plan(fname);

  string source;
  string dest;
  string weight;
  string line;

	int buildingCounter = 0;
	//init buildings vector
	vector<Building>* buildings = new vector<Building>;
	//init edges vector
	vector<Edge>* edges = new vector<Edge>;
	Building startVertex; 

	//read file
  while (getline(city_plan, line)) {
    stringstream ss(line);
		//split by ","
    getline(ss, source, ',');
    getline(ss, dest, ',');
    getline(ss, weight, ',');

		Building* newBuildingS;
		Building* newBuildingD;

		//if building counter = 0, directly push buildings in to buildings vector.
		if(buildingCounter == 0) {
			newBuildingS = new Building(source, buildingCounter);
			//assign first building to startvertex GP in test cases
			startVertex = *newBuildingS;
			buildings->push_back(*newBuildingS);
			buildingCounter++;
			newBuildingD = new Building(dest, buildingCounter);
			buildings->push_back(*newBuildingD);
			buildingCounter++;

			Edge* newEdge = new Edge(*newBuildingS, *newBuildingD, stoi(weight));
			//push new edge in to edges vector
			edges->push_back(*newEdge);
		} else {
			vector<Building>::iterator buildIt;
			bool sourcePresent = false;
			bool destPresent = false;

			//check the buildings in the new loop. If they are new or existing
			for (buildIt = buildings->begin(); buildIt != buildings->end(); ++buildIt ) {
				if ((*buildIt).getName() == source) {
					sourcePresent = true;
					*newBuildingS = *buildIt;
				}
				if ((*buildIt).getName() == dest) {
					destPresent = true;
					*newBuildingD = *buildIt;
				}
			}

			//if new building push to buildings vector
			if (!sourcePresent){
				newBuildingS = new Building(source, buildingCounter);
				buildings->push_back(*newBuildingS);
				buildingCounter++;
			}
			//if new building push to buildings vector
			if (!destPresent) {
				newBuildingD = new Building(dest, buildingCounter);
				buildings->push_back(*newBuildingD);
				buildingCounter++;
			}

			//push the new edge to edges vector
			Edge* newEdge = new Edge(*newBuildingS, *newBuildingD, stoi(weight));
			edges->push_back(*newEdge);
		}
  }

	//initialize the graph with number of vertices equal to buidings size
	Graph g(buildings->size());

	//add every edge to the graph
	vector<Edge>::iterator ite;
	for (ite = edges->begin(); ite != edges->end(); ++ite ) {
		//std::cout << (*ite).getSource().getName() << " " << (*ite).getDest().getName() << " " << (*ite).getWeight() << endl;
		g.AddEdge((*ite).getSource(), (*ite).getDest(), (*ite).getWeight());
	}
	
	//find MST with prim's algorithm and print the result
	g.FindPrimMST(startVertex, buildings, edges);
	
	return 0;
}