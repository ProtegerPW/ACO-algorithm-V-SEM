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
#define sALPHA 5 //ALPHA - weight of pheromone,
#define sBETA 5  //BETA - weight of visibility
#define sRHO 0.5 //RHO - evaporation coefficient
#define sITER 500

double getRandom();

class AntColony
{
public:
    AntColony() : shortest_path_(INT32_MAX){};
    ~AntColony();

    void findOptimisedRoute(int id_start_node, int id_end_node, int num_iterations);
    void displayResults();

    void scanData(const char *file_name);
    void initMatrices();
    void fillMatrices(const char *file_name);

    void displayMatrices();

private:
    class Ant;

    void addEdge(int id_node1, int id_node2);
    void updatePheromone();
    void pheromoneEvaporation();

    const int NUM_OF_ANTS = sANTS;
    const double ALPHA = sALPHA, BETA = sBETA, RHO = sRHO;

    int num_nodes_;

    typedef vector<list<int>> Connections;
    Connections connections_; // TODO opis reprezentacji grafu przez wektor list

    int *graph_;         // matrix holding weights between nodes
    double *visibility_; // matrix used when determining probability of choosing a node by ant
    double *pheromone_;  // matrix of pheromone distribution

    list<Ant *> ants_;

    vector<int> best_path_; // attributes holding information about current best path found
    int shortest_path_;
};

class AntColony::Ant
{
public:
    Ant(AntColony *ptr_colony) : ptr_colony_(ptr_colony), distance_covered_(0), stuck_(false){};

    bool isStuck() const;
    int getDistance() const;
    unsigned int getPathTakenSize() const;
    int getPathTakenValue(int index) const;
    vector<int>::const_iterator getPathTakenIterBegin() const;
    vector<int>::const_iterator getPathTakenIterEnd() const;

    void resetStats();
    void exploreGraph(int id_start_node, int id_end_node);

private:
    void analyzeOptions(int id_curr_node);
    bool isVisited(int id_node);
    int chooseOption();

    AntColony *ptr_colony_;

    // ant's stats
    vector<int> path_taken_;
    int distance_covered_;
    bool stuck_;

    vector<pair<int, double>> path_options_;
};

#endif
