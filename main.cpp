//TODO STEP 1 - read input file & create matrix
//TODO STEP 2 -

//TODO matrix of cities & their connections - GRAPH
//TODO matrix of visibility (distance)
//TODO matrix of pheromone value

//TODO matrix to store probability

//TODO matrix of deltapheromone (added at the end)

//TODO matrix to store path of ant(s)

#include <iostream>
#include <fstream>
#include <string>
#include "ACO.h"

using namespace std;

int main(int argc, char const *argv[])
{
    if (argc < 2)
        return 0;

    readData(argv[1]);

    return 0;
}