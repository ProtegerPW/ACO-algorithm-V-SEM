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
#include <random>

using namespace std;

// TODO config file
#define sANTS 5
#define sALPHA 1 //ALPHA - weight of pheromone,
#define sBETA 2  //BETA - weight of visibility
#define sRHO 0.5 //RHO - evaporation coefficient
#define sITER 5

class AntColony;

class Ant {
    public:
        Ant( AntColony *ptr_colony ): _ptr_colony( ptr_colony ) { };

        void resetAnt();
        void exploreGraph(int id_start_node, int id_end_node);
    private:
        bool isVisited(int id_node);
        int chooseOption();

        AntColony *_ptr_colony;

        vector<pair<int, double>> _path_options;

        vector<int> _path_taken;
        int _distance_covered;
        bool _is_stuck;
    };

double getRandom();

class AntColony
{
    friend class Ant;
public:
    AntColony() {};
    ~AntColony();

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

    double randGen();

    const int NUM_OF_ANTS = sANTS;
    const double ALPHA = sALPHA, BETA = sBETA, RHO = sRHO;

    int _num_of_nodes;

    using Connections = vector<list<int>>;
    Connections _connections;   // wierzcholki + lista sciezek od wierzcholka

    int *_graph;                      // macierz odleglosci pomiedzy miastami
    double *_visibility, *_pheromone; // visibility - macierz odwrotna do graph
                                      // pheromone - macierz rozkladu feromonow

    list<Ant*> _ants;

    vector<int> _best_path;           // struktura pomocnicza przechowujaca najlepsza sciezke
};

#endif