#include "ACO.h"

void Ant::resetAnt() {
    cout << "Resetting ant" << endl;
    _path_taken.clear();
    _distance_covered = 0;
    _is_stuck = false;
}

void Ant::exploreGraph(int id_start_node, int id_end_node) {
    int matrix_index, id_curr_node, id_chosen_node;
    double likelihood, sum; 

    list<int>::iterator neighbour;

    // TODO co jesli wierzcholek poczatkowy nie ma polaczen w ogole? 
    
    id_curr_node = id_start_node;
    
    while( id_curr_node != id_end_node && _is_stuck == false ) {
        if( _ptr_colony->_connections[id_curr_node].size() == 1 )
            _is_stuck = true;
        else {
            matrix_index = id_curr_node * _ptr_colony->_num_of_nodes;
            sum = 0.0;
                // ant analizes neighbours
            neighbour = _ptr_colony->_connections[id_curr_node].begin();
            while( neighbour != _ptr_colony->_connections[id_curr_node].end() ) {
                if( isVisited( *neighbour ) == false ) {
                    likelihood = pow(_ptr_colony->_pheromone[matrix_index + *neighbour], sALPHA);
                    likelihood *= _ptr_colony->_visibility[matrix_index + *neighbour];
                    sum += likelihood;
                    _path_options.push_back( make_pair( *neighbour, likelihood ) );
                }
                ++neighbour;
            }
            for( unsigned int i = 0; i < _path_options.size(); ++i ) {
                _path_options[i].second /= sum;
            }
                // ant chooses a node
            id_chosen_node = chooseOption();
                // ant goes to that node
            _path_taken.push_back( id_chosen_node );

            id_curr_node = id_chosen_node;
        }
    }
    // if( is_stuck == true )
        // heuristic
}

bool Ant::isVisited(int id_node) {
    return (find(_path_taken.begin(), _path_taken.end(), id_node) != _path_taken.end())? true: false;
}

int Ant::chooseOption() {
    double random = getRandom();

}

double getRandom() {
    static random_device rd;
    static mt19937 e2(rd());
    static uniform_real_distribution<> dist(0, 1);
    return dist(e2);
}

// // //

AntColony::~AntColony() {
    delete _graph;
    delete _visibility;
    delete _pheromone;
    for( auto &ant: _ants )
        delete ant;
}

void AntColony::findOptimisedRoute(int id_start_node, int id_end_node, int num_iterations) {
    for(int i = 0; i < num_iterations; ++i) {
        for( Ant *ant: _ants ) {
            ant->resetAnt();
        }
        for( Ant *ant: _ants ) {
            ant->exploreGraph( id_start_node, id_end_node );
        }
        // updatePheromoneMatrix()
        // pheromoneEvaporation()
    }
}

void AntColony::scanData(const char *file_name)
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

    for( int i = 0; i < NUM_OF_ANTS; ++i) {
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
    displayMatrices();
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

    // TODO implement <0,1) distribution from <random>
double AntColony::randGen()
{
    srand(time(NULL));
    double temp = (double)rand() / RAND_MAX;
    cout << "Random #" << temp << endl;
    return temp;
}

/*int getNearest(pair<int, double> x, pair<int, double> y, double target)
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

bool AntColony::isVisited(int city, int ant)
{
    if (find(_ant_paths[ant].begin(), _ant_paths[ant].end(), city) != _ant_paths[ant].end())
        return true;
    else
        return false;
}

void AntColony::chooseNextNode(int ant)
{
    _ant_paths[ant].push_back(_start_node); // add start city at the begging of vector

    // for (int j = 0; j < _num_of_nodes; j++)
    int activeNode = 0, chosenNode = _start_node;

    while (_ant_paths[ant].back() != _finish_node)
    {
        activeNode = chosenNode;

        cout << "active City: " << activeNode << endl;

        //stała do wyliczenia miast w macierzy grafu
        int multiplier = activeNode * _num_of_nodes;
        vector<pair<int, double>> calculations;

        //szukamy połaczeń z miasta w którym się znajduje mrówka
        for (list<int>::iterator it = _connections[activeNode].begin(); it != _connections[activeNode].end(); ++it)
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
        chosenNode = getClosest(calculations);
        cout << "Choosen city:  " << chosenNode << endl;
        addNodeToAnt(chosenNode, ant);
        if (chosenNode == _finish_node)
            break;
    }
}

void AntColony::addNodeToAnt(int city, int ant)
{
    _ant_paths[ant].push_back(city);

    cout << "Ant #" << ant << " goes through ";

    for (int temp : _ant_paths[ant])
    {
        cout << temp << " ";
    }
    cout << endl;
}

void AntColony::bestRoute()
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
                }
            }
        }
    }
}
*/

//TODO #1 dodać fun aktualizującą poziom feronomu na ścieżce
//TODO #2 dodać fun liczącą długość ścieżki i porównującą ją z obecnym rekordem
//TODO #3 sth to think of