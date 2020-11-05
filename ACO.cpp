#ifndef _ACO_C_
#define _ACO_C_

#include "ACO.h"

using namespace std;

void antsColony::readData(const char *fileName)
{
    ifstream dataFile;
    dataFile.open(fileName);

    int a, b, c, numOfCities = 0;
    while (dataFile >> a >> b >> c) //read three num to find out num of cities
    {
        if (a > numOfCities)
            numOfCities = a;
        if (b > numOfCities)
            numOfCities = b;
    }
    dataFile.close();
    setNumOfCities(numOfCities);
    //cout << "Number of cities is equal: " << numOfCities << endl;

    //TODO init vectors and
    initVectors();
    fillVectors(fileName);
}

int antsColony::getNumOfCities()
{
    return NUMOFCITIES;
}

void antsColony::setNumOfCities(int numOfCities)
{
    NUMOFCITIES = numOfCities;
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
    int powerOfCities = pow(getNumOfCities(), 2);
    graph.assign(powerOfCities, 0);
    routes.assign(powerOfCities, 0);
    visibility.assign(powerOfCities, 0.0);
    pheromone.assign(powerOfCities, 1.0);
    deltapheromone.assign(powerOfCities, 0.0);
    probability.assign(powerOfCities, 0.0);
    displayMatrices();
}

void antsColony::displayMatrices()
{
    int numOf = getNumOfCities();
    for (int i = 0; i < numOf; i++)
    {
        for (int j = 0; j < numOf; j++)
        {
            cout << graph[i * numOf + j] << " ";
        }
        cout << endl;
    }

    cout << endl;

    for (int i = 0; i < numOf; i++)
    {
        for (int j = 0; j < numOf; j++)
        {
            cout << routes[i * numOf + j] << " ";
        }
        cout << endl;
    }

    cout << endl;

    for (int i = 0; i < numOf; i++)
    {
        for (int j = 0; j < numOf; j++)
        {
            cout << visibility[i * numOf + j] << " ";
        }
        cout << endl;
    }
}

void antsColony::fillVectors(const char *fileName)
{
    ifstream dataFile;
    dataFile.open(fileName);
    int a, b, c, numOf = getNumOfCities();
    while (dataFile >> a >> b >> c)
    {
        graph[(a - 1) * numOf + (b - 1)] = graph[(b - 1) * numOf + (a - 1)] = 1;
        routes[(a - 1) * numOf + (b - 1)] = routes[(b - 1) * numOf + (a - 1)] = c;
        visibility[(a - 1) * numOf + (b - 1)] = visibility[(b - 1) * numOf + (a - 1)] = (1 / static_cast<double>(c));
    }
    dataFile.close();
    displayMatrices();
}

#endif