#include "AdjacencyList.h"

//This source code is original, but I did look here for reference:
//https://www.programiz.com/dsa/graph-adjacency-list

AdjacencyList::AdjacencyList(size_t numVertices, bool isDirected)
{
    //Creates a graph with specified num of vertices, with no edges
    vertices = new Node[numVertices];
    degreeList = new LinkedList<int>[numVertices]; //index represents degree of children
    edges = new bool[numVertices*numVertices]; //index |V|*v1 + v2 is true if v1->v2 is an edge
    directed = isDirected;
    size = numVertices;

    for (size_t i = 0; i < numVertices; i++)
        vertices[i].id = i;

}

AdjacencyList::AdjacencyList(std::string filename, bool isDirected)
{
    //Construct an adjacency list from an input file
    std::ifstream file(filename);
    size_t numVertices;
    int currentLine = 2;

    //Read in size and starting positions and initialize adj list members
    file >> numVertices;
    size = numVertices;

    directed = isDirected;
    vertices = new Node[numVertices];
    degreeList = new LinkedList<int>[numVertices]; //index represents degree of children
    edges = new bool[numVertices*numVertices];
    for (size_t i = 0; i < numVertices; i++)
        vertices[i].id = i;

    int* startingPos = new int[size];
    for (int i = 0; i < size; i++)
    {
        int pos;
        file >> pos;
        startingPos[i] = pos;
        currentLine++;
    }

    //Read in the edges
    for (int i = 0; i < size; i++)
    {
        while (currentLine < startingPos[i + 1])
        {
            int v2;
            file >> v2;
            addEdge(i, v2);
            currentLine++;
        }
    }

    file.close();
    delete[] startingPos;

    genDegreeList();
}

AdjacencyList::~AdjacencyList()
{
    delete[] vertices;
    delete[] degreeList;
    delete[] edges;
}

AdjacencyList AdjacencyList::createCycle(size_t numVertices)
{
    AdjacencyList adj(numVertices);
    for (size_t i = 0; i < numVertices - 1; i++)
        adj.addEdge(i, i+1);
    adj.addEdge(numVertices - 1, 0);

    adj.genDegreeList();

    return adj;
}

AdjacencyList AdjacencyList::createCompleteGraph(size_t numVertices)
{
    AdjacencyList adj(numVertices, true); //init as directed graph to make creation easier
    for (size_t i = 0; i < numVertices; i++)
        for (size_t j = 0; j < numVertices; j++)
            if (i != j)
                adj.addEdge(i, j);

    adj.directed = false; //change back to undirected

    adj.genDegreeList();
    
    return adj;
}

AdjacencyList AdjacencyList::createRandomGraph(size_t numVertices, size_t numEdges, AdjacencyList::Distribution dist)
{
    RandomGen rg(numVertices - 1);
    AdjacencyList adj(numVertices);

    int edgeCount = 0; //keep track of how many edges are successfully created
    while (edgeCount < numEdges)
    {
        //Generate random pair of vertices
        int v1, v2;
        if (dist == AdjacencyList::Distribution::UNIFORM)
        {
            v1 = rg.randUniform();
            v2 = rg.randUniform();
        }
        else if (dist == AdjacencyList::Distribution::SKEWED)
        {
            v1 = rg.randSkewed();
            v2 = rg.randSkewed();
        }
        else
        {
            v1 = rg.randNormal();
            v2 = rg.randNormal();
        }

        if (v1 != v2 && v1 < numVertices && v2 < numVertices)
        {
            if (!adj.hasEdge(v1, v2))
            {
                adj.addEdge(v1, v2);
                edgeCount++;
            }
        }

    }

    adj.genDegreeList();
    return adj;

}

void AdjacencyList::Node::addNeighbor(int id)
{
    neighbors.push_back(id);
}

bool AdjacencyList::hasEdge(int v1, int v2) const
{
    int edgeIndex = size * v1 + v2; //index in edge existence table - for v1->v2
    int edgeIndexReverse = size * v2 + v1; //for v2->v1
    return (edges[edgeIndex] || (!directed && edges[edgeIndexReverse]));
}

void AdjacencyList::addEdge(int v1, int v2)
{
    //Adds an edge between vertices v1 and v2

    //Error checking
    if (v1 > size - 1 || v2 > size - 1)
        throw std::out_of_range("Invalid vertex input");

    //Update edges table
    if (!hasEdge(v1, v2))
    {
        vertices[v1].addNeighbor(v2);
        if (!directed) //undirected graph means edge goes both ways
            vertices[v2].addNeighbor(v1);

        int edgeIndex = size * v1 + v2; //index in edge existence table - for v1->v2
        int edgeIndexReverse = size * v2 + v1; //for v2->v1
        edges[edgeIndex] = true;
        if (!directed)
            edges[edgeIndexReverse] = true;
    }
}

size_t AdjacencyList::V()
{
    //Returns |V|, the number of vertices in the graph
    return size;
}

void AdjacencyList::print()
{   
    /*
    Outputs a representation of graph in the form :
    vertex0, edge0, edge1...
    vertex1, edge0...
    Mostly for debugging
    */

    for (size_t i = 0; i < size; i++)
    {
        std::cout << vertices[i].id << ",";
        for (auto iter = vertices[i].neighbors.begin(); !iter.isEnd(); iter++)
            std::cout << *iter << ",";

        std::cout << std::endl;
    }

}

void AdjacencyList::save(std::string filename)
{
    std::ofstream f(filename);
    f << size << std::endl; //num vertices
    
    //Starting lines for each vertex
    int pos = 1 + size + 1; //keep track of starting pos for each vertex
    for (size_t i = 0; i < size; i++)
    {
        f << pos << std::endl;
        pos += vertices[i].neighbors.size();
    }

    //Edges
    for (size_t i = 0; i < size; i++)
        for (auto iter = vertices[i].neighbors.begin(); !iter.isEnd(); iter++)
            f << *iter << std::endl;

    f.close();
}

void AdjacencyList::genDegreeList()
{
    //Generates the degree list for the current graph
    //Should be called once the graph has been generated

    for (size_t i = 0; i < size; i++)
    {
        int degree = vertices[i].neighbors.size();
        degreeList[degree].push_back(vertices[i].id); //insert to linked list at index "degree"
        vertices[i].degreePtr = degreeList[degree].end(); //add reference to element in linked list
    }


}

void AdjacencyList::SLVO()
{
    int* deletionOrder = new int[size];
    int* degreeWhenDel = new int[size]; //[i] = degree of vi when deleted
    int index = size - 1;
    for (size_t i = 0; i < size; i++)
    {
        LinkedList<int>& degreeI = degreeList[i];
        if (degreeI.size() == 0) continue;

        for (auto iter = degreeI.begin(); !iter.isEnd(); iter++)
        {
            int& v = *iter;
            deletionOrder[index] = v;
            degreeWhenDel[index] = i;
            index--;

            Node& n = vertices[v];
            n.deleted = true;
            degreeI.erase(n.degreePtr);
            
            for (auto iter2 = n.neighbors.begin(); !iter2.isEnd(); iter2++)
                
        }
    }

    delete[] deletionOrder;
    delete[] degreeWhenDel;
}