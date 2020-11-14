#include "ACO.h"

void Ant::resetStats()
{
    _path_taken.clear();
    _distance_covered = 0;
    _is_stuck = false;
}

void Ant::exploreGraph(int id_start_node, int id_end_node)
{
    int id_curr_node, id_chosen_node, matrix_index;

    // TODO co jesli wierzcholek poczatkowy nie ma polaczen w ogole? JM: hmm good question - trzeba zrobić, że niemożliwe jest dojście do end_node

    id_curr_node = id_start_node;
    _path_taken.push_back(id_start_node);

    while (id_curr_node != id_end_node)
    {
        analyzeOptions(id_curr_node); // ant analyzes available route options
        if (_is_stuck == true)        // if ant has no way to go
            break;
        id_chosen_node = chooseOption(); // ant chooses a node to go to
                                         // update ant's statistics
        _path_taken.push_back(id_chosen_node);
        matrix_index = id_curr_node * _ptr_colony->_num_of_nodes + id_chosen_node;
        _distance_covered += _ptr_colony->_graph[matrix_index];
        cout << ">> Chosen node: " << id_chosen_node << endl;

        _path_options.clear(); // reset options

        id_curr_node = id_chosen_node; // ant travels to the next node
    }

    if (_is_stuck == true)
        cout << "--- Ant got stuck." << endl;
    else
    {
        cout << "--- Ant found a way: ";
        for (int i : _path_taken)
            cout << i << " ";
        cout << endl
             << "\tPath length: " << _distance_covered << endl;
    }
}

void Ant::analyzeOptions(int id_curr_node)
{
    int matrix_index = id_curr_node * _ptr_colony->_num_of_nodes;
    double likelihood, sum = 0.0;

    for (list<int>::iterator neighbour = _ptr_colony->_connections[id_curr_node].begin();
         neighbour != _ptr_colony->_connections[id_curr_node].end(); ++neighbour)
    {
        if (!isVisited(*neighbour))
        {
            likelihood = pow(_ptr_colony->_pheromone[matrix_index + *neighbour], sALPHA);
            likelihood *= _ptr_colony->_visibility[matrix_index + *neighbour];
            if (likelihood > 0.0)
            {
                sum += likelihood;
                _path_options.push_back(make_pair(*neighbour, sum));
            }
        }
    }
    //
    if (_path_options.size() == 0)
    {
        _is_stuck = true;
        return;
    }
    //
    for (unsigned int i = 0; i < _path_options.size(); ++i)
    {
        _path_options[i].second /= sum;
        cout << "opcja[" << _path_options[i].first << "] = " << _path_options[i].second << endl;
    }
}

bool Ant::isVisited(int id_node)
{
    return find(_path_taken.begin(), _path_taken.end(), id_node) != _path_taken.end() ? true : false;
}

int Ant::chooseOption()
{
    double random = getRandom();
    cout << "random = " << random << endl;
    for (unsigned int i = 0; i < _path_options.size() - 1; ++i)
    {
        if (random < _path_options[i].second)
            return _path_options[i].first;
    }
    return _path_options.back().first;
}

// TODO zrobic z tego singleton? JM: Tak
double getRandom()
{
    static random_device rd;
    static mt19937 e2(rd());
    static uniform_real_distribution<> dist(0, 1);
    return dist(e2);
}

// // //

AntColony::~AntColony()
{
    delete[] _graph;
    delete[] _visibility;
    delete[] _pheromone;
    for (auto &ant : _ants)
        delete ant;
}

void AntColony::findOptimisedRoute(int id_start_node, int id_end_node, int num_iterations)
{
    for (int i = 0; i < num_iterations; ++i)
    {
        for (Ant *ant : _ants)
        {
            cout << "--- Ant begins its journey ---" << endl;
            ant->exploreGraph(id_start_node, id_end_node);
        }
        pheromoneEvaporation();
        updatePheromone();
        for (Ant *ant : _ants)
        {
            if (ant->_distance_covered < _shortest_path && ant->_is_stuck == false)
            {
                _best_path.clear();
                copy(ant->_path_taken.begin(), ant->_path_taken.end(), back_inserter(_best_path));
                _shortest_path = ant->_distance_covered;
            }
            //
            ant->resetStats();
        }
    }
}

void AntColony::updatePheromone()
{
    cout << "updatePheromone()" << endl;
    double delta_pheromone;
    int row, column, matrix_index, distance;

    for (Ant *ant : _ants)
    {
        if (ant->_is_stuck == true)
        {
            // heurystyka
            row = ant->_path_taken.end()[-2];
            column = ant->_path_taken.end()[-1];
            matrix_index = row * _num_of_nodes + column;

            _pheromone[matrix_index] = 0.0;
        }
        else
        {
            delta_pheromone = 1.0 / (ant->_distance_covered);

            for (unsigned int i = 0; i < ant->_path_taken.size() - 1; ++i)
            {
                row = ant->_path_taken[i];
                column = ant->_path_taken[i + 1];
                matrix_index = row * _num_of_nodes + column;

                //for X to Y city
                distance = _graph[matrix_index];
                _pheromone[matrix_index] += delta_pheromone;

                //for Y to X city - same value but different index
                matrix_index = column * _num_of_nodes + row;
                _pheromone[matrix_index] += delta_pheromone;
            }
        }
    }
}

void AntColony::pheromoneEvaporation()
{
    cout << "pheromoneEvaporation()" << endl;
    int matrix_index;
    double evaporation_coefficient = 1 - RHO;
    list<int>::iterator node_to;
    for (int node_from = 1; node_from < _num_of_nodes; ++node_from)
    {
        for (node_to = _connections[node_from].begin(); node_to != _connections[node_from].end(); ++node_to)
        {
            matrix_index = node_from * _num_of_nodes + *node_to;
            _pheromone[matrix_index] *= evaporation_coefficient;
        }
    }
}

void AntColony::displayResults()
{
    cout << endl
         << "---------------------------------------" << endl
         << "Optimised path: ";
    for (int i : _best_path)
        cout << i << " ";
    cout << endl
         << "Path length: " << _shortest_path << endl;
}

void AntColony::scanData(const char *file_name)
{
    ifstream dataFile;
    dataFile.open(file_name);
    // TODO if fails -> abort mission
    int id_node1, id_node2, junk, max_id = 0;
    while (dataFile >> id_node1 >> id_node2 >> junk) // looking for maximum node ID in the file
    {
        if (id_node1 > max_id)
            max_id = id_node1;
        if (id_node2 > max_id)
            max_id = id_node2;
    }
    dataFile.close();
    _num_of_nodes = ++max_id;
}

void AntColony::initMatrices()
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

    for (int i = 0; i < NUM_OF_ANTS; ++i)
    {
        _ants.push_back(new Ant(this));
    }
}

void AntColony::fillMatrices(const char *file_name)
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
}

void AntColony::addEdge(int id_node1, int id_node2)
{
    _connections[id_node1].push_back(id_node2);
    _connections[id_node2].push_back(id_node1);
}

void AntColony::displayMatrices()
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

//TODO testy
//TODO implement nested classes
//TODO implement constexpr