#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include "LinkedList.h"
#include "AdjacencyList.h"
#include "RandomGen.h"

using namespace std;

void graphCreation()
{
    //Graph creation
    ofstream creationFile("results/graph_creation.csv");
    creationFile << "vertices,cycle,complete\n";

    for (int i = 100; i <= 3000; i += 100)
    {
        //Cycle
        int cycleTime = 0;
        for (int j = 0; j < 3; j++) //take the average of three runs for the time
        {
            auto start = std::chrono::high_resolution_clock::now();
            AdjacencyList g = AdjacencyList::createCycle(i);
            auto stop = std::chrono::high_resolution_clock::now();
            
            auto d = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            cycleTime += d.count();

            if (j == 2)
                g.save("graphs/cycle_" + to_string(i) + ".graph");
        }

        //Complete
        int completeTime = 0;
        for (int j = 0; j < 3; j++) //take the average of three runs for the time
        {
            auto start = std::chrono::high_resolution_clock::now();
            AdjacencyList g = AdjacencyList::createCompleteGraph(i);
            auto stop = std::chrono::high_resolution_clock::now();
            
            auto d = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            completeTime += d.count();

            if (j == 2)
                g.save("graphs/complete_" + to_string(i) + ".graph");
        }

        //Output
        creationFile << i << "," << (cycleTime / 3) << "," << (completeTime / 3) << endl;

    }

    //Random graphs
    ofstream randCreationFile("results/random_creation.csv");
    randCreationFile << "vertices,edges,uniform,skewed,normal\n";

    for (int i = 100; i <= 3000; i += 100)
    {
        int uTime = 0;
        for (int j = 0; j < 2; j++) //get average of two runs (random grpahs can take a while)
        {
            int numEdges = 0.05 * (i * i - 1) / 2; //each random graph has about 5% of the maximum number of edges
            auto start = std::chrono::high_resolution_clock::now();
            AdjacencyList g = AdjacencyList::createRandomGraph(i, numEdges, AdjacencyList::Distribution::UNIFORM);
            auto stop = std::chrono::high_resolution_clock::now();
            
            auto d = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            uTime += d.count();

            if (j == 1)
                g.save("graphs/uniform_" + to_string(i) + ".graph");
        }

        int sTime = 0;
        for (int j = 0; j < 2; j++) //get average of two runs (random grpahs can take a while)
        {
            int numEdges = 0.05 * (i * i - 1) / 2; //each random graph has about 5% of the maximum number of edges
            auto start = std::chrono::high_resolution_clock::now();
            AdjacencyList g = AdjacencyList::createRandomGraph(i, numEdges, AdjacencyList::Distribution::SKEWED);
            auto stop = std::chrono::high_resolution_clock::now();
            
            auto d = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            sTime += d.count();

            if (j == 1)
                g.save("graphs/skewed_" + to_string(i) + ".graph");
        }

        int nTime = 0;
        for (int j = 0; j < 2; j++) //get average of two runs (random grpahs can take a while)
        {
            int numEdges = 0.05 * (i * i - 1) / 2; //each random graph has about 5% of the maximum number of edges
            auto start = std::chrono::high_resolution_clock::now();
            AdjacencyList g = AdjacencyList::createRandomGraph(i, numEdges, AdjacencyList::Distribution::NORMAL);
            auto stop = std::chrono::high_resolution_clock::now();
            
            auto d = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            nTime += d.count();

            if (j == 1)
                g.save("graphs/normal_" + to_string(i) + ".graph");
        }
        
        //Output
        randCreationFile << i << "," << uTime / 2 << "," << sTime / 2 << "," << nTime / 2 << endl;
    }
}

int main()
{
    //Global configuration settings
    const bool CREATE_GRAPHS = false;
    const bool HISTOGRAMS = false;
    const bool SLVO_TEST = false;
    const bool COMPARISON = true;

    //Testing process
    if (CREATE_GRAPHS)
        graphCreation();

    if (HISTOGRAMS)
    {
        AdjacencyList g1 = AdjacencyList::createCycle(1000);
        AdjacencyList g2 = AdjacencyList::createCompleteGraph(1000);
        AdjacencyList g3 = AdjacencyList::createRandomGraph(1000, 2000, AdjacencyList::Distribution::UNIFORM);
        AdjacencyList g4 = AdjacencyList::createRandomGraph(1000, 2000, AdjacencyList::Distribution::SKEWED);
        AdjacencyList g5 = AdjacencyList::createRandomGraph(1000, 2000, AdjacencyList::Distribution::NORMAL);

        g1.conflictHist("cycle.csv");
        g2.conflictHist("complete.csv");
        g3.conflictHist("uni.csv");
        g4.conflictHist("skewed.csv");
        g5.conflictHist("normal.csv");
    }

    if (SLVO_TEST)
    {
        //SLVO small scale test
        AdjacencyList g(5);
        g.addEdge(0,2);
        g.addEdge(1,2);
        g.addEdge(2,4);
        g.addEdge(2,3);
        g.addEdge(3,4);
        g.genDegreeList();

        g.colorGraph(AdjacencyList::Coloring::SLVO);
    }
    
    if (COMPARISON)
    {
        AdjacencyList g("g.graph");
        g.colorGraph(AdjacencyList::Coloring::SLVO);
        AdjacencyList g2("g.graph");
        g2.colorGraph(AdjacencyList::Coloring::SODL);
        AdjacencyList g3("g.graph");
        g3.colorGraph(AdjacencyList::Coloring::RANDOM);
        AdjacencyList g4("g.graph");
        g4.colorGraph(AdjacencyList::Coloring::LLVO);
        AdjacencyList g5("g.graph");
        g5.colorGraph(AdjacencyList::Coloring::LODL);
        AdjacencyList g6("g.graph");
        g6.colorGraph(AdjacencyList::Coloring::IN_ORDER);
        
    }

    return 0;

}