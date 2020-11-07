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
    _num_of_cities = num_of_cities + 1;

    initMatrices();
    fillMatrices(file_name);
}

void AntsColony::initMatrices()
{
    int pow_num_of_cities = pow(_num_of_cities, 2);
    _graph = new int[pow_num_of_cities]();
    _visibility = new double[pow_num_of_cities]();
    _pheromone = new double[pow_num_of_cities]();
    for (int i = 0; i < pow_num_of_cities; i++)
    {
        _pheromone[i] = 1.0;
    }

    for (int i = 0; i < _num_of_cities; i++)
    {
        list<int> temp;
        _nodes.push_back(temp);
    }

    for (int i = 0; i < sANTS + 1; i++)
    {
        vector<int> temp;
        _ant_paths.push_back(temp);
    }
}

void AntsColony::fillMatrices(const char *file_name)
{
    ifstream dataFile;
    dataFile.open(file_name);
    int a, b, c;
    while (dataFile >> a >> b >> c)
    {
        _graph[a * _num_of_cities + b] = _graph[b * _num_of_cities + a] = c;
        _visibility[a * _num_of_cities + b] = _visibility[b * _num_of_cities + a] = pow((1 / static_cast<double>(c)), sBETA);
        addNode(a, b);
    }
    dataFile.close();
    displayMatrices();
    chooseNextCity(_start_city, 1);
}

void AntsColony::addNode(int start, int stop)
{
    _nodes[start].insert(_nodes[start].end(), stop);
    _nodes[stop].insert(_nodes[stop].end(), start);
}

void AntsColony::displayMatrices()
{
    for (int i = 0; i < _num_of_cities; i++)
    {
        if (i == 0)
            continue;
        for (int j = 0; j < _num_of_cities; j++)
        {
            if (j == 0)
                continue;
            cout << _graph[i * _num_of_cities + j] << " ";
        }
        cout << endl;
    }

    cout << endl;

    for (int i = 0; i < _num_of_cities; i++)
    {
        if (i == 0)
            continue;
        for (int j = 0; j < _num_of_cities; j++)
        {
            if (j == 0)
                continue;
            cout << _visibility[i * _num_of_cities + j] << " ";
        }
        cout << endl;
    }

    for (int i = 0; i < _num_of_cities; i++)
    {
        if (i == 0)
            continue;
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

bool AntsColony::isVisited(int city, int ant)
{
    if (find(_ant_paths[ant].begin(), _ant_paths[ant].end(), city) != _ant_paths[ant].end())
        return true;
    else
        return false;
}

void AntsColony::chooseNextCity(int start, int ant)
{
    _ant_paths[ant].push_back(start); // add start city

    for (int j = 0; j < _num_of_cities; j++)
    {
        int activeCity = (start + j) % _num_of_cities;
        cout << "active City: " << activeCity << endl;

        //pomijamy miasto zerowe
        if (activeCity == 0)
            continue;

        //pomijamy miasto, jeżeli mrówka już przez nie przeszła
        if (j != 0 && isVisited(activeCity, ant))
            continue;

        int multiplier = activeCity * _num_of_cities;
        vector<double> calculations;

        for (list<int>::iterator it = _nodes[activeCity].begin(); it != _nodes[activeCity].end(); ++it)
        {
            cout << "Cout city " << *it << endl;
            if (isVisited((*it), ant))
                continue;

            calculations.push_back(pow(_pheromone[multiplier + (*it)], sALPHA) * _visibility[multiplier + (*it)]);
        }
        double sum = accumulate(calculations.begin(), calculations.end(), 0.0);

        //debbuging -------------------------------------------
        cout << "Probability: " << endl;
        for (double temp : calculations)
        {
            cout << temp << endl;
        }
        cout << endl;
        cout << "Calculations size: " << calculations.size() << endl;
        //----------------------------------------------------

        for (int i = 0; i < calculations.size(); i++)
        {
            calculations[i] = (calculations[i] / sum);
            if (i > 0)
                calculations[i] = calculations[i] + calculations[i - 1];
            cout << " #" << i << " " << calculations[i] << endl;
        }
        vector<double>::iterator it = find(calculations.begin(), calculations.end(), getClosest(calculations));
        cout << "Choosen path " << it - calculations.begin() << endl;
        addCityToAnt(start, it - calculations.begin(), ant);
    }
}

void AntsColony::addCityToAnt(int start, int path, int ant)
{
    list<int>::iterator it = _nodes[start].begin();
    advance(it, path);
    _ant_paths[ant].push_back(*it);

    cout << "Ant #" << ant << " goes through ";

    for (int temp : _ant_paths[ant])
    {
        cout << temp << " ";
    }
    cout << endl;
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