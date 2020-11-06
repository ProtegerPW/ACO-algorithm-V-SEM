#ifndef _ACO_H_
#define _ACO_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <cmath>
#include <numeric>

using namespace std;

#define sANTS 5
#define sALPHA 1 //ALPHA - weight of pheromone,
#define sBETA 2  //BETA - weight of visibility
#define sRHO 0.5 //RHO - evaporation coefficient
#define sITER 5

class AntsColony
{
public:
    AntsColony(const char *file_name, int start_city, int finish_city) : _start_city(start_city), _finish_city(finish_city)
    {
        readData(file_name);
    }

    void readData(const char *file_name);
    void initVectors();
    void fillVectors(const char *file_name);
    void addNode(int start, int stop);
    void displayMatrices();
    void bestRoute();
    void sth(int start);

private:
    //----VARIABLES-- ----

    const int NUM_OF_ANTS = sANTS;
    const double ALPHA = sALPHA, BETA = sBETA, RHO = sRHO;

    int _num_of_cities, _start_city, _finish_city;

    vector<list<int>> _nodes; // wierzcholki + lista sciezek od wierzcholka

    int *_graph;                      // macierz odleglosci pomiedzy miastami
    double *_visibility, *_pheromone; // visibility - macierz odwrotna do graph
                                      // pheromone - macierz rozkladu feromonow

    list<vector<int>> _ant_paths; // lista sciezek obranych przez mrowki
    vector<int> _best_path;       // struktura pomocnicza przechowujaca najlepsza sciezke
};

#endif