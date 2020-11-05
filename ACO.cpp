#ifndef _ACO_C_
#define _ACO_C_

#include "ACO.h"

using namespace std;

void readData(const char *filename)
{
    int temp = 0;
    ifstream dataFile;
    dataFile.open(filename);
    dataFile >> temp;
    cout << temp;
    dataFile.close();
}

int antsColony::getNumOfCities()
{
    return NUMOFCITIES;
}

int antsColony::getNumOfAnts()
{
    return NUMOFANTS;
}

double antsColony::getAlpha()
{
    return ALPHA;
}

double antsColony::getBeta()
{
    return BETA;
}

double antsColony::getRho()
{
    return RHO;
}

void antsColony::initVectors()
{
    graph.assign(getNumOfCities() ^ 2, 0);
    routes.assign(getNumOfCities() ^ 2, 0);

    pheromone.assign(getNumOfCities() ^ 2, 1);
    deltapheromone.assign(getNumOfCities() ^ 2, 0);
    probability.assign(getNumOfCities() ^ 2, 0);
}

#endif