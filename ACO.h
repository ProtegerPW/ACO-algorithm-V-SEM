#ifndef _ACO_H_
#define _ACO_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

#define sANTS 5
#define sALPHA 1 //ALPHA - weight of pheromone,
#define sBETA 2  //BETA - weight of visibility
#define sRHO 0.5 //RHO - evaporation coefficient

class antsColony
{
public:
    antsColony(const char *fileName) : NUMOFANTS(sANTS), ALPHA(sALPHA), BETA(sBETA), RHO(sRHO)
    {
        readData(fileName);
        //initVectors();
    }

    int getNumOfCities();
    void setNumOfCities(int numOfCities);
    int getNumOfAnts();
    double getAlpha();
    double getBeta();
    double getRho();

    void readData(const char *fileName);
    void initVectors();
    void fillVectors(const char *fileName);
    void displayMatrices();

private:
    //----VARIABLES-- ----

    int NUMOFANTS, NUMOFCITIES;
    double ALPHA, BETA, RHO;

    vector<int> graph, routes, bestPath; //graph - stores information about connection with cities, routes - stores information about distance, bestPath - vector to store best Path
    vector<float> visibility, pheromone, deltapheromone, probability;
};

#endif