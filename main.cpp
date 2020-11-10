#include <iostream>
#include <fstream>
#include <string>
#include "ACO.h"

using namespace std;

int main(int argc, char const *argv[])
{
    if (argc < 4)
    {
        cout << "Not enough arguments" << endl;
        return 0;
    }
    int id_start_node = atoi(argv[2]), id_end_node = atoi(argv[3]);
    const char *file_name = argv[1];

    AntColony ACO;
    ACO.scanData(file_name);
    ACO.initMatrices();
    ACO.fillMatrices(file_name);
    ACO.findOptimisedRoute(id_start_node, id_end_node, 5);
}