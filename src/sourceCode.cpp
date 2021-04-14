/* @Author
Student Name: <Muhammet Derviş Kopuz>
Student ID : <504201531>
Date: <06/04/2021> 
Please compile the project using -std=c++11 flag
<g++ sourceCode.cpp -o hw1 -std=c++11>

*/

#include<iostream>
#include <fstream>
#include <list>
#include <vector>
#include <math.h>
#include <algorithm>
#include <time.h>

using namespace std;

//Forward Declerations
class Node;
bool checkZeroConditions(string operand1, Node currentNode);
bool checkSummationRuleStart(int operand1, int operand2, int result);
int getLastValue(string operand1, Node currentNode);
int getValue(string letter, Node currentNode);
bool loopDigits(string operand1, string operand2, string result, Node currentNode);
bool checkSummationRule(int operand1, int operand2, int result, int carry1);
bool checkSummationRuleEnd(int operand, int carry);
int getPrevCarry (string operand1, string operand2, string result, Node currentNode, int length1, int length2, int minsize);


class Node {

  public:
    Node(string letter, int value, int verticeNum);
    Node();

    vector<string>* letters;
    vector<int>* values;
    int verticeNum;
    bool visited;
    };

Node::Node (string letter, int value, int verticeNum1) {
  letters = new vector<string>;
  letters->push_back(letter);
  values = new vector<int>;
  values->push_back(value);
  verticeNum = verticeNum1;
  visited = false;
}

Node::Node() {
  letters = new vector<string>;
  values = new vector<int>;
  visited = false;
}

class Graph {

  public:
    Graph(Node* vertex,int lettersSize);
    void addEdge(Node src, Node* dest);
    Node BFS(Node startVertex,string operand1, string operand2, string result, list<string> letters);
    int DFS(Node startVertex,string operand1, string operand2, string result, list<string> letters);

    Node* vertex;
    vector<Node>* adj;
    int numberOfVisitedNotes;
    int nodesInMemory;
    int maxNodesInMermory;
};

Graph::Graph(Node* vertex1, int lettersSize) {
  vertex = vertex1;
  int verticeNum = vertex1->verticeNum;
  long adjSize = pow(10, lettersSize) + 0.5; //round up
  numberOfVisitedNotes = 0;
  nodesInMemory = 0;
  maxNodesInMermory = 0;
  adj = new vector<Node>[adjSize];
}

void Graph::addEdge(Node parent, Node* child) {
  adj[parent.verticeNum].push_back(*child);
}

Node Graph::BFS(Node node, string operand1, string operand2, string result, list<string> letters) {
  
  int verticeNum = node.verticeNum;

  list<Node> queue;

  //mark current node visited and enqueue
  node.visited = true;
  queue.push_back(node);

  while(!queue.empty()) {
    node = queue.front();
    queue.pop_front();

    if (queue.size() > maxNodesInMermory) {
      maxNodesInMermory = queue.size();
    }

    int adjSize = adj[node.verticeNum].size();
    //for every adj node
    for (int i = 0; i != adjSize; ++i) {
      Node currentNode = adj[node.verticeNum].at(i);
      numberOfVisitedNotes++;
      if(!currentNode.visited) {
        currentNode.visited = true;

        //check for duplicate numbers
        vector<int> temp = *currentNode.values;
        sort(temp.begin(), temp.end());
        vector<int>::iterator valueIterator = unique(temp.begin(), temp.end());
        bool hasDuplicates = !(valueIterator == temp.end());
        if (hasDuplicates) {
          delete currentNode.letters;
          delete currentNode.values;
          continue;
        }

        //not 0 conditions
        if (!checkZeroConditions(operand1, currentNode)) {
          delete currentNode.letters;
          delete currentNode.values;
          continue;
        }

        if (!checkZeroConditions(operand2, currentNode)) {
          delete currentNode.letters;
          delete currentNode.values;
          continue;
        }

        if (!checkZeroConditions(result, currentNode)) {
          delete currentNode.letters;
          delete currentNode.values;
          continue;
        }

        //Check summation rules
        bool passed = checkSummationRuleStart(getLastValue(operand1, currentNode),
                                              getLastValue(operand2, currentNode),
                                              getLastValue(result, currentNode));
        if (!passed) {
          delete currentNode.letters;
          delete currentNode.values;
          continue;
        }

        passed = loopDigits(operand1, operand2, result, currentNode);
        if (!passed) {
          delete currentNode.letters;
          delete currentNode.values;
          continue;
        }
        
        queue.push_back(currentNode);
        if (currentNode.letters->size() == letters.size() + 1) {
          return currentNode;
        }
      }
    }
  }
  return node;
}

