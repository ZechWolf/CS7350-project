#include <iostream>
#include <chrono>
#include <fstream>
#include "LinkedList.h"
#include "AdjacencyList.h"
#include "RandomGen.h"

using namespace std;

int main()
{

    //AdjacencyList t3 = AdjacencyList::createRandomGraph(10, 10, AdjacencyList::Distribution::UNIFORM);
    //t3.colorGraph(AdjacencyList::Coloring::SLVO);

    AdjacencyList g1(10);
    g1.addEdge(5,6);
    g1.addEdge(6,7);
    g1.addEdge(7,8);
    g1.genDegreeList();

    g1.colorGraph(AdjacencyList::Coloring::SLVO);

    int x = 0;
    // RandomGen r(50);
    // for (int i = 0; i < 1000; i++)
    //     cout << r.randNormal() << endl;

    return 0;

}