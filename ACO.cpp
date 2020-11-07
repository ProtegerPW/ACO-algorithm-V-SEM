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
    chooseNextCity(1);
    cout << "End for ant #1" << endl;
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

int getNearest(pair<int, double> x, pair<int, double> y, double target)
{
    if (target - x.second >= y.second - target)
        return y.first;
    else
        return x.first;
}

int getClosest(vector<pair<int, double>> input)
{
    double target = randGen();
    int left = 0, right = input.size(), mid = 0;

    while (left < right)
    {
        mid = (left + right) / 2;
        if (target < input[mid].second)
        {
            if (mid > 0 && target > input[mid - 1].second)
                return getNearest(input[mid - 1], input[mid], target);
            right = mid;
        }
        else
        {
            if (mid < input.size() - 1 && target < input[mid + 1].second)
                return getNearest(input[mid], input[mid + 1], target);
            left = mid + 1;
        }
    }
    cout << " Vertice of chosen city is: " << input[mid].first
         << endl;
    return input[mid].first;
}

bool AntsColony::isVisited(int city, int ant)
{
    if (find(_ant_paths[ant].begin(), _ant_paths[ant].end(), city) != _ant_paths[ant].end())
        return true;
    else
        return false;
}

void AntsColony::chooseNextCity(int ant)
{
    _ant_paths[ant].push_back(_start_city); // add start city at the begging of vector

    // for (int j = 0; j < _num_of_cities; j++)
    int activeCity = 0, choseCity = _start_city;

    while (_ant_paths[ant].back() != _finish_city)
    {
        activeCity = choseCity;

        cout << "active City: " << activeCity << endl;

        //stała do wyliczenia miast w macierzy grafu
        int multiplier = activeCity * _num_of_cities;
        vector<pair<int, double>> calculations;

        //szukamy połaczeń z miasta w którym się znajduje mrówka
        for (list<int>::iterator it = _nodes[activeCity].begin(); it != _nodes[activeCity].end(); ++it)
        {
            cout << "Cout city " << *it << endl; //debug
            if (isVisited((*it), ant))
            {
                continue;
            }
            //wyliczenie pierwszej składowej równania (licznika)
            calculations.push_back(make_pair((*it), pow(_pheromone[multiplier + (*it)], sALPHA) * _visibility[multiplier + (*it)]));
        }
        //skończ liczyć trasę, jeżeli nie ma miast do odwiedzenia
        if (calculations.size() == 0)
            break;

        double sum = accumulate(calculations.begin(), calculations.end(), 0.0, [](auto &a, auto &b) { return a + b.second; });

        //obliczenia prawdopodobieństwa wybrania miasta
        for (int i = 0; i < calculations.size(); i++)
        {
            calculations[i].second = (calculations[i].second / sum);
            if (i > 0)
                calculations[i].second = calculations[i].second + calculations[i - 1].second;
            cout << " #" << i << " " << calculations[i].second << endl; //debug
        }
        //wybranie miasta najbliżej losowej wartośći z przedziału (0,1)
        choseCity = getClosest(calculations);
        cout << "Choosen city:  " << choseCity << endl;
        addCityToAnt(choseCity, ant);
        if (choseCity == _finish_city)
            break;
    }
}

void AntsColony::addCityToAnt(int city, int ant)
{
    _ant_paths[ant].push_back(city);

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
                }
            }
        }
    }
}

#endif

//TODO #1 dodać fun aktualizującą poziom feronomu na ścieżce
//TODO #2 dodać fun liczącą długość ścieżki i porównującą ją z obecnym rekordem
//TODO #3 sth to think of