int Graph::DFS(Node node, string operand1, string operand2, string result, list<string> letters) {
 
  int verticeNum = node.verticeNum;

  //mark current node visited and enqueue
  node.visited = true;

  // Recur for all the vertices adjacent to this vertex
	int adjSize = adj[node.verticeNum].size();
  for (int i = 0; i != adjSize; ++i) {
    Node currentNode = adj[node.verticeNum].at(i);
    numberOfVisitedNotes++;
    nodesInMemory++;
    if (nodesInMemory > maxNodesInMermory) {
      maxNodesInMermory = nodesInMemory;
    }
    if(!currentNode.visited) {

      //check for duplicate numbers
      vector<int> temp = *currentNode.values;
      sort(temp.begin(), temp.end());
      vector<int>::iterator valueIterator = unique(temp.begin(), temp.end());
      bool hasDuplicates = !(valueIterator == temp.end());
      if (hasDuplicates) {
        continue;
      }

      //not 0 conditions
      if (!checkZeroConditions(operand1, currentNode)) {
        continue;
      }

      if (!checkZeroConditions(operand2, currentNode)) {
        continue;
      }

      if (!checkZeroConditions(result, currentNode)) {
        continue;
      }

      //Check summation rules
      bool passed = checkSummationRuleStart(getLastValue(operand1, currentNode),
                                            getLastValue(operand2, currentNode),
                                            getLastValue(result, currentNode));
      if (!passed) {
        continue;
      }

      passed = loopDigits(operand1, operand2, result, currentNode);
      if (!passed) {
        continue;
      }

      if (currentNode.letters->size() == letters.size() + 1) {
        return currentNode.verticeNum;
      }
      
      int resultVertice = DFS(currentNode, operand1, operand2, result, letters);
      if (resultVertice != 0) {
        return resultVertice;
      }
    }
  }
  nodesInMemory-=10;
  return 0;
}

bool loopDigits(string operand1, string operand2, string result, Node currentNode) {
  int length1 = operand1.length() - 2 ;
  int length2 = operand2.length() - 2;
  int resultLength = result.length() - 2;

  while (length1 >= 0 && length2 >= 0 && resultLength >= 0) {
    //bir önceki basamaklar toplamı 10 dan büyükse 1 değilse 0
    int carry1 = getValue(string(1, operand1.at(length1 + 1)), currentNode);
    int carry2 =  getValue(string(1, operand2.at(length2 + 1)), currentNode);
    if (carry1 == -9 || carry2 == -9) {
      length1--;
      length2--;
      resultLength--;
      continue;
    }
    int carry = (carry1 + carry2) >= 10 ? 1 : 0;

    int value1 = getValue(string(1, operand1.at(length1)), currentNode);
    int value2 = getValue(string(1, operand2.at(length2)), currentNode);
    int valueR = getValue(string(1, result.at(resultLength)), currentNode);

    bool passed = checkSummationRule(value1, value2, valueR, carry);
    if (!passed) {
      return false;
    }

    length1--;
    length2--;
    resultLength--;

  }

  if(length1 >= 0 && length2 < 0) {

    length1--;
    length2--;
    resultLength--;

  }

  if(length2 >= 0 && length1 < 0) {

    length1--;
    length2--;
    resultLength--;
  }
  
  

  //check the first digit
  if (resultLength == 0) {

    int carry1;
    if (length1 + 1 >= 0) {
      carry1 = getValue(string(1, operand1.at(length1 + 1)), currentNode);
    } else {
      carry1 = 0;
    }
    int carry2;
    if (length2 + 1 >= 0) {
      carry2 =  getValue(string(1, operand2.at(length2 + 1)), currentNode);
    } else {
      carry2 = 0;
    }
    carry1 += getPrevCarry(operand1, operand2, result, currentNode, length1, length2, max(operand1.size(), operand2.size()));
    int carry = (carry1 + carry2) >= 10 ? 1 : 0;

    if (length1 == 0) {
      carry += (getValue(string(1, operand1.at(length1)), currentNode));
    }

    if (length2 == 0) {
      carry += (getValue(string(1, operand2.at(length2)), currentNode));
    }

    int valueR = getValue(string(1, result.at(resultLength)), currentNode);
    bool passed = checkSummationRuleEnd(valueR, carry);
    if (!passed) {
      return false;
    }
  }

  return true;
}

