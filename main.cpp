#include <iostream>
#include <chrono>
#include <fstream>
#include "LinkedList.h"
#include "AdjacencyList.h"
#include "RandomGen.h"

using namespace std;

int main()
{

    AdjacencyList t3 = AdjacencyList::createRandomGraph(10000, 2000000, AdjacencyList::Distribution::NORMAL);
    //t3.colorGraph(AdjacencyList::Coloring::SLVO);

    AdjacencyList g1(5);
    g1.addEdge(0,2);
    g1.addEdge(1,2);
    g1.addEdge(2,3);
    g1.addEdge(2,4);
    g1.addEdge(3,4);
    g1.genDegreeList();

    //g1.colorGraph(AdjacencyList::Coloring::SLVO);

    return 0;

}