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
#define sITER 5

class antsColony
{
public:
    antsColony(const char *fileName, int startCity, int finishCity) : NUMOFANTS(sANTS), ALPHA(sALPHA), BETA(sBETA), RHO(sRHO), startCity(startCity), finishCity(finishCity)
    {
        readData(fileName);
    }

    int getNumOfCities();
    void setNumOfCities(int numOfCities);
    //void setStartCity(int start);
    int getStartCity();
    //void setFinishCity(int finish);
    int getFinishCity();
    int getNumOfAnts();
    double getAlpha();
    double getBeta();
    double getRho();

    void readData(const char *fileName);
    void initVectors();
    void fillVectors(const char *fileName);
    void displayMatrices();
    void bestRoute();

private:
    //----VARIABLES-- ----

    int NUMOFANTS, NUMOFCITIES, startCity, finishCity;
    double ALPHA, BETA, RHO;

    vector<int> graph, antRoutes, bestPath; //graph - stores information about distance, bestPath - vector to store best Path
    vector<float> visibility, pheromone, deltapheromone, probability;
};

#endif