int getPrevCarry (string operand1, string operand2, string result, Node currentNode, int length1, int length2, int maxsize) {
  int pPrevCarry = 0;
  int i = maxsize;
  for (int k = 0; k<maxsize; k++) {
    int pPrevCarry1;
    if (length1 + i >= 0) {
      pPrevCarry1 = getValue(string(1, operand1.at(length1 + i)), currentNode);
    } else {
      pPrevCarry1 = 0;
    }
    int pPrevCarry2;
    if (length2 + i >= 0) {
      pPrevCarry2 =  getValue(string(1, operand2.at(length2 + i)), currentNode);
    } else {
      pPrevCarry2 = 0;
    }
    pPrevCarry1 += pPrevCarry;
    pPrevCarry = (pPrevCarry1 + pPrevCarry2) >= 10 ? 1 : 0;
    i--;

  }
  return pPrevCarry;
}

int getValue(string letter, Node currentNode) {
  for (int k = 0; k < currentNode.letters->size(); k++) {
    if (letter == currentNode.letters->at(k)) {
      return currentNode.values->at(k);
    }
  }
  return -9;
}

bool checkZeroConditions(string operand1, Node currentNode) {
  for (int i = 0; i < 1; i++) {
    string letter = string(1, operand1.at(i));
    for (int k = 0; k < currentNode.letters->size(); k++) {
      if (letter == currentNode.letters->at(k)) {
        if(currentNode.values->at(k) == 0) {
          return false;
        }
      }
    }
  }
  return true;
}

int getLastValue(string operand1, Node currentNode) {
  int length = operand1.size();
  string lastLetter = string(1,operand1.at(length-1));

  for (int k = 0; k < currentNode.letters->size(); k++) {
    if (lastLetter == currentNode.letters->at(k)) {
      return currentNode.values->at(k);
    }
  }
  return -9;
}

bool checkSummationRuleStart(int operand1, int operand2, int result) {
  if (operand1 == -9 || operand2 == -9 || result == -9) {
    return true;
  }

  int carry = 0;
  if ((operand1 + operand2) >= 10) {
    carry = 1;
  }

  if (((operand1 + operand2) == (result + 10 * carry))) {
    return true;
  }
  return false;
}

bool checkSummationRule(int operand1, int operand2, int result, int carry1) {
  if (operand1 == -9 || operand2 == -9 || result == -9) {
    return true;
  }

  int carry2 = 0;
  if ((operand1 + operand2 + carry1) >= 10) {
    carry2 = 1;
  }

  if ((operand1 + operand2 + carry1) == (result + 10 * carry2)) {
    return true;
  }
  return false;
}

bool checkSummationRuleEnd(int operand, int carry) {
  if (operand == -9) {
    return true;
  }

  if(operand == carry) {
    return true;
  }
  return false;
}

Node* cloneNode(Node parent) {
  Node* newNode = new Node();
  *newNode->letters = *parent.letters;
  *newNode->values = *parent.values;
  newNode->visited = false;

  return newNode;
}

void writeToFile(list<string> letters, vector<int> numbers, Node resultNode, string fileName) {
  ofstream outputFileName(fileName);
  outputFileName << "\t";

  for(vector<int>::iterator number = numbers.begin(); number!= numbers.end(); ++number) {
    if (number == numbers.end()-1) {
      outputFileName << *number;
      outputFileName << "\n";
      continue;
    }
    outputFileName << *number << "\t";
  }

  int letterCounter = 0;
  for (list<string>::iterator letter = letters.begin(); letter!= letters.end(); ++letter) {
    outputFileName << *letter ;
    int value = resultNode.values->at(letterCounter);
    for (int i = 0; i < value; i++) {
      outputFileName << "\t" << ".";
    }
    outputFileName << "\t" << "1";
    int remainder = 9 - value;
    if (remainder == 0) {
      outputFileName << "\n";
    }
    for (int k = 0; k < remainder; k++) {
      if (k == remainder -1) {
        outputFileName << "\t" << "." << "\n";
        continue;
      }
      outputFileName << "\t" << ".";
    }
    letterCounter++;
  }

}

void pushLetters(string input) {
  list<string> letters;
  for (int i = 0; i < input.size(); i++) {
    letters.push_back(string(1,input.at(i)));
  }

}

