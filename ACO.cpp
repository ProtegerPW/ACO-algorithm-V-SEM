#include "ACO.h"

AntsColony::~AntsColony() {
    delete _graph;
    delete _visibility;
    delete _pheromone;
}

void AntsColony::scanData(const char *file_name)
{
    ifstream dataFile;
    dataFile.open(file_name);
    // TODO if fails -> abort mission
    int id_node1, id_node2, junk, max_id = 0;
    while (dataFile >> id_node1 >> id_node2 >> junk)    // looking for maximum node ID in the file
    {
        if (id_node1 > max_id)
            max_id = id_node1;
        if (id_node2 > max_id)
            max_id = id_node2;
    }
    dataFile.close();
    _num_of_nodes = ++max_id;
}

void AntsColony::initMatrices()
{
    int pow_num_of_nodes = pow(_num_of_nodes, 2);

    _graph = new int[pow_num_of_nodes]();
    _visibility = new double[pow_num_of_nodes]();
    _pheromone = new double[pow_num_of_nodes]();

    for (int i = 0; i < pow_num_of_nodes; i++)
    {
        _pheromone[i] = 1.0;
    }

    for (int i = 0; i < _num_of_nodes + 1; i++)
    {
        list<int> temp;
        _connections.push_back(temp);
    }

    for (int i = 0; i < NUM_OF_ANTS + 1; i++)       // ?
    {
        vector<int> temp;
        _ant_paths.push_back(temp);
    }
}

void AntsColony::fillMatrices(const char *file_name)
{
    ifstream dataFile;
    dataFile.open(file_name);
    // TODO if fails -> abort mission
    int id_node1, id_node2, weight;
    int matrix_pos1, matrix_pos2;
    while (dataFile >> id_node1 >> id_node2 >> weight)
    {
        matrix_pos1 = id_node1 * _num_of_nodes + id_node2;
        matrix_pos2 = id_node2 * _num_of_nodes + id_node1;

        _graph[matrix_pos1] = _graph[matrix_pos2] = weight;
        _visibility[matrix_pos1] = _visibility[matrix_pos2] = pow((1 / static_cast<double>(weight)), sBETA);
        
        addEdge(id_node1, id_node2);
    }
    dataFile.close();
    displayMatrices();
    // chooseNextNode(_start_node, 1);                // ?
}

void AntsColony::addEdge(int id_node1, int id_node2)
{
    _connections[id_node1].push_back(id_node2);
    _connections[id_node2].push_back(id_node1);
}

void AntsColony::displayMatrices()
{
    for (int i = 1; i < _num_of_nodes; i++)
    {
        for (int j = 1; j < _num_of_nodes; j++)
        {
            cout << _graph[i * _num_of_nodes + j] << " ";
        }
        cout << endl;
    }

    cout << endl;

    for (int i = 1; i < _num_of_nodes; i++)
    {
        for (int j = 1; j < _num_of_nodes; j++)
        {
            cout << _visibility[i * _num_of_nodes + j] << " ";
        }
        cout << endl;
    }

    for (int i = 1; i < _num_of_nodes; i++)
    {
        for (list<int>::iterator it = _connections[i].begin(); it != _connections[i].end(); ++it)
        {
            cout << (*it) << " ";
        }
        cout << endl;
    }
}

    // TODO implement <0,1) distribution from <random>
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

void AntsColony::chooseNextNode(int start, int ant)
{
    _ant_paths[ant].push_back(start); // add start city

    for (int j = 0; j < _num_of_nodes; j++)
    {
        int activeCity = (start + j) % _num_of_nodes;
        cout << "active City: " << activeCity << endl;

        //pomijamy miasto zerowe
        if (activeCity == 0)
            continue;

        //pomijamy miasto, jeżeli mrówka już przez nie przeszła
        if (j != 0 && isVisited(activeCity, ant))
            continue;

        int multiplier = activeCity * _num_of_nodes;
        vector<double> calculations;

        for (list<int>::iterator it = _connections[activeCity].begin(); it != _connections[activeCity].end(); ++it)
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
    list<int>::iterator it = _connections[start].begin();
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

            for (int k = 0; k < _num_of_nodes; k++)
            {
                for (int l = 0; l < _num_of_nodes; l++)
                {
                    if (_graph[(((_start_node - 1) + k) % _num_of_nodes) * _num_of_nodes + l] == 0)
                        continue;
                    //TODO calculate prob of moving to this city
                }
            }
        }
    }
}

//TODO #1 calculate the possibility to visit other cities from the starter point
//TODO #2 generate random num between 0 and 1

//TODO #3 sth to think of