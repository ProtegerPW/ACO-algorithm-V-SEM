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

class AntsColony
{
public:
    AntsColony(int start_node, int finish_node): _start_node(start_node), _finish_node(finish_node) { }
    ~AntsColony();

    void scanData(const char *file_name);
    void initMatrices();
    void fillMatrices(const char *file_name);

    void displayMatrices();

    void bestRoute();
    void chooseNextCity(int ant);
    void addCityToAnt(int path, int ant);
    bool isVisited(int city, int ant);

private:
    void addEdge(int id_node1, int id_node2);


    const int NUM_OF_ANTS = sANTS;
    const double ALPHA = sALPHA, BETA = sBETA, RHO = sRHO;

    int _num_of_nodes, _start_node, _finish_node;

    vector<list<int>> _connections;   // wierzcholki + lista sciezek od wierzcholka

    int *_graph;                      // macierz odleglosci pomiedzy miastami
    double *_visibility, *_pheromone; // visibility - macierz odwrotna do graph
                                      // pheromone - macierz rozkladu feromonow

    vector<vector<int>> _ant_paths;   // lista sciezek obranych przez mrowki
    vector<int> _best_path;           // struktura pomocnicza przechowujaca najlepsza sciezke
};

#endif