int main(int argc, char* argv[])
{
  
  const string searchMethod = "bfs";
  const string operand1 = "two";
  const string operand2 = "two";
  const string result = "four";
  const string outputFileName = "output";
  

 /*
  const string searchMethod = argv[1];
  const string operand1 = argv[2];
  const string operand2 = argv[3];
  const string result = argv[4];
  const string outputFileName = argv[5];
  */

 list<string> letters;

 //add operand1 letters
 for (int i = 0; i < operand1.size(); i++) {
   bool contains = false;
   for (list<string>::iterator letter = letters.begin(); letter!= letters.end(); ++letter) {
     if (string(1,operand1.at(i)) == *letter) {
       contains = true;
     }
   }

    if (!contains) {
      letters.push_back(string(1,operand1.at(i)));
    }
  }

  //add operand2 letters
  for (int i = 0; i < operand2.size(); i++) {
   bool contains = false;
   for (list<string>::iterator letter = letters.begin(); letter!= letters.end(); ++letter) {
     if (string(1,operand2.at(i)) == *letter) {
       contains = true;
     }
   }

    if (!contains) {
      letters.push_back(string(1,operand2.at(i)));
    }
  }

  //add result letters
  for (int i = 0; i < result.size(); i++) {
   bool contains = false;
   for (list<string>::iterator letter = letters.begin(); letter!= letters.end(); ++letter) {
     if (string(1,result.at(i)) == *letter) {
       contains = true;
     }
   }

    if (!contains) {
      letters.push_back(string(1,result.at(i)));
    }
  }

  //push numbers
  vector<int> numbers;
  numbers.push_back(0);
  numbers.push_back(1);
  numbers.push_back(2);
  numbers.push_back(3);
  numbers.push_back(4);
  numbers.push_back(5);
  numbers.push_back(6);
  numbers.push_back(7);
  numbers.push_back(8);
  numbers.push_back(9);
  
  //initialize start node
  Node* startNode = new Node("start", 0, 0);
  Graph graph(startNode, letters.size());
  int verticeCounterNumber = 1;
  int layerCount = 0;
  int adjCounter = 0;

  //add possibilities for first letter
  for (list<string>::iterator letter = letters.begin(); letter!= letters.end(); ++letter) {
    for(vector<int>::iterator number = numbers.begin(); number!= numbers.end(); ++number) {
      Node* newNode = new Node(*letter, *number, verticeCounterNumber);
      graph.addEdge(*graph.vertex, newNode); //Add a new node with every value to parent
      verticeCounterNumber++;
    }
    break;
  }

  layerCount++;
  string poppedLetter = letters.front();
  letters.pop_front();

  //create the tree for the remaining letters
  for (list<string>::iterator letter = letters.begin(); letter!= letters.end(); ++letter) {
    for(int k = 0; k<layerCount; k++) { 
      for(int i = 0; i < 10; i++) {
        for(vector<int>::iterator number = numbers.begin(); number!= numbers.end(); ++number) {
          
          Node* newNode = cloneNode(graph.adj[adjCounter].at(i));
          newNode->verticeNum = verticeCounterNumber;
          newNode->letters->push_back(*letter);
          newNode->values->push_back(*number);

          graph.addEdge(graph.adj[adjCounter].at(i), newNode); //Add a new node with every value to parent
          delete newNode;

          verticeCounterNumber++;

        }
      }
      adjCounter++;
    }
    layerCount *= 10;
  }

  Node resultNode; 
  int dfsResult;
  const clock_t begin_time = clock();
  if (searchMethod == "BFS" || searchMethod == "bfs") {
    resultNode = graph.BFS(*startNode, operand1, operand2, result, letters);

  } else if (searchMethod == "DFS" || searchMethod == "dfs") {

    dfsResult = graph.DFS(*startNode, operand1, operand2, result, letters);
    int onda = dfsResult%10;
    int dfsResultParent = (dfsResult - onda)/10;
    resultNode = graph.adj[dfsResultParent].at(onda - 1);

  }
  float finalTime = float( clock () - begin_time ) /  CLOCKS_PER_SEC;

  letters.push_front(poppedLetter);

  writeToFile(letters, numbers, resultNode, outputFileName);

  cout << "Algorithm: " << searchMethod << endl;
  cout << "Number of the visited nodes: " << graph.numberOfVisitedNotes << endl;
  cout << "Maximum number of nodes kept in the memory: " << graph.maxNodesInMermory << endl;
  cout << "Running time: " << finalTime << " seconds" << endl;
  cout << "Solution: ";
  for (int i = 0; i<resultNode.letters->size(); i++) {
    if (i == resultNode.letters->size() - 1) {
      cout << resultNode.letters->at(i) << ": " << resultNode.values->at(i);
      continue;
    }
      cout << resultNode.letters->at(i) << ": " << resultNode.values->at(i) << ", ";
  }

  return 0;
  
}