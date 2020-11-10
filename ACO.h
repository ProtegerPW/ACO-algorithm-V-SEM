#ifndef _ACO_H_
#define _ACO_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <cmath>
#include <numeric>
#include <algorithm>

using namespace std;

// TODO config file
#define sANTS 5
#define sALPHA 1 //ALPHA - weight of pheromone,
#define sBETA 2  //BETA - weight of visibility
#define sRHO 0.5 //RHO - evaporation coefficient
#define sITER 5

class Ant {
public:
    Ant() { };
    ~Ant();
    
    void resetAnt();
    void exploreGraph(int id_start_node, int id_end_node);

private:
    vector<int> _path_taken;
    int _distance_covered;
    bool _is_stuck;
};

using Connections = vector<list<int>>;

class AntsColony
{
public:
    friend class Ant;

    AntsColony() {};
    ~AntsColony();

    void findOptimisedRoute(int id_start_node, int id_end_node, int num_iterations);

    void scanData(const char *file_name);
    void initMatrices();
    void fillMatrices(const char *file_name);

    void displayMatrices();

    void bestRoute();
    void chooseNextNode(int id_ant);
    void addNodeToAnt(int id_node, int id_ant);
    bool isVisited(int id_node, int id_ant);

private:
    void addEdge(int id_node1, int id_node2);

    void setupAnts();
    void antExploresGraph(int id_ant, int id_start_node, int id_end_node);

    double randGen();

    const int NUM_OF_ANTS = sANTS;
    const double ALPHA = sALPHA, BETA = sBETA, RHO = sRHO;

    int _num_of_nodes;

    Connections _connections;   // wierzcholki + lista sciezek od wierzcholka

    int *_graph;                      // macierz odleglosci pomiedzy miastami
    double *_visibility, *_pheromone; // visibility - macierz odwrotna do graph
                                      // pheromone - macierz rozkladu feromonow

    list<Ant> _ants;

    vector<int> _best_path;           // struktura pomocnicza przechowujaca najlepsza sciezke
};

#endif