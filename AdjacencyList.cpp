#include "AdjacencyList.h"

//This source code is original, but I did look here for reference:
//https://www.programiz.com/dsa/graph-adjacency-list

AdjacencyList::AdjacencyList(size_t numVertices, bool isDirected)
{
    //Creates a graph with specified num of vertices, with no edges
    vertices = new Node[numVertices]();
    degreeList = new LinkedList<int>[numVertices](); //index represents degree of children
    edges = new bool[numVertices*numVertices](); //index |V|*v1 + v2 is true if v1->v2 is an edge
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
    vertices = new Node[numVertices]();
    degreeList = new LinkedList<int>[numVertices](); //index represents degree of children
    edges = new bool[numVertices*numVertices]();
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
    vertices[v1].addNeighbor(v2);
    if (!directed) //undirected graph means edge goes both ways
        vertices[v2].addNeighbor(v1);

    int edgeIndex = size * v1 + v2; //index in edge existence table - for v1->v2
    int edgeIndexReverse = size * v2 + v1; //for v2->v1
    edges[edgeIndex] = true;
    if (!directed)
        edges[edgeIndexReverse] = true;
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
        degreeList[degree].push_front(vertices[i].id); //insert to linked list at index "degree"
        vertices[i].degreePtr = degreeList[degree].begin(); //add reference to element in linked list
        vertices[i].originalDegree = degree;
        vertices[i].currentDegree = degree;
        averageOriginalDegree += degree;
    }
    averageOriginalDegree /= size;
}

void AdjacencyList::delVertex(int v)
{
    //Mark a vertex as removed during the coloring process
    //Handles changing the degree of neighboring vertices

    Node& n = vertices[v];
    n.deleted = true;
    degreeList[n.currentDegree].erase(n.degreePtr);

    //Decrement degree of v's neighbors
    if (n.neighbors.size() > 0)
    {
        for (auto iter = n.neighbors.begin(); !iter.isEnd(); iter++)
        {
            Node& neighbor = vertices[*iter];
            if (!neighbor.deleted)
            {
                degreeList[neighbor.currentDegree].erase(neighbor.degreePtr);
                neighbor.currentDegree--;
                degreeList[neighbor.currentDegree].push_front(neighbor.id);
                neighbor.degreePtr = degreeList[neighbor.currentDegree].begin();
            }
        }
    }
    
}

void AdjacencyList::outputFile(std::string filename)
{
    std::fstream file(filename + ".csv");
    file << "vertex,color" << std::endl;
    for (size_t i = 0; i < size; i++)
        file << i << "," << vertices[i].color << std::endl;

    file.close();
}

void AdjacencyList::colorGraph(AdjacencyList::Coloring algorithm)
{
    if (algorithm == AdjacencyList::Coloring::SLVO)
        SLVO();
    else if (algorithm == AdjacencyList::Coloring::SODL)
        SODL();
    else if (algorithm == AdjacencyList::Coloring::RANDOM)
        RANDOM();
    else if (algorithm == AdjacencyList::Coloring::LLVO)
        LLVO();
    else if (algorithm == AdjacencyList::Coloring::LODL)
        LODL();
    else
        inOrder();
}

void AdjacencyList::colorList(int* order, int* degWhenDel)
{
    //Colors the graph based on the given vertex ordering
    int maxColor = 1;
    for (size_t i = 0; i < size; i++)
    {
        Node& v1 = vertices[order[i]];
        int color = 1;
        for (size_t j = 0; j < i; j++)
        {
            Node& v2 = vertices[order[j]];
            if (hasEdge(v1.id, v2.id) && color == v2.color)
                color++;
        }
        v1.color = color;
        if (color > maxColor)
            maxColor = color;

        //Printing
        std::cout << "Vertex " << v1.id << ":" << std::endl;
        std::cout << "Color: " << color << ", Original degree: " << v1.originalDegree;
        if (degWhenDel == nullptr)
            std::cout << std::endl;
        else
            std::cout << ", Degree when deleted: " << degWhenDel[i] << std::endl;

        puts("");

    }

    std::cout << "SUMMARY:" << std::endl;
    std::cout << "Colors used: " << maxColor << std::endl;
    std::cout << "Average original degree: " << averageOriginalDegree << std::endl;
}

