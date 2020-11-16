#include "ACO.h"

bool AntColony::Ant::isStuck() const {
    return stuck_;
}

int AntColony::Ant::getDistance() const {
    return distance_covered_;
}

unsigned int AntColony::Ant::getPathTakenSize() const {
    return path_taken_.size();
}

    // TODO co jesli index wyjdzie poza zakres?
int AntColony::Ant::getPathTakenValue( int index ) const {
    if( index < 0 ) {
        return path_taken_.end()[index];
    }
    return path_taken_[index];
}

vector<int>::const_iterator AntColony::Ant::getPathTakenIterBegin() const {
    return path_taken_.begin();
}

vector<int>::const_iterator AntColony::Ant::getPathTakenIterEnd() const {
    return path_taken_.end();
}

void AntColony::Ant::resetStats()
{
    path_taken_.clear();
    distance_covered_ = 0;
    stuck_ = false;
}

void AntColony::Ant::exploreGraph(int id_start_node, int id_end_node)
{
    int id_curr_node, id_chosen_node, matrix_index;

    // TODO co jesli wierzcholek poczatkowy nie ma polaczen w ogole? 
    // JM: hmm good question - trzeba zrobić, że niemożliwe jest dojście do end_node

    id_curr_node = id_start_node;
    path_taken_.push_back(id_start_node);

    while (id_curr_node != id_end_node)
    {
        analyzeOptions(id_curr_node);    // ant analyzes available route options
        if (stuck_ == true)     // if ant has no way to go
            break;
        id_chosen_node = chooseOption(); // ant chooses a node to go to
                                         // update ant's statistics
        path_taken_.push_back(id_chosen_node);
        matrix_index = id_curr_node * ptr_colony_->num_nodes_ + id_chosen_node;
        distance_covered_ += ptr_colony_->graph_[matrix_index];
        cout << ">> Chosen node: " << id_chosen_node << endl;

        path_options_.clear(); // reset options

        id_curr_node = id_chosen_node; // ant travels to the next node
    }

    if (stuck_ == true)
        cout << "--- Ant got stuck." << endl;
    else
    {
        cout << "--- Ant found a way: ";
        for (int i : path_taken_)
            cout << i << " ";
        cout << endl
             << "\tPath length: " << distance_covered_ << endl;
    }
}

void AntColony::Ant::analyzeOptions(int id_curr_node)
{
    int matrix_index = id_curr_node * ptr_colony_->num_nodes_;
    double likelihood, sum = 0.0;

    for (list<int>::iterator neighbour = ptr_colony_->connections_[id_curr_node].begin();
         neighbour != ptr_colony_->connections_[id_curr_node].end(); ++neighbour)
    {
        if (!isVisited(*neighbour))
        {
            likelihood = pow(ptr_colony_->pheromone_[matrix_index + *neighbour], sALPHA);
            likelihood *= ptr_colony_->visibility_[matrix_index + *neighbour];
            if (likelihood > 0.0)
            {
                sum += likelihood;
                path_options_.push_back(make_pair(*neighbour, sum));
            }
        }
    }
    //
    if (path_options_.size() == 0)
    {
        stuck_ = true;
        return;
    }
    //
    for (unsigned int i = 0; i < path_options_.size(); ++i)
    {
        path_options_[i].second /= sum;
        cout << "opcja[" << path_options_[i].first << "] = " << path_options_[i].second << endl;
    }
}

bool AntColony::Ant::isVisited(int id_node)
{
    return find(path_taken_.begin(), path_taken_.end(), id_node) != path_taken_.end() ? true : false;
}

int AntColony::Ant::chooseOption()
{
    double random = getRandom();
    cout << "random = " << random << endl;
    for (unsigned int i = 0; i < path_options_.size() - 1; ++i)
    {
        if (random < path_options_[i].second)
            return path_options_[i].first;
    }
    return path_options_.back().first;
}

// TODO zrobic z tego singleton? JM: Tak
double getRandom()
{
    static random_device rd;
    static mt19937 e2(rd());
    static uniform_real_distribution<> dist(0, 1);
    return dist(e2);
}

// // // // //

AntColony::~AntColony()
{
    delete[] graph_;
    delete[] visibility_;
    delete[] pheromone_;
    for (auto &ant : ants_)
        delete ant;
}

