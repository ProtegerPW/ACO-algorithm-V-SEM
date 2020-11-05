#ifndef _ACO_H_
#define _ACO_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

#define sANTS 5
#define sALPHA 1 //ALPHA - weight of pheromone,
#define sBETA 2  //BETA - weight of visibility
#define sRHO 0.5 //RHO - evaporation coefficient

class antsColony
{
public:
    antsColony(int numOfCities) : NUMOFANTS(sANTS), NUMOFCITIES(numOfCities), ALPHA(sALPHA), BETA(sBETA), RHO(sRHO)
    {
        initVectors();
    }

    void readData(const char *filename);

private:
    int getNumOfCities();
    int getNumOfAnts();
    double getAlpha();
    double getBeta();
    double getRho();

    void initVectors();

    //----VARIABLES-- ----

    int NUMOFANTS, NUMOFCITIES;
    double ALPHA, BETA, RHO;

    vector<int> graph, routes, bestPath; //graph - stores information about connection with cities, routes - stores information about distance, bestPath - vector to store best Path
    vector<double> visibility, pheromone, deltapheromone, probability;
};

#endif