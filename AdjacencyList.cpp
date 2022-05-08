#include "AdjacencyList.h"

//This source code is original, but I did look here for reference:
//https://www.programiz.com/dsa/graph-adjacency-list

AdjacencyList::AdjacencyList(size_t numVertices, bool isDirected=false)
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

AdjacencyList::~AdjacencyList()
{
    delete[] vertices;
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
            int edgeIndex = adj.V() * v1 + v2; //index in edge existence table - for v1->v2
            int edgeIndexReverse = adj.V() * v2 + v1; //for v2->v1
            if (!adj.edges[edgeIndex])
            {
                adj.addEdge(v1, v2);
                adj.edges[edgeIndex] = true;
                adj.edges[edgeIndexReverse] = true;
                edgeCount++;
            }
        }

        //Edge creation

        //Edge creation
        // std::string edgeStr = std::to_string(v1) + "-" + std::to_string(v2);
        // std::string edgeStr2 = std::to_string(v2) + "-" + std::to_string(v1);
        // if (v1 != v2 && !existingEdges.count(edgeStr)) //check if edge already exists
        // {
        //     existingEdges.insert(edgeStr);
        //     existingEdges.insert(edgeStr2);
        //     adj.addEdge(v1, v2);
        //     edges++;
        // }

    }

    return adj;

}

void AdjacencyList::Node::addNeighbor(int id)
{
    neighbors.push_back(id);
}

void AdjacencyList::addEdge(int v1, int v2)
{
    //Adds an edge between vertices v1 and v2

    //Error checking
    if (v1 > size - 1 || v2 > size - 1)
        throw std::out_of_range("Invalid vertex input");

    vertices[v1].addNeighbor(v2);
    if (!directed) //undirected graph means edge goes both ways
        vertices[v2].addNeighbor(v1);
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