void AdjacencyList::SLVO()
{
    //Smallest last vertex ordering

    int* deletionOrder = new int[size];
    int* degreeWhenDel = new int[size]; //[i] = degree of vi when deleted
    int index = size - 1; //current index in deletionOrder
    int deleted = 0; //total number of vertices removed
    int degreeIndex = 0; //current index in degreeList
    int maxColors = 1; //keep track of max number of colors needed
    
    while (deleted < size)
    {
        LinkedList<int>& degreeI = degreeList[degreeIndex];
        if (degreeI.size() > 0) //continue if there is no vertex to delete
        {
            int v = *(degreeI.begin());
            delVertex(v);
            deletionOrder[index] = v;
            degreeWhenDel[index] = degreeIndex;

            if (degreeIndex + 1 > maxColors)
                maxColors = degreeIndex + 1;
            
            index--;
            deleted++;
            degreeIndex = 0; //reset back to degree 0 to get to vertices that lowered in degree
        }
        else
            degreeIndex++;
    }

    //Color the graph and output summary stats
    colorList(deletionOrder, degreeWhenDel);
    std::cout << "Maximum degree when deleted: " << maxColors << std::endl;

    int termCliqueSize = 1;
    for (int i = 0; i < size; i++)
    {
        if (!(degreeWhenDel[i] < degreeWhenDel[i+1]))
            break;
        termCliqueSize++;
    }

    std::cout << "Size of terminal clique: " << termCliqueSize << std::endl;

    std::ofstream file("slvo_plot.csv");
    for (int i = 0; i < size; i++)
        file << i + 1 << "," << degreeWhenDel[i] << std::endl;

    delete[] deletionOrder;
    delete[] degreeWhenDel;
}

void AdjacencyList::SODL()
{
    //Smallest original degree last ordering
    int* order = new int[size];
    int index = size - 1; //current index in Order
    int degreeIndex = 0; //current index in degreeList

    for (int i = 0; i < size; i++)
    {
        LinkedList<int>& degreeI = degreeList[i];
        for (auto iter = degreeI.begin(); !iter.isEnd(); iter++)
        {
            order[index] = *iter;
            index--;
        }
    }

    colorList(order);

    delete[] order;

}

void AdjacencyList::RANDOM()
{
    //Random ordering
    int* order = new int[size];
    for (int i = 0; i < size; i++)
        order[i] = i;

    //Shuffle the sequence -- fisher yates shuffle https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
    for (int i = size - 1; i >= 0; i--)
    {
        RandomGen rg(i);
        int j = rg.randUniform();
        std::swap(order[j], order[i]);
    }
    
    colorList(order);

    delete[] order;
    
}

void AdjacencyList::LLVO()
{
    //Largest last vertex ordering

    int* deletionOrder = new int[size];
    int* degreeWhenDel = new int[size]; //[i] = degree of vi when deleted
    int index = size - 1; //current index in deletionOrder
    int deleted = 0; //total number of vertices removed
    int degreeIndex = size - 1; //current index in degreeList, start at highest degree for LLVO
    
    while (deleted < size)
    {
        LinkedList<int>& degreeI = degreeList[degreeIndex];
        if (degreeI.size() > 0) //continue if there is no vertex to delete
        {
            int v = *(degreeI.begin());
            delVertex(v);
            deletionOrder[index] = v;
            degreeWhenDel[index] = degreeIndex;
            
            index--;
            deleted++;
            degreeIndex = size - 1; //reset back to degree 0 to get to vertices that lowered in degree
        }
        else
            degreeIndex--;
    }

    colorList(deletionOrder, degreeWhenDel);

    delete[] deletionOrder;
    delete[] degreeWhenDel;
}

void AdjacencyList::LODL()
{
    //Largest original degree last ordering
    int* order = new int[size];
    int index = 0; //current index in Order
    int degreeIndex = 0; //current index in degreeList

    for (int i = 0; i < size; i++)
    {
        LinkedList<int>& degreeI = degreeList[i];
        for (auto iter = degreeI.begin(); !iter.isEnd(); iter++)
        {
            order[index] = *iter;
            index++;
        }
    }

    colorList(order);

    delete[] order;

}

void AdjacencyList::inOrder()
{
    int* order = new int[size];
    for (int i = 0; i < size; i++)
        order[i] = i;

    colorList(order);

    delete[] order;
}

void AdjacencyList::conflictHist(std::string filename)
{
    //Outputs histogram of conflicts for each vertex
    std::ofstream file(filename);
    file << "vertex,numEdges" << std::endl;
    for (size_t i = 0; i < size; i++)
        file << i << "," << vertices[i].neighbors.size() << std::endl;
}