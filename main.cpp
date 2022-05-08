#include <iostream>
#include <chrono>
#include <fstream>
#include "LinkedList.h"
#include "AdjacencyList.h"
#include "RandomGen.h"

using namespace std;

int main()
{

    AdjacencyList t = AdjacencyList::createCycle(20);
    AdjacencyList t2 = AdjacencyList::createCompleteGraph(20);
    AdjacencyList t3 = AdjacencyList::createRandomGraph(10, 10, AdjacencyList::Distribution::UNIFORM);
    t3.print();
    t3.save("graph.txt");
    cout << endl;
    AdjacencyList test("graph.txt");
    test.print();
    test.save("graph2.txt");

    int x = 0;
    // RandomGen r(50);
    // for (int i = 0; i < 1000; i++)
    //     cout << r.randNormal() << endl;

    return 0;

}