void AntColony::findOptimisedRoute(int id_start_node, int id_end_node, int num_iterations)
{
    for (int i = 0; i < num_iterations; ++i)
    {
        for (Ant *ant : ants_)
        {
            cout << "--- Ant begins its journey ---" << endl;
            ant->exploreGraph(id_start_node, id_end_node);
        }
        pheromoneEvaporation();
        updatePheromone();
        for (Ant *ant : ants_)
        {
            if (ant->getDistance() < shortest_path_ && ant->isStuck() == false)
            {
                best_path_.clear();
                copy(ant->getPathTakenIterBegin(), ant->getPathTakenIterEnd(), back_inserter(best_path_));  // ??
                shortest_path_ = ant->getDistance();
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
    int row, column, matrix_index;

    for (Ant *ant : ants_)
    {
        if (ant->isStuck() == true)
        {
            // heurystyka
            row = ant->getPathTakenValue(-2);               // get penultimate node index of path_taken_ vector
            column = ant->getPathTakenValue(-1);            // get last node index of path_taken_ vector
            matrix_index = row * num_nodes_ + column;

            pheromone_[matrix_index] = 0.0;
        }
        else
        {
            delta_pheromone = 1.0 / ant->getDistance();

            for (unsigned int i = 0; i < ant->getPathTakenSize() - 1; ++i)
            {
                row = ant->getPathTakenValue(i);
                column = ant->getPathTakenValue(i + 1);

                // from X to Y
                matrix_index = row * num_nodes_ + column;
                pheromone_[matrix_index] += delta_pheromone;

                // from Y to X - same value, reversed indexes
                matrix_index = column * num_nodes_ + row;
                pheromone_[matrix_index] += delta_pheromone;
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
    for (int node_from = 1; node_from < num_nodes_; ++node_from)
    {
        for (node_to = connections_[node_from].begin(); node_to != connections_[node_from].end(); ++node_to)
        {
            matrix_index = node_from * num_nodes_ + *node_to;
            pheromone_[matrix_index] *= evaporation_coefficient;
        }
    }
}

void AntColony::displayResults()
{
    cout << endl
         << "---------------------------------------" << endl
         << "Optimised path: ";
    for (int i : best_path_)
        cout << i << " ";
    cout << endl
         << "Path length: " << shortest_path_ << endl;
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
    num_nodes_ = ++max_id;
}

void AntColony::initMatrices()
{
    int pow_num_of_nodes = pow(num_nodes_, 2);

    graph_ = new int[pow_num_of_nodes]();
    visibility_ = new double[pow_num_of_nodes]();
    pheromone_ = new double[pow_num_of_nodes]();

    for (int i = 0; i < pow_num_of_nodes; i++)
    {
        pheromone_[i] = 1.0;
    }

    for (int i = 0; i < num_nodes_ + 1; i++)
    {
        list<int> temp;
        connections_.push_back(temp);
    }

    for (int i = 0; i < NUM_OF_ANTS; ++i)
    {
        ants_.push_back(new Ant(this));
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
        matrix_pos1 = id_node1 * num_nodes_ + id_node2;
        matrix_pos2 = id_node2 * num_nodes_ + id_node1;

        graph_[matrix_pos1] = graph_[matrix_pos2] = weight;
        visibility_[matrix_pos1] = visibility_[matrix_pos2] = pow((1 / static_cast<double>(weight)), sBETA);

        addEdge(id_node1, id_node2);
    }
    dataFile.close();
}

void AntColony::addEdge(int id_node1, int id_node2)
{
    connections_[id_node1].push_back(id_node2);
    connections_[id_node2].push_back(id_node1);
}

void AntColony::displayMatrices()
{
    for (int i = 1; i < num_nodes_; i++)
    {
        for (int j = 1; j < num_nodes_; j++)
        {
            cout << graph_[i * num_nodes_ + j] << " ";
        }
        cout << endl;
    }

    cout << endl;

    for (int i = 1; i < num_nodes_; i++)
    {
        for (int j = 1; j < num_nodes_; j++)
        {
            cout << visibility_[i * num_nodes_ + j] << " ";
        }
        cout << endl;
    }

    for (int i = 1; i < num_nodes_; i++)
    {
        for (list<int>::iterator it = connections_[i].begin(); it != connections_[i].end(); ++it)
        {
            cout << (*it) << " ";
        }
        cout << endl;
    }
}

//TODO testy
//TODO implement constexpr