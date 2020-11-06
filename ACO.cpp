#ifndef _ACO_C_
#define _ACO_C_

#include "ACO.h"

using namespace std;

void AntsColony::readData(const char *file_name)
{
    ifstream dataFile;
    dataFile.open(file_name);

    int a, b, c, num_of_cities = 0;
    while (dataFile >> a >> b >> c) //scan file to count num_of_cities
    {
        if (a > num_of_cities)
            num_of_cities = a;
        if (b > num_of_cities)
            num_of_cities = b;
    }
    dataFile.close();
    _num_of_cities = num_of_cities;

    initVectors();
    fillVectors(file_name);
}

void AntsColony::initVectors()
{
    int pow_num_of_cities = pow(_num_of_cities, 2);
    _graph = new int[pow_num_of_cities]();
    _visibility = new double[pow_num_of_cities]();
    _pheromone = new double[pow_num_of_cities]();
    //TODO init _pheromone table?
    for (int i = 0; i < _num_of_cities; i++)
    {
        list<int> temp;
        _nodes.push_back(temp);
    }
}

void AntsColony::fillVectors(const char *file_name)
{
    ifstream dataFile;
    dataFile.open(file_name);
    int a, b, c;
    while (dataFile >> a >> b >> c)
    {
        _graph[(a - 1) * _num_of_cities + (b - 1)] = _graph[(b - 1) * _num_of_cities + (a - 1)] = c;
        _visibility[(a - 1) * _num_of_cities + (b - 1)] = _visibility[(b - 1) * _num_of_cities + (a - 1)] = pow((1 / static_cast<double>(c)), sBETA);
        addNode(a, b);
    }
    dataFile.close();
    displayMatrices();
}

void AntsColony::addNode(int start, int stop)
{
    _nodes[start - 1].insert(_nodes[start - 1].end(), stop);
    _nodes[stop - 1].insert(_nodes[stop - 1].end(), start);
}

void AntsColony::displayMatrices()
{
    for (int i = 0; i < _num_of_cities; i++)
    {
        for (int j = 0; j < _num_of_cities; j++)
        {
            cout << _graph[i * _num_of_cities + j] << " ";
        }
        cout << endl;
    }

    cout << endl;

    for (int i = 0; i < _num_of_cities; i++)
    {
        for (int j = 0; j < _num_of_cities; j++)
        {
            cout << _visibility[i * _num_of_cities + j] << " ";
        }
        cout << endl;
    }

    for (int i = 0; i < _num_of_cities; i++)
    {
        for (list<int>::iterator it = _nodes[i].begin(); it != _nodes[i].end(); ++it)
        {
            cout << (*it) << " ";
        }
        cout << endl;
    }
}

void AntsColony::bestRoute()
{
    for (int i = 1; i <= sITER; i++)
    {
        cout << "Iteration " << i << " has started..." << endl;

        for (int j = 0; j < sANTS; j++)
        {
            cout << "Ant nr #" << j << " has started its adventure" << endl;

            for (int k = 0; k < _num_of_cities; k++)
            {
                for (int l = 0; l < _num_of_cities; l++)
                {
                    if (_graph[(((_start_city - 1) + k) % _num_of_cities) * _num_of_cities + l] == 0)
                        continue;
                    //TODO calculate prob of moving to this city
                }
            }
        }
    }
}

#endif

//TODO calculate the possibility to visit other cities from the starter point
//TODO set initialCity and