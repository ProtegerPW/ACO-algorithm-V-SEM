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
    for (int i = 0; i < pow(_num_of_cities, 2); i++)
    {
        _pheromone[i] = 1.0;
    }
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
    sth(_start_city);
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

double randGen()
{
    srand(time(NULL));
    double temp = (double)rand() / RAND_MAX;
    cout << " Random #" << temp << endl;
    return temp;
}

double getNearest(double x, double y, double target)
{
    if (target - x >= y - target)
        return y;
    else
        return x;
}

double getClosest(vector<double> input)
{
    double target = randGen();
    int left = 0, right = input.size(), mid = 0;

    while (left < right)
    {
        mid = (left + right) / 2;
        if (target < input[mid])
        {
            if (mid > 0 && target > input[mid - 1])
                return getNearest(input[mid - 1], input[mid], target);
            right = mid;
        }
        else
        {
            if (mid < input.size() - 1 && target < input[mid + 1])
                return getNearest(input[mid], input[mid + 1], target);
            left = mid + 1;
        }
    }
    return input[mid];
}

void AntsColony::sth(int start)
{

    vector<double> calculations;
    int multiplier = (start - 1) * _num_of_cities;

    for (list<int>::iterator it = _nodes[start - 1].begin(); it != _nodes[start - 1].end(); ++it)
    {
        calculations.push_back(pow(_pheromone[multiplier + ((*it) - 1)], sALPHA) * _visibility[multiplier + ((*it) - 1)]);
    }
    double sum = accumulate(calculations.begin(), calculations.end(), 0.0);

    //debbuging -------------------------------------------
    cout << endl;
    for (double temp : calculations)
    {
        cout << temp << endl;
    }
    cout << endl;
    cout << calculations.size() << endl;
    //----------------------------------------------------

    for (int i = 0; i < calculations.size(); i++)
    {
        calculations[i] = (calculations[i] / sum);
        if (i > 0)
            calculations[i] = calculations[i] + calculations[i - 1];
        cout << calculations[i] << endl;
    }
    auto it = find(calculations.begin(), calculations.end(), getClosest(calculations));
    cout << "Choosen path " << it - calculations.begin() << endl;
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

//TODO #1 calculate the possibility to visit other cities from the starter point
//TODO #2 generate random num between 0 and 1

//TODO #3 sth to think of