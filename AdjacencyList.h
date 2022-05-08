#pragma once
#include "LinkedList.h"
#include "RandomGen.h"
#include <fstream>
#include <string>
#include <iostream>
#include <unordered_set>

//This source code is original, but I did look here for reference:
//https://www.programiz.com/dsa/graph-adjacency-list

class AdjacencyList {

private:

    class Node {
    private:

        int id;
        int color = -1;
        int degree;
        bool deleted; //marks if removed by coloring algo
        LinkedList<int> neighbors;
        LinkedList<int>::ListIter degreePtr;

    public:

        Node() = default;
        Node(int id): id(id), neighbors() {}

        friend class AdjacencyList;
        bool hasNeighbor(int id) const;
        void addNeighbor(int id);

    };

    Node* vertices;
    LinkedList<int>* degreeList; //adj list representing degree list of each vertex
    bool directed; //true if the graph is a directed graph
    size_t size;

    bool* edges; //look-up table to check edge existence. Edge v1->v2 exists if (|V|*v1 + v2) is true

    //Coloring algos
    void SLVO();

public:

    //Constructors
    AdjacencyList() = default;
    AdjacencyList(size_t numVertices, bool isDirected=false);
    AdjacencyList(std::string filename, bool isDirected=false);
    ~AdjacencyList();

    enum class Distribution {
        UNIFORM,
        SKEWED,
        NORMAL
    };

    enum class Coloring {
        SLVO,
        SODL,
        RANDOM,
        LLVO,
        LODL,
    };

    //Named constructors
    static AdjacencyList createCycle(size_t numVertices);
    static AdjacencyList createCompleteGraph(size_t numVertices);
    static AdjacencyList createRandomGraph(size_t numVertices, size_t numEdges, AdjacencyList::Distribution dist);

    //Methods
    void addEdge(int v1, int v2);
    void print();
    void save(std::string filename);
    void genDegreeList();
    bool hasEdge(int v1, int v2) const;
    size_t V();

    //Coloring methods
    void colorGraph(AdjacencyList::Coloring algorithm);

};